#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>
#include <time.h>

#define MAX_CLIENTS 10
#define BUF_SIZE 512
#define ID_SIZE 32

// Struktura przechowująca dane pojedynczego klienta
typedef struct {
    char id[ID_SIZE];           // identyfikator klienta (nadawany przez klienta)
    struct sockaddr_in addr;    // adres sieciowy klienta (IP i port)
    int alive;                  // flaga "żywy" do pingowania (0 - nie odpowiada, 1 - odpowiada)
} Client;

// Statyczna tablica klientów - ogranicza liczbę jednoczesnych połączeń
Client clients[MAX_CLIENTS];

// Mutex do synchronizacji dostępu do tablicy klientów
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

// Deklaracje funkcji pomocniczych
void broadcast(const char *msg, const struct sockaddr_in *except_addr);
void send_to_one(const char *id, const char *msg, const char *from_id);
void list_clients(const struct sockaddr_in *client_addr);
void remove_client(const struct sockaddr_in *client_addr);
void *ping_clients(void *arg);
void current_time(char *buffer);
int find_client_by_addr(const struct sockaddr_in *addr);
int find_client_by_id(const char *id);

int server_fd; // deskryptor socketu serwera

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <IP> <PORT>\n", argv[0]);
        return 1;
    }

    // Tworzenie socketu serwera (IPv4, UDP)
    server_fd = socket(AF_INET, SOCK_DGRAM, 0);
    if (server_fd < 0) { perror("socket"); exit(1); }

    // Konfiguracja adresu serwera
    struct sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[1]); // adres IP z argumentu
    serv_addr.sin_port = htons(atoi(argv[2]));      // port z argumentu

    // Bindowanie socketu do adresu i portu
    if (bind(server_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("bind"); exit(1);
    }

    printf("UDP Server running on %s:%s\n", argv[1], argv[2]);

    // Uruchomienie wątku pingującego klientów (sprawdza, czy klienci odpowiadają)
    pthread_t pinger;
    pthread_create(&pinger, NULL, ping_clients, NULL);

    // Główna pętla odbierająca wiadomości od klientów
    while (1) {
        char buffer[BUF_SIZE];
        struct sockaddr_in cli_addr;
        socklen_t cli_len = sizeof(cli_addr);

        // Odbieranie wiadomości od klienta (UDP)
        int n = recvfrom(server_fd, buffer, sizeof(buffer) - 1, 0,
                         (struct sockaddr*)&cli_addr, &cli_len);
        if (n <= 0) continue;

        buffer[n] = '\0'; // zakończenie stringa

        pthread_mutex_lock(&clients_mutex);
        int idx = find_client_by_addr(&cli_addr);
        if (idx == -1) {
            // Nowy klient - oczekujemy, że pierwsza wiadomość to ID
            int i;
            for (i = 0; i < MAX_CLIENTS; i++) {
                if (clients[i].id[0] == '\0') { // szukamy wolnego miejsca
                    strncpy(clients[i].id, buffer, ID_SIZE - 1); // kopiujemy ID klienta
                    clients[i].id[ID_SIZE - 1] = '\0';
                    clients[i].addr = cli_addr; // zapisujemy adres klienta
                    clients[i].alive = 1;       // klient jest "żywy"
                    printf("Client connected: %s\n", clients[i].id);
                    break;
                }
            }
            if (i == MAX_CLIENTS) {
                // Brak miejsca na nowych klientów
                char msg[] = "Max clients reached.\n";
                sendto(server_fd, msg, strlen(msg), 0,
                       (struct sockaddr*)&cli_addr, cli_len);
            }
            pthread_mutex_unlock(&clients_mutex);
            continue;
        }
        pthread_mutex_unlock(&clients_mutex);

        // Klient już zarejestrowany - obsługa poleceń
        clients[idx].alive = 1; // klient odpowiedział, więc jest "żywy"

        // Obsługa poleceń klienta
        if (strncmp(buffer, "LIST", 4) == 0) {
            // Klient prosi o listę aktywnych klientów
            list_clients(&cli_addr);
        } else if (strncmp(buffer, "2ALL", 4) == 0) {
            // Klient wysyła wiadomość do wszystkich
            char msg[BUF_SIZE], timebuf[64];
            current_time(timebuf); // pobierz aktualny czas
            snprintf(msg, sizeof(msg), "FROM %s %s %s", clients[idx].id, timebuf, buffer + 5);
            broadcast(msg, &cli_addr); // wyślij do wszystkich poza nadawcą
        } else if (strncmp(buffer, "2ONE", 4) == 0) {
            // Klient wysyła wiadomość do jednego klienta
            char target_id[ID_SIZE], *p = strchr(buffer + 5, ' ');
            if (p) {
                *p = '\0'; // oddziel ID od wiadomości
                strcpy(target_id, buffer + 5); // pobierz ID odbiorcy
                send_to_one(target_id, p + 1, clients[idx].id); // wyślij wiadomość
            }
        } else if (strncmp(buffer, "STOP", 4) == 0) {
            // Klient kończy pracę
            printf("Client %s disconnected.\n", clients[idx].id);
            remove_client(&cli_addr);
        } else if (strncmp(buffer, "IM_ALIVE", 8) == 0) {
            // Odpowiedź klienta na ping
            clients[idx].alive = 1;
        }
        // Inne polecenia można dodać tutaj
    }

    return 0;
}

