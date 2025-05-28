#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

volatile sig_atomic_t confirmation_received = 0;

void sigusr1_handler(int sig) {
    (void)sig;
    confirmation_received = 1; // Otrzymano potwierdzenie
}

int main(int argc, char *argv[]) {
    // Sprawdzenie liczby argumentów wywołania programu
    if (argc != 3) {
        fprintf(stderr, "Użycie: %s <PID catcher> <tryb pracy>\n", argv[0]);
        return EXIT_FAILURE; // Zakończenie programu z błędem
    }

    // Pobranie PID procesu catcher z argumentów
    pid_t catcher_pid = atoi(argv[1]);
    // Pobranie trybu pracy z argumentów
    int mode = atoi(argv[2]);

    // Sprawdzenie poprawności trybu pracy (musi być w zakresie 1-5)
    if (mode < 1 || mode > 5) {
        fprintf(stderr, "[SENDER]  Nieprawidłowy tryb pracy: %d\n", mode);
        return EXIT_FAILURE; // Zakończenie programu z błędem
    }

    // Ustawienie handlera dla sygnału SIGUSR1 (potwierdzenie od catcher)
    struct sigaction sa;
    sa.sa_flags = 0; // Brak dodatkowych flag
    sa.sa_handler = sigusr1_handler; // Funkcja obsługująca sygnał SIGUSR1
    sigemptyset(&sa.sa_mask); // Wyczyszczenie maski sygnałów
    sigaction(SIGUSR1, &sa, NULL); // Zarejestrowanie handlera

    // Przygotowanie wartości sygnału SIGUSR1 (przekazanie trybu pracy)
    union sigval value;
    value.sival_int = mode;

    // Wysłanie sygnału SIGUSR1 do procesu catcher
    if (sigqueue(catcher_pid, SIGUSR1, value) == -1) {
        perror("[SENDER]  Nie udało się wysłać sygnału"); // Obsługa błędu
        return EXIT_FAILURE; // Zakończenie programu z błędem
    }

    // Oczekiwanie na potwierdzenie od catcher
    while (!confirmation_received) {
        pause(); // Zatrzymanie działania programu do momentu nadejścia sygnału
    }

    // Potwierdzenie zostało odebrane
    printf("[SENDER]  Potwierdzenie otrzymane. Kończenie pracy sender.\n");
    return EXIT_SUCCESS; // Zakończenie programu z sukcesem
}