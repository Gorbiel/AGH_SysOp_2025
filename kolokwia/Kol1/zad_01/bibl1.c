#include <stdlib.h>
#include <stdio.h>
#include "bibl1.h"

/*napisz biblioteke ladowana dynamicznie przez program zawierajaca funkcje:

1) zliczajaca sume n elementow tablicy tab:
int sumuj(int *tab, int n);

2) wyznaczajaca srednie n elementow tablicy tab
double srednia(int *tab, int n);

3) wyznaczajaca mediane n elementow tablicy tab
double mediana(int *tab, int n);

*/


// Function to calculate the sum of n elements in the array
int sumuj(int *tab, int n) {
    int sum = 0;
    for (int i = 0; i < n; i++) {
        sum += tab[i];
    }
    return sum;
}

// Function to calculate the average (mean) of n elements in the array
double srednia(int *tab, int n) {
    if (n == 0) return 0.0; // Avoid division by zero
    int sum = sumuj(tab, n);
    return (double)sum / n;
}

// Function to calculate the median of n elements in the array
double mediana(int *tab, int n) {
    if (n == 0) return 0.0; // Handle empty array case

    // Create a copy of the array to sort
    int *sorted = malloc(n * sizeof(int));
    if (!sorted) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < n; i++) {
        sorted[i] = tab[i];
    }

    // Sort the array
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (sorted[j] > sorted[j + 1]) {
                int temp = sorted[j];
                sorted[j] = sorted[j + 1];
                sorted[j + 1] = temp;
            }
        }
    }

    // Calculate the median
    double median;
    if (n % 2 == 0) {
        median = (sorted[n / 2 - 1] + sorted[n / 2]) / 2.0;
    } else {
        median = sorted[n / 2];
    }

    free(sorted);
    return median;
}