// Wysyła wiadomość do wszystkich klientów poza nadawcą (except_addr)
void broadcast(const char *msg, const struct sockaddr_in *except_addr) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        // Sprawdzamy, czy klient jest aktywny i czy nie jest nadawcą
        if (clients[i].id[0] != '\0' &&
            (except_addr == NULL ||
             memcmp(&clients[i].addr, except_addr, sizeof(struct sockaddr_in)) != 0)) {
            // Wysyłamy wiadomość przez UDP do klienta
            sendto(server_fd, msg, strlen(msg), 0,
                   (struct sockaddr*)&clients[i].addr, sizeof(clients[i].addr));
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

// Wysyła wiadomość do jednego klienta o podanym ID
void send_to_one(const char *id, const char *msg, const char *from_id) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].id[0] != '\0' && strcmp(clients[i].id, id) == 0) {
            char buffer[BUF_SIZE], timebuf[64];
            current_time(timebuf);
            // Format: FROM <nadawca> <data> <wiadomość>
            snprintf(buffer, sizeof(buffer), "FROM %s %s %s", from_id, timebuf, msg);
            sendto(server_fd, buffer, strlen(buffer), 0,
                   (struct sockaddr*)&clients[i].addr, sizeof(clients[i].addr));
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

// Wysyła listę aktywnych klientów do wskazanego klienta
void list_clients(const struct sockaddr_in *client_addr) {
    pthread_mutex_lock(&clients_mutex);
    char buffer[BUF_SIZE] = "ACTIVE CLIENTS:\n";
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].id[0] != '\0') {
            strcat(buffer, clients[i].id);
            strcat(buffer, "\n");
        }
    }
    // Wysyłamy listę przez UDP do klienta
    sendto(server_fd, buffer, strlen(buffer), 0,
           (struct sockaddr*)client_addr, sizeof(*client_addr));
    pthread_mutex_unlock(&clients_mutex);
}

// Usuwa klienta z tablicy na podstawie adresu
void remove_client(const struct sockaddr_in *client_addr) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        // Porównujemy adresy klienta
        if (clients[i].id[0] != '\0' &&
            memcmp(&clients[i].addr, client_addr, sizeof(struct sockaddr_in)) == 0) {
            clients[i].id[0] = '\0'; // oznaczamy miejsce jako wolne
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

// Wątek cyklicznie pingujący klientów i usuwający nieaktywnych
void *ping_clients(void *arg) {
    (void)arg;
    while (1) {
        sleep(10); // co 10 sekund
        pthread_mutex_lock(&clients_mutex);
        for (int i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i].id[0] != '\0') {
                if (clients[i].alive) {
                    // Jeśli klient był "żywy", ustawiamy alive na 0 i wysyłamy ping
                    clients[i].alive = 0;
                    sendto(server_fd, "ALIVE?", 6, 0,
                           (struct sockaddr*)&clients[i].addr, sizeof(clients[i].addr));
                } else {
                    // Jeśli klient nie odpowiedział na poprzedni ping, usuwamy go
                    printf("Removing inactive client: %s\n", clients[i].id);
                    clients[i].id[0] = '\0';
                }
            }
        }
        pthread_mutex_unlock(&clients_mutex);
    }
    return NULL;
}

// Zwraca aktualny czas jako string w formacie YYYY-MM-DD HH:MM:SS
void current_time(char *buffer) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, 64, "%Y-%m-%d %H:%M:%S", t);
}

// Znajduje indeks klienta po adresie
int find_client_by_addr(const struct sockaddr_in *addr) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        // Porównujemy adresy IP i porty
        if (clients[i].id[0] != '\0' &&
            memcmp(&clients[i].addr, addr, sizeof(struct sockaddr_in)) == 0) {
            return i;
        }
    }
    return -1;
}

// Znajduje indeks klienta po ID
int find_client_by_id(const char *id) {
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].id[0] != '\0' && strcmp(clients[i].id, id) == 0) {
            return i;
        }
    }
    return -1;
}