#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <signal.h>
#include <pthread.h>

#define BUF_SIZE 512

int sockfd; // deskryptor socketu klienta (globalny, by był widoczny w wątku)
struct sockaddr_in serv_addr; // adres serwera
socklen_t serv_len;           // długość struktury adresowej

// Wątek odbierający wiadomości od serwera
void *receive_handler(void *arg) {
    (void)arg;
    char buffer[BUF_SIZE];
    while (1) {
        // Odbieranie wiadomości z serwera (UDP)
        int n = recvfrom(sockfd, buffer, sizeof(buffer) - 1, 0,
                         (struct sockaddr*)&serv_addr, &serv_len);
        if (n <= 0) break; // serwer zamknął połączenie lub błąd
        buffer[n] = '\0';
        // Odpowiedź na ping serwera (ALIVE?)
        if (strcmp(buffer, "ALIVE?") == 0) {
            // Wysyłamy potwierdzenie, że klient żyje
            sendto(sockfd, "IM_ALIVE", 8, 0,
                   (struct sockaddr*)&serv_addr, serv_len);
        } else {
            // Wypisujemy każdą inną wiadomość od serwera
            printf("%s\n", buffer);
        }
    }
    exit(0); // zakończ program, jeśli serwer się rozłączy
}

// Obsługa sygnału Ctrl+C - wyrejestrowanie z serwera
void sigint_handler(int signo) {
    (void)signo;
    // Wysyłamy do serwera informację o zakończeniu pracy
    sendto(sockfd, "STOP", 4, 0,
           (struct sockaddr*)&serv_addr, serv_len);
    close(sockfd); // zamykamy socket
    exit(0);       // kończymy program
}

int main(int argc, char *argv[]) {
    if (argc != 4) {
        printf("Usage: %s <ID> <IP> <PORT>\n", argv[0]);
        return 1;
    }

    signal(SIGINT, sigint_handler); // obsługa Ctrl+C

    // Tworzenie socketu klienta (IPv4, UDP)
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0) { perror("socket"); return 1; }

    // Konfiguracja adresu serwera
    memset(&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = inet_addr(argv[2]);
    serv_addr.sin_port = htons(atoi(argv[3]));
    serv_len = sizeof(serv_addr);

    // Wysłanie identyfikatora do serwera (pierwsza wiadomość)
    sendto(sockfd, argv[1], strlen(argv[1]), 0,
           (struct sockaddr*)&serv_addr, serv_len);

    // Uruchomienie wątku odbierającego wiadomości od serwera
    pthread_t recv_thread;
    pthread_create(&recv_thread, NULL, receive_handler, NULL);

    // Główna pętla - wysyłanie poleceń wpisanych przez użytkownika
    char buffer[BUF_SIZE];
    while (fgets(buffer, BUF_SIZE, stdin)) {
        // Wysyłanie polecenia do serwera (np. LIST, 2ALL, 2ONE, STOP)
        sendto(sockfd, buffer, strlen(buffer), 0,
               (struct sockaddr*)&serv_addr, serv_len);
    }

    return 0;
}