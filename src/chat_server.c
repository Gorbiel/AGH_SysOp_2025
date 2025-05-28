#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <unistd.h>

#define MAX_CLIENTS 10
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

// Struktura przechowująca informacje o kliencie
typedef struct {
    int client_id;              // ID klienta
    int queue_id;               // ID kolejki klienta
} client_t;

client_t clients[MAX_CLIENTS];  // Tablica klientów
int client_count = 0;           // Liczba podłączonych klientów

int main() {
    // Generowanie klucza serwera na podstawie PID
    key_t server_key = getpid();
    printf("Server started with PID (server_key): %d\n", server_key);

    // Tworzenie kolejki serwera
    int server_queue_id = msgget(server_key, IPC_CREAT | 0666);
    if (server_queue_id == -1) {
        perror("msgget");
        exit(EXIT_FAILURE);
    }

    printf("Server queue ID: %d\n", server_queue_id);

    while (1) {
        message_t msg;
        // Odbieranie wiadomości z kolejki serwera
        if (msgrcv(server_queue_id, &msg, sizeof(message_t) - sizeof(long), 0, 0) == -1) {
            perror("msgrcv");
            continue;
        }

        if (msg.type == INIT) {
            // Obsługa wiadomości INIT (rejestracja nowego klienta)
            if (client_count >= MAX_CLIENTS) {
                printf("Maximum number of clients reached.\n");
                continue;
            }

            // Otwieranie kolejki klienta na podstawie przesłanego klucza
            int client_queue_id = msgget(msg.client_queue_key, 0);
            if (client_queue_id == -1) {
                perror("msgget (client)");
                continue;
            }

            // Rejestracja klienta w tablicy
            clients[client_count].client_id = client_count + 1;
            clients[client_count].queue_id = client_queue_id;
            client_count++;

            // Wysłanie odpowiedzi do klienta z nadanym ID
            message_t response = { .type = INIT, .client_id = client_count };
            if (msgsnd(client_queue_id, &response, sizeof(message_t) - sizeof(long), 0) == -1) {
                perror("msgsnd");
            }

            printf("Client %d connected.\n", client_count);
        } else if (msg.type == MESSAGE) {
            // Obsługa wiadomości MESSAGE (przekazywanie wiadomości do innych klientów)
            printf("Message from client %d: %s\n", msg.client_id, msg.text);

            // Wysłanie wiadomości do wszystkich innych klientów
            for (int i = 0; i < client_count; i++) {
                if (clients[i].client_id != msg.client_id) {
                    if (msgsnd(clients[i].queue_id, &msg, sizeof(message_t) - sizeof(long), 0) == -1) {
                        perror("msgsnd");
                    }
                }
            }
        }
    }

    return 0;
}