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
    int sockfd;                 // deskryptor socketu klienta
    char id[ID_SIZE];           // identyfikator klienta (nadawany przez klienta)
    struct sockaddr_in addr;    // adres sieciowy klienta
    int alive;                  // flaga "żywy" do pingowania (0 - nie odpowiada, 1 - odpowiada)
} Client;

// Statyczna tablica klientów - ogranicza liczbę jednoczesnych połączeń
Client clients[MAX_CLIENTS];

// Mutex do synchronizacji dostępu do tablicy klientów
pthread_mutex_t clients_mutex = PTHREAD_MUTEX_INITIALIZER;

// Deklaracje funkcji pomocniczych
void broadcast(const char *msg, int except_fd);
void send_to_one(const char *id, const char *msg);
void list_clients(int client_fd);
void remove_client(int client_fd);
void *handle_client(void *arg);
void *ping_clients(void *arg);
void current_time(char *buffer);

int main(int argc, char *argv[]) {
    if (argc != 3) {
        printf("Usage: %s <IP> <PORT>\n", argv[0]);
        return 1;
    }

    // Tworzenie socketu serwera (IPv4, TCP)
    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
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

    // Nasłuchiwanie na połączenia przychodzące (maksymalnie 5 oczekujących)
    listen(server_fd, 5);
    printf("Server running on %s:%s\n", argv[1], argv[2]);

    // Uruchomienie wątku pingującego klientów (sprawdza, czy klienci odpowiadają)
    pthread_t pinger;
    pthread_create(&pinger, NULL, ping_clients, NULL);

    // Główna pętla akceptująca nowych klientów
    while (1) {
        struct sockaddr_in cli_addr;
        socklen_t cli_len = sizeof(cli_addr);
        // Akceptowanie nowego połączenia
        int client_fd = accept(server_fd, (struct sockaddr*)&cli_addr, &cli_len);
        if (client_fd < 0) continue; // jeśli błąd, próbujemy dalej

        // Dodanie klienta do tablicy klientów
        pthread_mutex_lock(&clients_mutex);
        int i;
        for (i = 0; i < MAX_CLIENTS; i++) {
            if (clients[i].sockfd == 0) { // szukamy wolnego miejsca
                clients[i].sockfd = client_fd;
                clients[i].addr = cli_addr;
                clients[i].alive = 1; // nowy klient jest "żywy"
                break;
            }
        }
        pthread_mutex_unlock(&clients_mutex);

        // Jeśli brak miejsca, odrzucamy klienta
        if (i == MAX_CLIENTS) {
            printf("Max clients reached.\n");
            close(client_fd);
        } else {
            // Tworzymy nowy wątek do obsługi klienta
            pthread_t tid;
            pthread_create(&tid, NULL, handle_client, &clients[i]);
        }
    }

    return 0;
}

