#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>
#include <signal.h>

#define MAX_MSG_SIZE 256
#define INIT 1
#define MESSAGE 2

// Struktura wiadomości przesyłanej przez kolejki komunikatów
typedef struct {
    long type;                  // Typ wiadomości (np. INIT lub MESSAGE)
    int client_id;              // ID klienta
    key_t client_queue_key;     // Klucz kolejki klienta
    char text[MAX_MSG_SIZE];    // Treść wiadomości
} message_t;

int client_queue_id;            // ID kolejki klienta
int client_id;                  // ID klienta nadany przez serwer

// Funkcja obsługująca odbieranie wiadomości od serwera
void handle_server_messages() {
    while (1) {
        message_t msg;
        // Odbieranie wiadomości z kolejki klienta
        if (msgrcv(client_queue_id, &msg, sizeof(message_t) - sizeof(long), 0, 0) == -1) {
            perror("msgrcv");
            exit(EXIT_FAILURE);
        }

        // Wyświetlanie wiadomości odebranej od serwera
        printf("Message from client %d: %s\n", msg.client_id, msg.text);
    }
}

// Funkcja czyszcząca kolejkę klienta po zakończeniu działania programu
void cleanup() {
    msgctl(client_queue_id, IPC_RMID, NULL); // Usunięcie kolejki klienta
    printf("Client cleanup done.\n");
}

int main(int argc, char *argv[]) {
    // Sprawdzanie poprawności argumentów wiersza poleceń
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <server_key>\n", argv[0]);
        exit(EXIT_FAILURE);
    }

    // Pobranie klucza serwera z argumentu wiersza poleceń
    key_t server_key = atoi(argv[1]);

    // Otwieranie kolejki serwera
    int server_queue_id = msgget(server_key, 0);
    if (server_queue_id == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Generowanie unikalnego klucza dla kolejki klienta na podstawie PID
    key_t client_key = getpid();
    client_queue_id = msgget(client_key, IPC_CREAT | 0666);
    if (client_queue_id == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    // Rejestracja funkcji cleanup, która zostanie wywołana przy zakończeniu programu
    atexit(cleanup);

    // Wysłanie wiadomości INIT do serwera z kluczem kolejki klienta
    message_t init_msg = { .type = INIT, .client_queue_key = client_key };
    if (msgsnd(server_queue_id, &init_msg, sizeof(message_t) - sizeof(long), 0) == -1) {
        perror("msgsnd");
        exit(EXIT_FAILURE);
    }

    // Oczekiwanie na odpowiedź od serwera z nadanym ID klienta
    message_t response;
    if (msgrcv(client_queue_id, &response, sizeof(message_t) - sizeof(long), INIT, 0) == -1) {
        perror("msgrcv");
        exit(EXIT_FAILURE);
    }

    client_id = response.client_id; // Przypisanie ID klienta
    printf("Connected to server. Client ID: %d\n", client_id);

    // Tworzenie procesu potomnego do odbierania wiadomości od serwera
    if (fork() == 0) {
        handle_server_messages();
        exit(0);
    }

    // Główna pętla do wysyłania wiadomości do serwera
    char input[MAX_MSG_SIZE];
    while (1) {
        fgets(input, MAX_MSG_SIZE, stdin);
        input[strcspn(input, "\n")] = '\0'; // Usunięcie znaku nowej linii

        // Tworzenie wiadomości do wysłania
        message_t msg = { .type = MESSAGE, .client_id = client_id };
        strncpy(msg.text, input, MAX_MSG_SIZE);

        // Wysłanie wiadomości do serwera
        if (msgsnd(server_queue_id, &msg, sizeof(message_t) - sizeof(long), 0) == -1) {
            perror("msgsnd");
        }
    }

    return 0;
}