#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <time.h>

// Stałe określające limity systemu szpitalnego
#define MAX_WAITING_PATIENTS 3           // Maksymalna liczba pacjentów w poczekalni
#define MEDICINE_CAPACITY 6              // Maksymalna pojemność apteczki
#define MEDICINE_PER_CONSULTATION 3      // Liczba leków zużywana na jedną konsultację

// Liczba pacjentów i farmaceutów (ustawiana z linii poleceń)
int num_patients;
int num_pharmacists;

// Zmienne współdzielone do synchronizacji
int waiting_patients = 0;                // Liczba pacjentów oczekujących w poczekalni
int medicine_stock = 6;                  // Aktualna liczba leków w apteczce

// int total_patients_consulted = 0;        // liczba obsłużonych pacjentów

// Mutex i zmienne warunkowe do synchronizacji wątków
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t doctor_wakeup = PTHREAD_COND_INITIALIZER;
pthread_cond_t pharmacy_not_full = PTHREAD_COND_INITIALIZER;
pthread_cond_t pharmacy_needs_refill = PTHREAD_COND_INITIALIZER;

// Kolejka pacjentów oczekujących na konsultację
int* patient_queue;                      // Tablica przechowująca ID pacjentów w poczekalni
int queue_index = 0;                     // Indeks do kolejki pacjentów

int patients_remaining;                  // Liczba pacjentów, którzy jeszcze nie byli obsłużeni

// Funkcja pomocnicza do wypisywania aktualnego czasu
void print_time() {
    time_t now = time(NULL);
    struct tm* t = localtime(&now);
    printf("[%02d:%02d:%02d] - ", t->tm_hour, t->tm_min, t->tm_sec);
}

// Funkcja wątku pacjenta
void* patient_thread(void* arg) {
    int id = *((int*)arg);
    while (1) {
        // Pacjent przychodzi do szpitala po losowym czasie 2-5 sekund
        int walk_time = rand() % 4 + 2;
        print_time();
        printf("Pacjent(%d): Ide do szpitala, bede za %d s.\n", id, walk_time);
        sleep(walk_time);

        pthread_mutex_lock(&mutex);
        // Jeśli poczekalnia pełna, pacjent idzie na spacer i wraca później
        if (waiting_patients >= MAX_WAITING_PATIENTS) {
            pthread_mutex_unlock(&mutex);
            int retry_time = rand() % 3 + 1;
            print_time();
            printf("Pacjent(%d): za dużo pacjentów, wracam później za %d s.\n", id, retry_time);
            sleep(retry_time);
            continue;
        }

        // Pacjent siada w poczekalni (dodaje się do kolejki)
        patient_queue[queue_index++] = id;
        waiting_patients++;
        print_time();
        printf("Pacjent(%d): czeka %d pacjentów na lekarza.\n", id, waiting_patients);

        // Jeśli jest trzecim pacjentem, budzi lekarza
        if (waiting_patients == MAX_WAITING_PATIENTS) {
            print_time();
            printf("Pacjent(%d): budzę lekarza.\n", id);
            pthread_cond_signal(&doctor_wakeup);
        }

        pthread_mutex_unlock(&mutex);
        break; // Pacjent czeka tylko raz na wejście do poczekalni
    }

    pthread_exit(NULL);
}

// Funkcja wątku farmaceuty
void* pharmacist_thread(void* arg) {
    int id = *((int*)arg);
    int wait_time = rand() % 11 + 5;

    // Farmaceuta przybywa do szpitala po losowym czasie 5-15 sekund
    print_time();
    printf("Farmaceuta(%d): ide do szpitala, bede za %d s.\n", id, wait_time);
    sleep(wait_time);

    pthread_mutex_lock(&mutex);

    // Jeśli apteczka jest pełna, farmaceuta czeka na jej opróżnienie
    while (medicine_stock == MEDICINE_CAPACITY) {
        print_time();
        printf("Farmaceuta(%d): czekam na oproznienie apteczki.\n", id);
        pthread_cond_wait(&pharmacy_not_full, &mutex);
    }

    // Jeśli apteczka jest pusta lub kończą się leki, farmaceuta budzi lekarza
    if (medicine_stock < MEDICINE_PER_CONSULTATION) {
        print_time();
        printf("Farmaceuta(%d): budzę lekarza.\n", id);
        pthread_cond_signal(&doctor_wakeup);
    }

    // Farmaceuta informuje o dostawie leków
    print_time();
    printf("Farmaceuta(%d): dostarczam leki.\n", id);

    pthread_mutex_unlock(&mutex);

    // Farmaceuta informuje o zakończeniu dostawy i kończy pracę
    print_time();
    printf("Farmaceuta(%d): zakończyłem dostawę.\n", id);

    pthread_exit(NULL);
}

