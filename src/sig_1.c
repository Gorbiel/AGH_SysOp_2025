#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <signal.h> // Standardowy nagłówek POSIX

// Funkcja handlera sygnału SIGUSR1
void sigusr1_handler(int signum) {
    (void)signum; // Oznaczenie parametru jako nieużywanego
    printf("Otrzymano sygnał SIGUSR1\n");
}

int main(int argc, char *argv[]) {
    // Sprawdzenie liczby argumentów
    if (argc != 2) {
        fprintf(stderr, "Użycie: %s [none|ignore|handler|mask]\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Obsługa różnych opcji przekazanych jako argument
    if (strcmp(argv[1], "none") == 0) {
        // Brak zmiany reakcji na sygnał SIGUSR1
        printf("Brak zmiany reakcji na sygnał SIGUSR1\n");
    } else if (strcmp(argv[1], "ignore") == 0) {
        // Ignorowanie sygnału SIGUSR1
        if (signal(SIGUSR1, SIG_IGN) == SIG_ERR) {
            perror("Nie udało się ustawić ignorowania sygnału");
            return EXIT_FAILURE;
        }
        printf("Sygnał SIGUSR1 będzie ignorowany\n");
    } else if (strcmp(argv[1], "handler") == 0) {
        // Ustawienie handlera dla sygnału SIGUSR1
        if (signal(SIGUSR1, sigusr1_handler) == SIG_ERR) {
            perror("Nie udało się ustawić handlera sygnału");
            return EXIT_FAILURE;
        }
        printf("Zainstalowano handler dla sygnału SIGUSR1\n");
    } else if (strcmp(argv[1], "mask") == 0) {
        // Maskowanie sygnału SIGUSR1
        sigset_t set;
        sigemptyset(&set); // Inicjalizacja pustego zestawu sygnałów
        sigaddset(&set, SIGUSR1); // Dodanie SIGUSR1 do zestawu

        // Zablokowanie sygnału SIGUSR1
        if (sigprocmask(SIG_BLOCK, &set, NULL) == -1) {
            perror("Nie udało się zablokować sygnału");
            return EXIT_FAILURE;
        }
        printf("Sygnał SIGUSR1 został zablokowany\n");

        // Wysłanie sygnału SIGUSR1 do samego siebie
        raise(SIGUSR1);

        // Sprawdzenie, czy sygnał jest oczekujący
        sigset_t pending;
        sigpending(&pending); // Pobranie oczekujących sygnałów
        if (sigismember(&pending, SIGUSR1)) {
            printf("Sygnał SIGUSR1 jest oczekujący\n");
        } else {
            printf("Sygnał SIGUSR1 nie jest oczekujący\n");
        }
    } else {
        // Obsługa nieznanej opcji
        fprintf(stderr, "Nieznana opcja: %s\n", argv[1]);
        return EXIT_FAILURE;
    }

    // Wysłanie sygnału SIGUSR1 do samego siebie
    raise(SIGUSR1);

    // Pauza, aby umożliwić obsługę sygnału (jeśli nie jest zablokowany)
    // if (strcmp(argv[1], "mask") != 0) {
    //     pause();
    // }

    return EXIT_SUCCESS;
}