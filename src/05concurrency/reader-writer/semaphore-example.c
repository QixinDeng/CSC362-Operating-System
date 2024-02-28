/*
    Name: Dr. Qixin Deng
    Date: February 28, 2024
    Description:
    The code is a C program demonstrating a readers-writers solution using semaphores.
    It defines a shared data variable and synchronizes access to it using two semaphores:
    mutex for reader access control and writeBlock to manage writer access. The program
    creates multiple reader and writer threads; readers can access the data concurrently
    unless a writer is writing, while writers have exclusive access. This implementation
    ensures data consistency and prevents race conditions, illustrating a classic
    synchronization problem in concurrent programming.

    Contact Information:
    - Email: dengq@wabash.edu
    - GitHub: github.com/QixinDeng

    MIT License

    Copyright (c) 2024 Your Name

    Permission is hereby granted, free of charge, to any person obtaining a copy
    of this software and associated documentation files (the "Software"), to deal
    in the Software without restriction, including without limitation the rights
    to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
    copies of the Software, and to permit persons to whom the Software is
    furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included in all
    copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
    OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
    SOFTWARE.
*/
#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#define NUM_READERS 3
#define NUM_WRITERS 2

sem_t mutex, writeBlock;
int data = 0, readersCount = 0;

static void *reader(void *arg) {
    int readerID = *(int *)arg;
    while (1) {
        sem_wait(&mutex);
        readersCount++;
        if (readersCount == 1) // First reader locks writing
            sem_wait(&writeBlock);
        sem_post(&mutex);

        printf("Reader %d read data: %d\n", readerID, data);

        sem_wait(&mutex);
        readersCount--;
        if (readersCount == 0) // Last reader unlocks writing
            sem_post(&writeBlock);
        sem_post(&mutex);

        // Sleep to simulate reading
        sleep(1);
    }
}

static void *writer(void *arg) {
    int writerID = *(int *)arg;
    while (1) {
        sem_wait(&writeBlock);

        data++;
        printf("Writer %d wrote data: %d\n", writerID, data);

        sem_post(&writeBlock);

        // Sleep to simulate writing
        sleep(2);
    }
}

int semRWExample() {
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    int readerIDs[NUM_READERS], writerIDs[NUM_WRITERS];

    sem_init(&mutex, 0, 1);
    sem_init(&writeBlock, 0, 1);

    // Create reader threads
    for (int i = 0; i < NUM_READERS; i++) {
        readerIDs[i] = i + 1;
        pthread_create(&readers[i], NULL, reader, &readerIDs[i]);
    }

    // Create writer threads
    for (int i = 0; i < NUM_WRITERS; i++) {
        writerIDs[i] = i + 1;
        pthread_create(&writers[i], NULL, writer, &writerIDs[i]);
    }

    // Join threads
    for (int i = 0; i < NUM_READERS; i++)
        pthread_join(readers[i], NULL);

    for (int i = 0; i < NUM_WRITERS; i++)
        pthread_join(writers[i], NULL);

    sem_destroy(&mutex);
    sem_destroy(&writeBlock);

    return 0;
}
