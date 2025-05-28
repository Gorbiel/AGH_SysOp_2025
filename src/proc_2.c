#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

// Deklaracja zmiennej globalnej
int global = 0;

int main(int argc, char *argv[]) {
    // Sprawdzenie liczby argumentów
    if (argc != 2) {
        fprintf(stderr, "Usage: %s <directory_path>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Zmienna lokalna
    int local = 0;

    // Wypisanie nazwy programu
    printf("Program name: %s\n", argv[0]);

    // Tworzenie procesu potomnego
    pid_t pid = fork();

    if (pid < 0) {
        // Obsługa błędu fork()
        perror("fork failed");
        return EXIT_FAILURE;
    } else if (pid == 0) {
        // Proces potomny
        printf("child process\n");

        // Inkrementacja zmiennych
        global++;
        local++;

        // Wyświetlenie informacji o procesie
        printf("child pid = %d, parent pid = %d\n", getpid(), getppid());
        printf("child's local = %d, child's global = %d\n", local, global);

        // Wykonanie programu /bin/ls
        execl("/bin/ls", "ls", argv[1], NULL);

        // Obsługa błędu execl()
        perror("execl failed");
        return EXIT_FAILURE;
    } else {
        // Proces rodzica
        printf("parent process\n");
        printf("parent pid = %d, child pid = %d\n", getpid(), pid);

        // Oczekiwanie na zakończenie procesu potomnego
        int status;
        if (waitpid(pid, &status, 0) == -1) {
            perror("waitpid failed");
            return EXIT_FAILURE;
        }

        // Wyświetlenie kodu zakończenia procesu potomnego
        if (WIFEXITED(status)) {
            printf("Child exit code: %d\n", WEXITSTATUS(status));
        } else {
            printf("Child did not terminate normally\n");
        }

        // Wyświetlenie wartości zmiennych w procesie rodzica
        printf("Parent's local = %d, parent's global = %d\n", local, global);
    }

    return EXIT_SUCCESS;
}