#include <stdio.h>

int collatz_conjecture(int input) {
    if (input <= 0) return -1; // Obsługa błędnego wejścia

    return (input % 2 == 0) ? input / 2 : 3 * input + 1;
}

int test_collatz_convergence(int input, int max_iter, int *steps) {
    if (input < 1 || max_iter <= 0) return 0; // Obsługa błędnego wejścia

    steps[0] = input;
    int i;

    for (i = 1; i < max_iter; i++) {
        steps[i] = collatz_conjecture(steps[i - 1]);

        if (steps[i] == 1) {
            return i + 1; // Liczba wykonanych kroków (indeks + 1)
        }
    }

    return 0; // Nie osiągnięto 1 w max_iter iteracjach
}
