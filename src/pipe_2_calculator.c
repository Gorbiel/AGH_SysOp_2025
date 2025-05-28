#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <math.h>

#define TMP_DIR "./tmp"
#define PIPE_NAME_IO TMP_DIR "/pipe_IO_to_CALC"
#define PIPE_NAME_CALC TMP_DIR "/pipe_CALC_to_IO"

// Funkcja do całkowania
double f(double x) {
    return 4.0 / (x * x + 1);
}

int main() {
    // Otwieranie potoku do odczytu
    int pipe_fd_io = open(PIPE_NAME_IO, O_RDONLY);
    if (pipe_fd_io == -1) {
        perror("open pipe_IO_to_CALC");
        return 1;
    }

    // Odczytywanie danych od programu pierwszego
    double data[3];
    read(pipe_fd_io, data, sizeof(data));
    close(pipe_fd_io);

    double start = data[0];
    double end = data[1];
    double width = data[2];

    // Obliczanie całki metodą prostokątów
    double result = 0.0;
    for (double x = start; x < end; x += width) {
        result += f(x) * width;
    }

    // Otwieranie potoku do zapisu
    int pipe_fd_calc = open(PIPE_NAME_CALC, O_WRONLY);
    if (pipe_fd_calc == -1) {
        perror("open pipe_CALC_to_IO");
        return 1;
    }

    // Wysyłanie wyniku do programu pierwszego
    write(pipe_fd_calc, &result, sizeof(result));
    close(pipe_fd_calc);

    return 0;
}