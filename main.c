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

void *writer(void *param) {
    int id = (intptr_t)param;

    struct timespec reqTime, enterTime, exitTime;
    clock_gettime(CLOCK_REALTIME, &reqTime);

    time_t my_time = time(NULL);
    struct tm *timeinfo = localtime(&my_time);
    printf("Request by Writer Thread %d at %02d:%02d\n", id, timeinfo->tm_min, timeinfo->tm_sec);

    sem_wait(&rwmutex);

    clock_gettime(CLOCK_REALTIME, &enterTime);
    my_time = time(NULL);
    timeinfo = localtime(&my_time);
    wrand_int = (rand() % 4) + 1;
    printf("\tEntry by Writer Thread %d at %02d:%02d | Will take t = %d sec to write\n", id, timeinfo->tm_min, timeinfo->tm_sec, wrand_int);

    sleep(wrand_int);

    clock_gettime(CLOCK_REALTIME, &exitTime);
    my_time = time(NULL);
    timeinfo = localtime(&my_time);
    printf("\t\tExit by Writer Thread %d at %02d:%02d\n", id, timeinfo->tm_min, timeinfo->tm_sec);

    sem_post(&rwmutex);

    sem_wait(&avgmutex);
    avg_time += ((enterTime.tv_sec - reqTime.tv_sec) * 1000000 + (enterTime.tv_nsec - reqTime.tv_nsec) / 1000);
    sem_post(&avgmutex);

    return NULL;
}

void *reader(void *param) {
    struct read_info *ptr = (struct read_info *)param;

    struct timespec reqTime, enterTime, exitTime;
    clock_gettime(CLOCK_REALTIME, &reqTime);

    time_t my_time = time(NULL);
    struct tm *timeinfo = localtime(&my_time);
    printf("Request by Reader Thread %d at %02d:%02d\n", ptr->id, timeinfo->tm_min, timeinfo->tm_sec);

    sem_wait(&mutex);
    read_count++;
    if (read_count == 1)
        sem_wait(&rwmutex);
    sem_post(&mutex);

    clock_gettime(CLOCK_REALTIME, &enterTime);
    my_time = time(NULL);
    timeinfo = localtime(&my_time);
    printf("\tEntry by Reader Thread %d at %02d:%02d | Will take t = %d sec to read\n", ptr->id, timeinfo->tm_min, timeinfo->tm_sec, ptr->time);

    sleep(ptr->time);

    sem_wait(&mutex);
    read_count--;
    if (read_count == 0)
        sem_post(&rwmutex);

    clock_gettime(CLOCK_REALTIME, &exitTime);
    my_time = time(NULL);
    timeinfo = localtime(&my_time);
    printf("\t\tExit by Reader Thread %d at %02d:%02d\n", ptr->id, timeinfo->tm_min, timeinfo->tm_sec);
    sem_post(&mutex);

    sem_wait(&avgmutex);
    avg_time += ((enterTime.tv_sec - reqTime.tv_sec) * 1000000 + (enterTime.tv_nsec - reqTime.tv_nsec) / 1000);
    sem_post(&avgmutex);

    return NULL;
}