#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>

#define BUF_SIZE 512

int sockfd; // deskryptor socketu klienta (globalny, by był widoczny w wątku)

// Wątek odbierający wiadomości od serwera
void *receive_handler(void *arg) {
    (void)arg;
    char buffer[BUF_SIZE];
    while (1) {
        // Odbieranie wiadomości z serwera
        int n = recv(sockfd, buffer, sizeof(buffer) - 1, 0);
        if (n <= 0) break; // serwer zamknął połączenie
        buffer[n] = '\0';
        // Odpowiedź na ping serwera (ALIVE?)
        if (strcmp(buffer, "ALIVE?") == 0) {
            send(sockfd, "IM_ALIVE", 8, 0); // potwierdzenie, że klient żyje
        } else {
            printf("%s\n", buffer); // wypisz każdą inną wiadomość
        }
    }
    exit(0); // zakończ program, jeśli serwer się rozłączy
}

// Obsługa sygnału Ctrl+C - wyrejestrowanie z serwera
void sigint_handler(int signo) {
    (void)signo;
    send(sockfd, "STOP", 4, 0); // powiadom serwer o zakończeniu pracy
    close(sockfd);              // zamknij socket
    exit(0);                    // zakończ program
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <ID> <IP> <PORT>\n", argv[0]);
        return 1;
    }

    signal(SIGINT, sigint_handler); // obsługa Ctrl+C

    // Tworzenie socketu klienta (IPv4, TCP)
    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in serv_addr = {0};
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[2]); // adres IP serwera
    serv_addr.sin_port = htons(atoi(argv[3]));      // port serwera

    // Połączenie z serwerem
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0) {
        perror("connect");
        return 1;
    }

    // Wysłanie identyfikatora do serwera (pierwsza wiadomość)
    send(sockfd, argv[1], strlen(argv[1]), 0);

    // Uruchomienie wątku odbierającego wiadomości od serwera
    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receive_handler, NULL);

    // Główna pętla - wysyłanie poleceń wpisanych przez użytkownika
    char buffer[BUF_SIZE];
    while (fgets(buffer, BUF_SIZE, stdin)) {
        // Wysyłanie polecenia do serwera (np. LIST, 2ALL, 2ONE, STOP)
        send(sockfd, buffer, strlen(buffer), 0);
    }

    return 0;
}
