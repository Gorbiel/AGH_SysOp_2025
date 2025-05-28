#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
    if (argc != 2) // Sprawdzenie, czy podano dokładnie jeden argument
    {
        printf("Usage: %s <number_of_processes>\n", argv[0]);
        return 1;
    }

    int num_processes = atoi(argv[1]); // Konwersja argumentu na liczbę całkowitą
    if (num_processes <= 0) // Sprawdzenie, czy liczba procesów jest dodatnia
    {
        printf("Please provide a positive integer for the number of processes.\n");
        return 1;
    }

    for (int i = 0; i < num_processes; i++)
    {
        pid_t pid = fork(); // Tworzenie procesu potomnego
        if (pid < 0) // Sprawdzenie błędu podczas tworzenia procesu
        {
            perror("fork");
            return 1;
        }
        else if (pid == 0) // Kod wykonywany przez proces potomny
        {
            printf("Parent PID: %d, Child PID: %d\n", getppid(), getpid());
            return 0; // Proces potomny kończy działanie
        }
    }

    // Proces macierzysty czeka na zakończenie wszystkich procesów potomnych
    wait(NULL);

    // Proces macierzysty wypisuje liczbę procesów
    printf("%d\n", num_processes);

    return 0;
}