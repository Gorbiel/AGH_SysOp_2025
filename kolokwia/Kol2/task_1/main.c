#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <stdint.h>
#include <sys/ipc.h>
#include <sys/sem.h>
#include <sys/wait.h>
#include <time.h>

#ifdef USE_POSIX
#include <semaphore.h>
#include <fcntl.h> // For O_* constants
#endif

#define FILE_NAME "common.txt"
#define SEM_NAME "/kol_sem" // For POSIX semaphore

int main(int argc, char** args) {
    if(argc != 4) {
        printf("Not a suitable number of program parameters\n");
        return 1;
    }

    int fd = open(FILE_NAME, O_WRONLY | O_CREAT | O_TRUNC , 0644);
    if (fd < 0) {
        perror("open");
        return 1;
    }

    int parentLoopCounter = atoi(args[1]);
    int childLoopCounter = atoi(args[2]);
    int max_sleep_time = atoi(args[3]);

    char buf[128];
    pid_t childPid;

#ifdef USE_POSIX
    sem_t *sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0644, 1);
    if (sem == SEM_FAILED) {
        perror("sem_open");
        return 1;
    }
#else
    key_t key = ftok(FILE_NAME, 'S');
    if (key == -1) {
        perror("ftok");
        return 1;
    }

    int semid = semget(key, 1, IPC_CREAT | 0666);
    if (semid == -1) {
        perror("semget");
        return 1;
    }

    if (semctl(semid, 0, SETVAL, 1) == -1) {
        perror("semctl");
        return 1;
    }
#endif

    if ((childPid = fork())) {
        int status = 0;
        srand((unsigned)time(0));

        while (parentLoopCounter--) {
            int s = rand() % max_sleep_time + 1;
            sleep(s);

#ifdef USE_POSIX
            sem_wait(sem); // Enter critical section
#else
            struct sembuf sb = {0, -1, 0};
            semop(semid, &sb, 1); // Enter critical section
#endif

            snprintf(buf, sizeof(buf), "Wpis rodzica. Petla %d. Spalem %d\n", parentLoopCounter, s);
            write(fd, buf, strlen(buf));
            write(1, buf, strlen(buf));

#ifdef USE_POSIX
            sem_post(sem); // Exit critical section
#else
            sb.sem_op = 1;
            semop(semid, &sb, 1); // Exit critical section
#endif
        }

        waitpid(childPid, &status, 0);

    } else {
        srand((unsigned)time(0));

        while (childLoopCounter--) {
            int s = rand() % max_sleep_time + 1;
            sleep(s);

#ifdef USE_POSIX
            sem_wait(sem); // Enter critical section
#else
            struct sembuf sb = {0, -1, 0};
            semop(semid, &sb, 1); // Enter critical section
#endif

            snprintf(buf, sizeof(buf), "Wpis dziecka. Petla %d. Spalem %d\n", childLoopCounter, s);
            write(fd, buf, strlen(buf));
            write(1, buf, strlen(buf));

#ifdef USE_POSIX
            sem_post(sem); // Exit critical section
#else
            sb.sem_op = 1;
            semop(semid, &sb, 1); // Exit critical section
#endif
        }

        _exit(0);
    }

#ifdef USE_POSIX
    sem_close(sem);
    sem_unlink(SEM_NAME); // Cleanup POSIX semaphore
#else
    semctl(semid, 0, IPC_RMID); // Cleanup System V semaphore
#endif

    close(fd);
    return 0;
}
