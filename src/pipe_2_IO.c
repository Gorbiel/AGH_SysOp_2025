#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <errno.h>

#define TMP_DIR "./tmp"
#define PIPE_NAME_IO TMP_DIR "/pipe_IO_to_CALC"
#define PIPE_NAME_CALC TMP_DIR "/pipe_CALC_to_IO"

int main(int argc, char *argv[]) {
    if (argc != 4) {
        fprintf(stderr, "Usage: %s <start> <end> <width>\n", argv[0]);
        return 1;
    }

    double start = atof(argv[1]);
    double end = atof(argv[2]);
    double width = atof(argv[3]);

    if (start >= end || width <= 0) {
        fprintf(stderr, "Invalid input: start must be less than end, and width must be positive.\n");
        return 1;
    }

    // Tworzenie folderu ./tmp
    if (mkdir(TMP_DIR, 0777) == -1 && errno != EEXIST) {
        perror("mkdir ./tmp");
        return 1;
    }

    // Tworzenie potoków nazwanych
    if (mkfifo(PIPE_NAME_IO, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo pipe_IO_to_CALC");
        return 1;
    }
    if (mkfifo(PIPE_NAME_CALC, 0666) == -1 && errno != EEXIST) {
        perror("mkfifo pipe_CALC_to_IO");
        unlink(PIPE_NAME_IO); // Usuwanie pierwszego potoku w przypadku błędu
        return 1;
    }

    // Otwieranie potoku do zapisu
    int pipe_fd_io = open(PIPE_NAME_IO, O_WRONLY);
    if (pipe_fd_io == -1) {
        perror("open pipe_IO_to_CALC");
        return 1;
    }

    // Wysyłanie danych do programu drugiego
    double data[3] = {start, end, width};
    write(pipe_fd_io, data, sizeof(data));
    close(pipe_fd_io);

    // Otwieranie potoku do odczytu
    int pipe_fd_calc = open(PIPE_NAME_CALC, O_RDONLY);
    if (pipe_fd_calc == -1) {
        perror("open pipe_CALC_to_IO");
        return 1;
    }

    // Odbieranie wyniku od programu drugiego
    double result;
    read(pipe_fd_calc, &result, sizeof(result));
    close(pipe_fd_calc);

    // Wyświetlanie wyniku
    printf("Result of integral: %.10f\n", result);

    // Usuwanie potoków nazwanych
    unlink(PIPE_NAME_IO);
    unlink(PIPE_NAME_CALC);

    // Usuwanie folderu ./tmp
    if (rmdir(TMP_DIR) == -1) {
        perror("rmdir ./tmp");
    }

    return 0;
}