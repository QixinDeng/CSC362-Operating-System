#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <fcntl.h>
#include <unistd.h>

#define BUFFER_SIZE 5

static int buffer[BUFFER_SIZE];
static int in = 0;
static int out = 0;
static int count = 0;

static sem_t *empty;
static sem_t *filled;
static pthread_mutex_t mutex;

void* producer(void* arg) {
    for (int i = 0; i < 20; i++) {
        sem_wait(empty);
        pthread_mutex_lock(&mutex);

        buffer[in] = i;
        in = (in + 1) % BUFFER_SIZE;
        printf("Produced %d, count %d\n", i, ++count);

        pthread_mutex_unlock(&mutex);

        sem_post(filled);
        sleep(1); // Simulate work

    }
    return NULL;
}

void* consumer(void* arg) {
    for (int i = 0; i < 20; i++) {
        sem_wait(filled);
        pthread_mutex_lock(&mutex);

        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        printf("Consumed %d\n", item);
        count--;
        pthread_mutex_unlock(&mutex);

        sem_post(empty);
        sleep(2); // Simulate work

    }
    return NULL;
}

int semExample() {
    pthread_t prod, cons;

    char filled_sem[] = "/filled";
    char empty_sem[] = "/empty";

    sem_unlink(filled_sem);
    sem_unlink(empty_sem);

    // Initialize named semaphores
    filled = sem_open(filled_sem, O_TRUNC|O_CREAT, 0666, 0);
    empty = sem_open(empty_sem, O_TRUNC|O_CREAT, 0666, BUFFER_SIZE);

    pthread_mutex_init(&mutex, NULL);

    // Create producer and consumer threads
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    // Wait for threads to finish
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    // Cleanup
    pthread_mutex_destroy(&mutex);
    sem_close(empty);
    sem_close(filled);
    sem_unlink(filled_sem);
    sem_unlink(empty_sem);

    return 0;
}
