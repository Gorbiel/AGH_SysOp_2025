#include <stdio.h>
#include <stdlib.h>

#ifdef DYNAMIC_LOAD
#include <dlfcn.h>
#endif

#define MAX_ITER 100

#ifndef DYNAMIC_LOAD
#include "collatz.h"
#endif

int main() {
    int inputs[] = {6, 11, 19, 27, 50};
    int num_inputs = sizeof(inputs) / sizeof(inputs[0]);

#ifdef DYNAMIC_LOAD
    // Ładowanie biblioteki w czasie działania
    void *handle = dlopen("./libcollatz.so.0.0.1", RTLD_LAZY);
    if (!handle) {
        fprintf(stderr, "Błąd ładowania biblioteki: %s\n", dlerror());
        return 1;
    }

    int (*collatz_conjecture)(int) = dlsym(handle, "collatz_conjecture");
    int (*test_collatz_convergence)(int, int, int *) = dlsym(handle, "test_collatz_convergence");

    if (!collatz_conjecture || !test_collatz_convergence) {
        fprintf(stderr, "Błąd pobierania funkcji: %s\n", dlerror());
        dlclose(handle);
        return 1;
    }
#endif

    for (int i = 0; i < num_inputs; i++) {
        int steps[MAX_ITER];
        int num_steps = test_collatz_convergence(inputs[i], MAX_ITER, steps);

        printf("Liczba: %d\n", inputs[i]);

        if (num_steps > 0) {
            printf("Zbiegła do 1 w %d krokach: ", num_steps);
            for (int j = 0; j < num_steps; j++) {
                printf("%d ", steps[j]);
            }
            printf("\n");
        } else {
            printf("Nie udało się zbiec do 1 w %d iteracjach.\n", MAX_ITER);
        }
        printf("\n");
    }

#ifdef DYNAMIC_LOAD
    dlclose(handle);
#endif

    return 0;
}
