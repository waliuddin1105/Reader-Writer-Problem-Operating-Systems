#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <pthread.h>
#include <sys/syscall.h>
#include <stdint.h>
#include <string.h>
#include <sys/types.h>

struct read_info {
    int id;
    int time;
};

void *writer(void *i);
void *reader(void *i);

int wrand_int;
int avg_time = 0;

sem_t mutex;
sem_t rwmutex;
sem_t avgmutex;
int read_count = 0;

int main() {
    sem_init(&mutex, 0, 1);
    sem_init(&rwmutex, 0, 1);
    sem_init(&avgmutex, 0, 1);

    int nw, nr;
    FILE *inFile = fopen("input.txt", "r");
    if (!inFile) {
        perror("File open failed");
        return 1;
    }

    fscanf(inFile, "%d", &nr);
    fscanf(inFile, "%d", &nw);
    fclose(inFile);

    srand(time(0));

    pthread_t w[nw], r[nr];
    pthread_attr_t w_attr[nw], r_attr[nr];
    struct read_info read[nr];

    for (int i = 0; i < nw; ++i) {
        pthread_attr_init(&w_attr[i]);
        pthread_create(&w[i], &w_attr[i], writer, (void *)(intptr_t)(i + 1));
    }

    for (int i = 0; i < nr; ++i) {
        read[i].time = (rand() % 4) + 1;
        read[i].id = i + 1;
        pthread_attr_init(&r_attr[i]);
        pthread_create(&r[i], &r_attr[i], reader, &read[i]);
    }

    for (int i = 0; i < nw; i++)
        pthread_join(w[i], NULL);
    for (int i = 0; i < nr; i++)
        pthread_join(r[i], NULL);

    sem_destroy(&mutex);
    sem_destroy(&rwmutex);
    sem_destroy(&avgmutex);

    syscall(336, avg_time, nr, nw);
    printf("\nAvg time spent by readers and writers in critical section(sec): %d\n", (avg_time / ((nw + nr) * 1000)) / 1000);

    return 0;
}