// Funkcja obsługująca pojedynczego klienta (każdy klient ma swój wątek)
void *handle_client(void *arg) {
    Client *cli = (Client*)arg;
    char buffer[BUF_SIZE];

    // Odbiór pierwszej wiadomości - identyfikatora klienta
    recv(cli->sockfd, cli->id, ID_SIZE, 0);
    printf("Client connected: %s\n", cli->id);

    // Główna pętla odbierająca polecenia od klienta
    while (1) {
        int n = recv(cli->sockfd, buffer, sizeof(buffer), 0);
        if (n <= 0) break; // klient się rozłączył

        buffer[n] = '\0';
        cli->alive = 1; // klient odpowiedział, więc jest "żywy"

        // Obsługa poleceń klienta
        if (strncmp(buffer, "LIST", 4) == 0) {
            // Klient prosi o listę aktywnych klientów
            list_clients(cli->sockfd);
        } else if (strncmp(buffer, "2ALL", 4) == 0) {
            // Klient wysyła wiadomość do wszystkich
            char msg[BUF_SIZE], timebuf[64];
            current_time(timebuf); // pobierz aktualny czas
            // Format: FROM <nadawca> <data> <wiadomość>
            snprintf(msg, sizeof(msg), "FROM %s %s %s", cli->id, timebuf, buffer + 5);
            broadcast(msg, cli->sockfd); // wyślij do wszystkich poza nadawcą
        } else if (strncmp(buffer, "2ONE", 4) == 0) {
            // Klient wysyła wiadomość do jednego klienta
            char target_id[ID_SIZE], *p = strchr(buffer + 5, ' ');
            if (p) {
                *p = '\0'; // oddziel ID od wiadomości
                strcpy(target_id, buffer + 5); // pobierz ID odbiorcy
                send_to_one(target_id, p + 1); // wyślij wiadomość
            }
        } else if (strncmp(buffer, "STOP", 4) == 0) {
            // Klient kończy pracę
            break;
        } else if (strncmp(buffer, "IM_ALIVE", 8) == 0) {
            // Odpowiedź klienta na ping
            cli->alive = 1;
        }
        // Inne polecenia można dodać tutaj
    }

    printf("Client %s disconnected.\n", cli->id);
    remove_client(cli->sockfd); // usuń klienta z tablicy
    close(cli->sockfd);         // zamknij socket
    return NULL;
}

// Wysyła wiadomość do wszystkich klientów poza nadawcą (except_fd)
void broadcast(const char *msg, int except_fd) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].sockfd != 0 && clients[i].sockfd != except_fd) {
            send(clients[i].sockfd, msg, strlen(msg), 0);
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

// Wysyła wiadomość do jednego klienta o podanym ID
void send_to_one(const char *id, const char *msg) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].sockfd != 0 && strcmp(clients[i].id, id) == 0) {
            char buffer[BUF_SIZE], timebuf[64];
            current_time(timebuf);
            // Format: FROM <nadawca> <data> <wiadomość>
            snprintf(buffer, sizeof(buffer), "FROM %s %s %s", id, timebuf, msg);
            send(clients[i].sockfd, buffer, strlen(buffer), 0);
            break;
        }
    }
    pthread_mutex_unlock(&clients_mutex);
}

// Wysyła listę aktywnych klientów do wskazanego klienta
void list_clients(int client_fd) {
    pthread_mutex_lock(&clients_mutex);
    char buffer[BUF_SIZE] = "ACTIVE CLIENTS:\n";
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].sockfd != 0) {
            strcat(buffer, clients[i].id);
            strcat(buffer, "\n");
        }
    }
    send(client_fd, buffer, strlen(buffer), 0);
    pthread_mutex_unlock(&clients_mutex);
}

// Usuwa klienta z tablicy na podstawie deskryptora socketu
void remove_client(int client_fd) {
    pthread_mutex_lock(&clients_mutex);
    for (int i = 0; i < MAX_CLIENTS; i++) {
        if (clients[i].sockfd == client_fd) {
            clients[i].sockfd = 0;
            clients[i].id[0] = '\0';
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
            if (clients[i].sockfd != 0) {
                if (clients[i].alive) {
                    // Jeśli klient był "żywy", ustawiamy alive na 0 i wysyłamy ping
                    clients[i].alive = 0;
                    send(clients[i].sockfd, "ALIVE?", 6, 0); // wysyłamy ping
                } else {
                    // Jeśli klient nie odpowiedział na poprzedni ping, usuwamy go
                    printf("Removing inactive client: %s\n", clients[i].id);
                    close(clients[i].sockfd);
                    clients[i].sockfd = 0;
                    clients[i].id[0] = '\0';
                }
            }
        }
        pthread_mutex_unlock(&clients_mutex);
    }
}

// Zwraca aktualny czas jako string w formacie YYYY-MM-DD HH:MM:SS
void current_time(char *buffer) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    strftime(buffer, 64, "%Y-%m-%d %H:%M:%S", t);
}
