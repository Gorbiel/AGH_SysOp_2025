#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>

// #define MAX_ITERATIONS 1000000 // Maksymalna liczba iteracji (można użyć do ograniczenia obliczeń)

// Struktura przechowująca dane dla każdego wątku
typedef struct {
    int thread_id;       // ID wątku
    int num_threads;     // Liczba wątków
    double width;        // Szerokość prostokąta
    double *results;     // Tablica wyników
    int *ready_flags;    // Tablica gotowości (czy wątek zakończył obliczenia)
} ThreadData;

// Funkcja obliczająca wartość funkcji f(x) = 4 / (x^2 + 1)
double function(double x) {
    return 4.0 / (x * x + 1);
}

// Funkcja wykonywana przez każdy wątek
void *calculate_integral(void *arg) {
    ThreadData *data = (ThreadData *)arg; // Rzutowanie argumentu na wskaźnik do ThreadData
    int thread_id = data->thread_id;     // ID wątku
    int num_threads = data->num_threads; // Liczba wątków
    double width = data->width;          // Szerokość prostokąta
    double *results = data->results;     // Tablica wyników
    int *ready_flags = data->ready_flags; // Tablica gotowości

    double local_sum = 0.0; // Lokalna suma obliczana przez wątek
    // int max_steps = fmin((int)(1.0 / width), MAX_ITERATIONS);
    int max_steps = (int)(1.0 / width); // Liczba kroków (prostokątów) do obliczenia

    // Każdy wątek oblicza swoją część całki
    for (int i = thread_id; i < max_steps; i += num_threads) {
        double x = i * width; // Oblicz współrzędną x dla prostokąta
        local_sum += function(x) * width; // Dodaj pole prostokąta do lokalnej sumy
    }

    results[thread_id] = local_sum; // Zapisz wynik w tablicy wyników
    ready_flags[thread_id] = 1;    // Oznacz wątek jako gotowy
    return NULL;                   // Zakończ działanie wątku
}

int main(int argc, char *argv[]) {
    // Sprawdzenie liczby argumentów
    if (argc != 3) {
        fprintf(stderr, "Usage: %s <rectangle_width> <num_threads>\n", argv[0]);
        return EXIT_FAILURE;
    }

    // Odczytanie argumentów
    double width = atof(argv[1]);  // Szerokość prostokąta
    int num_threads = atoi(argv[2]); // Liczba wątków

    // Sprawdzenie poprawności argumentów
    if (width <= 0 || num_threads <= 0) {
        fprintf(stderr, "Both rectangle width and number of threads must be positive.\n");
        return EXIT_FAILURE;
    }

    // Deklaracja tablic i wątków
    pthread_t threads[num_threads];       // Tablica identyfikatorów wątków
    ThreadData thread_data[num_threads];  // Tablica struktur danych dla wątków
    double results[num_threads];          // Tablica wyników
    int ready_flags[num_threads];         // Tablica gotowości

    // Inicjalizacja tablic wyników i gotowości
    for (int i = 0; i < num_threads; i++) {
        results[i] = 0.0;      // Ustawienie początkowego wyniku na 0
        ready_flags[i] = 0;    // Ustawienie flagi gotowości na 0 (niegotowy)
    }

    // Tworzenie wątków
    for (int i = 0; i < num_threads; i++) {
        thread_data[i].thread_id = i;        // ID wątku
        thread_data[i].num_threads = num_threads; // Liczba wątków
        thread_data[i].width = width;       // Szerokość prostokąta
        thread_data[i].results = results;   // Wskaźnik do tablicy wyników
        thread_data[i].ready_flags = ready_flags; // Wskaźnik do tablicy gotowości

        // Tworzenie wątku
        if (pthread_create(&threads[i], NULL, calculate_integral, &thread_data[i]) != 0) {
            perror("pthread_create"); // Obsługa błędu tworzenia wątku
            return EXIT_FAILURE;
        }
    }

    // Oczekiwanie na zakończenie wszystkich wątków
    for (int i = 0; i < num_threads; i++) {
        pthread_join(threads[i], NULL); // Czekaj na zakończenie wątku
    }

    // Sumowanie wyników z tablicy wyników
    double total_sum = 0.0;
    for (int i = 0; i < num_threads; i++) {
        total_sum += results[i]; // Dodaj wynik obliczony przez każdy wątek
    }

    // Wyświetlenie wyniku całki
    printf("Calculated integral: %.10f\n", total_sum);
    return EXIT_SUCCESS; // Zakończenie programu
}