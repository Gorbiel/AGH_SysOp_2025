#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>

volatile sig_atomic_t mode = 1; // Aktualny tryb pracy
volatile sig_atomic_t received_signals = 0; // Liczba otrzymanych sygnałów
volatile pid_t sender_pid = 0; // PID sendera

void sigusr1_handler(int sig, siginfo_t *info, void *context) {
    (void)sig;
    (void)context;

    sender_pid = info->si_pid; // Pobranie PID sendera
    mode = info->si_value.sival_int; // Pobranie trybu pracy
    received_signals++; // Zwiększenie liczby odebranych sygnałów

    // Wysłanie potwierdzenia do sendera
    union sigval value = {0};
    sigqueue(sender_pid, SIGUSR1, value);
}

void sigint_handler(int sig) {
    (void)sig; // Oznaczenie parametru jako nieużywanego - zapobiega ostrzeżeniom kompilatora
    if (mode == 3) {
        printf("[CATCHER] Ignorowanie Ctrl+C\n");
    } else if (mode == 4) {
        printf("[CATCHER] Wciśnięto CTRL+C\n");
    } else {
        exit(0);
    }
}

int main() {
    // Konfiguracja obsługi sygnału SIGUSR1
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO; // Ustawienie flagi, aby handler mógł odbierać dodatkowe informacje o sygnale
    sa.sa_sigaction = sigusr1_handler; // Ustawienie funkcji obsługującej sygnał SIGUSR1
    sigemptyset(&sa.sa_mask); // Wyczyszczenie maski sygnałów (brak blokowania innych sygnałów podczas obsługi)
    sigaction(SIGUSR1, &sa, NULL); // Zarejestrowanie handlera dla SIGUSR1

    // Konfiguracja obsługi sygnału SIGINT (Ctrl+C)
    struct sigaction sa_int;
    sa_int.sa_flags = 0; // Brak dodatkowych flag
    sa_int.sa_handler = sigint_handler; // Ustawienie funkcji obsługującej sygnał SIGINT
    sigemptyset(&sa_int.sa_mask); // Wyczyszczenie maski sygnałów
    sigaction(SIGINT, &sa_int, NULL); // Zarejestrowanie handlera dla SIGINT

    // Wyświetlenie PID procesu catcher, aby sender mógł go znać
    printf("[CATCHER] Catcher PID: %d\n", getpid());

    // Główna pętla programu
    while (1) {
        if (mode == 1) {
            // Tryb 1: Wyświetlanie liczby odebranych sygnałów SIGUSR1
            printf("[CATCHER] Otrzymano %d żądań zmiany trybu pracy\n", received_signals);
        } else if (mode == 2) {
            // Tryb 2: Wyświetlanie liczb w odstępach 1-sekundowych
            for (int i = 1; mode == 2; i++) {
                printf("[CATCHER] %d\n", i);
                sleep(1); // Pauza na 1 sekundę
            }
        } else if (mode == 5) {
            // Tryb 5: Zakończenie działania programu
            printf("[CATCHER] Zakończenie działania catcher\n");
            exit(0); // Wyjście z programu
        }

        // Oczekiwanie na sygnały (blokuje działanie programu do momentu ich nadejścia)
        pause();
    }

    return 0; // Zakończenie programu (teoretycznie nigdy nie zostanie osiągnięte)
}