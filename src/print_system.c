#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <sys/types.h>

#define MAX_QUEUE 10       // Maksymalny rozmiar kolejki zadań
#define NUM_USERS 6        // Liczba użytkowników
#define NUM_PRINTERS 2     // Liczba drukarek

// Indeksy semaforów
#define SEM_MUTEX 0        // Semafor do wzajemnego wykluczania przy modyfikacji kolejki
#define SEM_FULL 1         // Semafor liczący liczbę zadań w kolejce
#define SEM_EMPTY 2        // Semafor liczący wolne miejsca w kolejce
#define SEM_STDOUT 3       // Semafor do synchronizacji wypisywania na standardowe wyjście

// Struktura reprezentująca kolejkę zadań (bufor cykliczny)
typedef struct {
    char jobs[MAX_QUEUE][11]; // Tablica przechowująca zadania (każde zadanie to 10 znaków + '\0')
    int front, rear;   // Indeksy elementów w kolejce
} print_queue_t;

// Funkcja do wykonywania operacji na semaforach
void sem_op(int semid, int sem_num, int op) {
    struct sembuf sb = {sem_num, op, 0}; // Struktura opisująca operację na semaforze
    semop(semid, &sb, 1);               // Wykonanie operacji na semaforze
}

// Funkcja reprezentująca pętlę użytkownika
void user_loop(print_queue_t *queue, int semid) {
    srand(time(NULL) ^ getpid()); // Inicjalizacja generatora liczb pseudolosowych unikalnym ziarnem

    char job[11]; // Bufor na zadanie (10 znaków + '\0')
    while (1) {
        // Generowanie losowego zadania (ciągu 10 znaków)
        for (int i = 0; i < 10; ++i)
            job[i] = 'a' + rand() % 26;
        job[10] = '\0';

        // Czekanie na wolne miejsce w kolejce
        sem_op(semid, SEM_EMPTY, -1);
        // Wzajemne wykluczanie przy modyfikacji kolejki
        sem_op(semid, SEM_MUTEX, -1);

        // Dodanie zadania do kolejki
        strcpy(queue->jobs[queue->rear], job);
        queue->rear = (queue->rear + 1) % MAX_QUEUE;

        // Synchronizowane wypisywanie na standardowe wyjście
        sem_op(semid, SEM_STDOUT, -1);
        printf("Użytkownik dodał zadanie: %s\n", job);
        fflush(stdout);
        sem_op(semid, SEM_STDOUT, 1);

        // Zwolnienie dostępu do kolejki
        sem_op(semid, SEM_MUTEX, 1);
        // Zwiększenie licznika zadań w kolejce
        sem_op(semid, SEM_FULL, 1);

        // Odczekanie losowej liczby sekund przed wygenerowaniem kolejnego zadania
        sleep(1 + rand() % 5);
    }
}

// Funkcja reprezentująca pętlę drukarki
void printer_loop(print_queue_t *queue, int semid, int id) {
    while (1) {
        // Czekanie na dostępne zadanie w kolejce
        sem_op(semid, SEM_FULL, -1);
        // Wzajemne wykluczanie przy modyfikacji kolejki
        sem_op(semid, SEM_MUTEX, -1);

        // Pobranie zadania z kolejki
        char job[11];
        strcpy(job, queue->jobs[queue->front]);
        queue->front = (queue->front + 1) % MAX_QUEUE;

        // Zwolnienie dostępu do kolejki
        sem_op(semid, SEM_MUTEX, 1);
        // Zwiększenie licznika wolnych miejsc w kolejce
        sem_op(semid, SEM_EMPTY, 1);

        // Synchronizowane wypisywanie na standardowe wyjście
        sem_op(semid, SEM_STDOUT, -1);
        printf("Drukarka %d drukuje: ", id);
        fflush(stdout);

        // Symulacja drukowania (wypisywanie znaków z opóźnieniem 1 sekundy)
        for (int i = 0; i < 10; ++i) {
            printf("%c", job[i]);
            fflush(stdout);
            sleep(1);
        }
        printf("\n");
        fflush(stdout);
        sem_op(semid, SEM_STDOUT, 1);
    }
}

int main() {
    srand(time(NULL)); // Inicjalizacja generatora liczb pseudolosowych

    // Utworzenie pamięci wspólnej dla kolejki zadań
    int shmid = shmget(IPC_PRIVATE, sizeof(print_queue_t), IPC_CREAT | 0666);
    print_queue_t *queue = (print_queue_t *)shmat(shmid, NULL, 0);
    queue->front = queue->rear = 0; // Inicjalizacja kolejki

    // Utworzenie semaforów
    int semid = semget(IPC_PRIVATE, 4, IPC_CREAT | 0666); // 4 semafory
    semctl(semid, SEM_MUTEX, SETVAL, 1);  // Inicjalizacja semafora MUTEX
    semctl(semid, SEM_FULL, SETVAL, 0);   // Inicjalizacja semafora FULL
    semctl(semid, SEM_EMPTY, SETVAL, MAX_QUEUE); // Inicjalizacja semafora EMPTY
    semctl(semid, SEM_STDOUT, SETVAL, 1); // Inicjalizacja semafora STDOUT

    // Tworzenie procesów użytkowników
    for (int i = 0; i < NUM_USERS; ++i) {
        if (fork() == 0) {
            user_loop(queue, semid);
            exit(0);
        }
    }

    // Tworzenie procesów drukarek
    for (int i = 0; i < NUM_PRINTERS; ++i) {
        if (fork() == 0) {
            printer_loop(queue, semid, i);
            exit(0);
        }
    }

    // Oczekiwanie na zakończenie procesów potomnych
    for (int i = 0; i < NUM_USERS + NUM_PRINTERS; ++i)
        wait(NULL);

    // Usunięcie pamięci wspólnej i semaforów
    shmdt(queue);
    shmctl(shmid, IPC_RMID, NULL);
    semctl(semid, 0, IPC_RMID);

    return 0;
}