// Funkcja wątku lekarza
void* doctor_thread(void* /*arg*/) {

    while (1) {
        pthread_mutex_lock(&mutex);

        // Lekarz śpi dopóki nie zbierze się 3 pacjentów i są leki lub apteczka wymaga uzupełnienia
        while (!((waiting_patients == MAX_WAITING_PATIENTS && medicine_stock >= MEDICINE_PER_CONSULTATION) ||
                 (medicine_stock < MEDICINE_PER_CONSULTATION))) {
            pthread_cond_wait(&doctor_wakeup, &mutex);
        }

        // Lekarz budzi się, bo spełniony został jeden z warunków
        print_time();
        printf("Lekarz: budzę się.\n");

        // Jeśli są 3 pacjenci i wystarczająca ilość leków
        if (waiting_patients == MAX_WAITING_PATIENTS && medicine_stock >= MEDICINE_PER_CONSULTATION) {
            int ids[3];
            // Pobierz identyfikatory pacjentów z kolejki
            for (int i = 0; i < 3; ++i) {
                ids[i] = patient_queue[i];
            }

            // Lekarz konsultuje pacjentów i zużywa leki
            print_time();
            printf("Lekarz: konsultuję pacjentów %d, %d, %d.\n", ids[0], ids[1], ids[2]);
            medicine_stock -= MEDICINE_PER_CONSULTATION;

            // Resetuje kolejkę i liczbę oczekujących pacjentów
            queue_index = 0;
            waiting_patients = 0;
            patients_remaining -= 3;

            // Sygnał dla farmaceutów, że apteczka nie jest pełna (mogą uzupełniać)
            pthread_cond_signal(&pharmacy_not_full);

            pthread_mutex_unlock(&mutex);

            // Konsultacja trwa 2-4 sekundy (symulacja pracy lekarza)
            sleep(rand() % 3 + 2);

            // Pacjenci kończą wizytę (komunikaty)
            for (int i = 0; i < 3; ++i) {
                print_time();
                printf("Pacjent(%d): kończę wizytę.\n", ids[i]);
            }
        } else if (medicine_stock < MEDICINE_PER_CONSULTATION) {
            // Lekarz przyjmuje dostawę leków, bo apteczka się kończy
            print_time();
            printf("Lekarz: przyjmuję dostawę leków.\n");
            // Dostawa trwa 1-3 sekundy (symulacja czasu uzupełniania)
            sleep(rand() % 3 + 1);
            medicine_stock = MEDICINE_CAPACITY;
            print_time();
            printf("Lekarz: apteczka uzupełniona do %d.\n", medicine_stock);
        }

        // Lekarz zasypia po wykonaniu zadania
        print_time();
        printf("Lekarz: zasypiam.\n");

        // Jeśli nie ma już pacjentów do obsłużenia, lekarz kończy pracę
        if (patients_remaining <= 0) {
            pthread_mutex_unlock(&mutex);
            break;
        }

        pthread_mutex_unlock(&mutex);
    }

    pthread_exit(NULL);
}

int main(int argc, char* argv[]) {
    // Sprawdzenie poprawności argumentów wywołania programu
    if (argc != 3) {
        printf("Użycie: %s <liczba_pacjentów> <liczba_farmaceutów>\n", argv[0]);
        exit(1);
    }

    srand(time(NULL)); // Inicjalizacja generatora liczb losowych

    // Pobranie liczby pacjentów i farmaceutów z argumentów
    num_patients = atoi(argv[1]);
    num_pharmacists = atoi(argv[2]);
    patients_remaining = num_patients;

    // Alokacja pamięci na wątki i identyfikatory
    pthread_t doctor;
    pthread_t* patients = malloc(sizeof(pthread_t) * num_patients);         // Tablica wątków pacjentów
    pthread_t* pharmacists = malloc(sizeof(pthread_t) * num_pharmacists);   // Tablica wątków farmaceutów
    int* ids = malloc(sizeof(int) * (num_patients + num_pharmacists));      // Tablica identyfikatorów

    patient_queue = malloc(sizeof(int) * MAX_WAITING_PATIENTS);             // Kolejka pacjentów

    // Tworzenie wątku lekarza
    pthread_create(&doctor, NULL, doctor_thread, NULL);

    // Tworzenie wątków pacjentów
    for (int i = 0; i < num_patients; i++) {
        ids[i] = i + 1; // Każdy pacjent dostaje unikalny ID
        pthread_create(&patients[i], NULL, patient_thread, &ids[i]);
        usleep(100000); // Mikro opóźnienie dla unikalnych ID (uniknięcie race condition)
    }

    // Tworzenie wątków farmaceutów
    for (int i = 0; i < num_pharmacists; i++) {
        ids[num_patients + i] = i + 1; // Każdy farmaceuta dostaje unikalny ID
        pthread_create(&pharmacists[i], NULL, pharmacist_thread, &ids[num_patients + i]);
        usleep(100000); // Mikro opóźnienie dla unikalnych ID
    }

    // Oczekiwanie na zakończenie wszystkich wątków pacjentów
    for (int i = 0; i < num_patients; i++) {
        pthread_join(patients[i], NULL);
    }

    // Oczekiwanie na zakończenie wątku lekarza
    pthread_join(doctor, NULL);

    // Zwolnienie zaalokowanej pamięci
    free(patients);
    free(pharmacists);
    free(ids);
    free(patient_queue);

    return 0;
}
