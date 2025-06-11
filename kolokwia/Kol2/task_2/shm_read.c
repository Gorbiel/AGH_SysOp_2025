#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <string.h>
#include <errno.h>

#define PATH "/tmp"
#define BUFFER_SIZE 1024
#define ID 0

int main(int argc, char const *argv[])
{
    char *shmAddr;

    // Convert path and identifier to System V IPC key
    key_t key = ftok(PATH, ID);
    if (key == -1)
    {
        fprintf(stderr, "Failed to generate IPC key: %s\n", strerror(errno));
        exit(1);
    }

    // Access the shared memory segment
    int shmid;
    if ((shmid = shmget(key, BUFFER_SIZE, 0666)) == -1)
    {
        fprintf(stderr, "Failed to access shared memory: %s\n", strerror(errno));
        exit(1);
    }

    // Map shared memory segment to process address space
    shmAddr = shmat(shmid, NULL, 0);
    if (shmAddr == (void *)-1)
    {
        fprintf(stderr, "Failed to attach: %s\n", strerror(errno));
        exit(1);
    }

    printf("%s\n", shmAddr);

    // Detach shared memory
    if (shmdt(shmAddr) == -1)
    {
        fprintf(stderr, "Failed to detach: %s\n", strerror(errno));
    }

    // Remove shared memory segment
    if (shmctl(shmid, IPC_RMID, NULL) == -1)
    {
        fprintf(stderr, "Failed to remove shared memory: %s\n", strerror(errno));
    }

    return 0;
}

/*
gcc -o shm_read shm_read.c
*/
