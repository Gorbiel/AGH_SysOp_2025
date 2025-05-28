#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <math.h>

// Funkcja do całkowania
double f(double x) {
    return 4.0 / (x * x + 1);
}

// Funkcja do obliczania czasu w milisekundach
long get_time_in_ms() {
    struct timeval tv;
    gettimeofday(&tv, NULL);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

int main(int argc, char *argv[]) {
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <rectangle_width> <max_processes>\n", argv[0]);
        return 1;
    }

    double width = atof(argv[1]);
    int max_processes = atoi(argv[2]);

    if (width <= 0 || max_processes <= 0) {
        fprintf(stderr, "Both rectangle width and max processes must be positive numbers.\n");
        return 1;
    }

    for (int k = 1; k <= max_processes; k++) {
        int pipes[k][2];
        pid_t pids[k];
        double range_per_process = 1.0 / k;
        long start_time = get_time_in_ms();

        // Tworzenie potoków i procesów potomnych
        for (int i = 0; i < k; i++) {
            if (pipe(pipes[i]) == -1) {
                perror("pipe");
                exit(1);
            }

            pids[i] = fork();
            if (pids[i] == -1) {
                perror("fork");
                exit(1);
            }

            if (pids[i] == 0) { // Proces potomny
                close(pipes[i][0]); // Zamykamy odczyt w potoku
                double local_sum = 0.0;
                double start = i * range_per_process;
                double end = start + range_per_process;

                for (double x = start; x < end; x += width) {
                    local_sum += f(x) * width;
                }

                write(pipes[i][1], &local_sum, sizeof(local_sum));
                close(pipes[i][1]); // Zamykamy zapis w potoku
                exit(0);
            } else { // Proces macierzysty
                close(pipes[i][1]); // Zamykamy zapis w potoku
            }
        }

        // Oczekiwanie na wyniki od procesów potomnych
        double total_sum = 0.0;
        for (int i = 0; i < k; i++) {
            double local_sum = 0.0;
            read(pipes[i][0], &local_sum, sizeof(local_sum));
            close(pipes[i][0]); // Zamykamy odczyt w potoku
            total_sum += local_sum;
        }

        // Oczekiwanie na zakończenie procesów potomnych
        for (int i = 0; i < k; i++) {
            waitpid(pids[i], NULL, 0);
        }

        long end_time = get_time_in_ms();
        printf("k = %d, Result = %.10f, Time = %ld ms\n", k, total_sum, end_time - start_time);
    }

    return 0;
}