/*
    Name: Dr. Qixin Deng
    Date: February 28, 2024
    Description:
    The code is a C program demonstrating the use of semaphores and mutexes for
    synchronizing producer and consumer threads. It defines a buffer and utilizes
    semaphores to track filled and empty slots in the buffer, ensuring producers
    don't overflow and consumers don't underflow the buffer. The mutex locks the
    buffer during access to prevent race conditions. Producers insert integers
    into the buffer, while consumers remove them, with both operations printing
    the action taken, the buffer position, and the current count of items in the buffer.

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
#include <fcntl.h> // For O_CREAT, O_EXEC

#define BUFFER_SIZE 5 // Size of the buffer

sem_t *filled; // Semaphore to keep track of the filled slots
sem_t *empty; // Semaphore to keep track of the empty slots
pthread_mutex_t mutex; // Mutex for protecting buffer access

int buffer[BUFFER_SIZE]; // Buffer
int in = 0; // Next place to write in the buffer
int out = 0; // Next place to read from the buffer
int count = 0; // Count of items in the buffer

// Producer function
void* producer(void* arg) {
    for(int i = 0; i < 20; ++i) {
        sem_wait(empty); // Wait if the buffer is full
        pthread_mutex_lock(&mutex); // Lock the mutex before accessing the buffer
        // Add item to the buffer
        buffer[in] = i;
        in = (in + 1) % BUFFER_SIZE;
        count++; // Increment the count of items in the buffer
        sleep(2);
        printf("Produced: %d, in: %d, count: %d\n", i, in, count);
        pthread_mutex_unlock(&mutex); // Unlock the mutex after accessing the buffer
        sem_post(filled); // Signal that there's a new item in the buffer
    }
    return NULL;
}

// Consumer function
void* consumer(void* arg) {
    for(int i = 0; i < 20; ++i) {
        sem_wait(filled); // Wait if the buffer is empty
        pthread_mutex_lock(&mutex); // Lock the mutex before accessing the buffer
        // Remove item from the buffer
        int item = buffer[out];
        out = (out + 1) % BUFFER_SIZE;
        count--; // Decrement the count of items in the buffer
        sleep(1);
        printf("Consumed: %d, out: %d, count: %d\n", item, out, count);
        pthread_mutex_unlock(&mutex); // Unlock the mutex after accessing the buffer
        sem_post(empty); // Signal that there's a new empty space in the buffer
    }
    return NULL;
}

int semExample() {
    pthread_t prod, cons;

    // Initialize named semaphores
    filled = sem_open("/filled_sem", O_CREAT, 0644, 0);
    empty = sem_open("/empty_sem", O_CREAT, 0644, BUFFER_SIZE);
    if (filled == SEM_FAILED || empty == SEM_FAILED) {
        perror("Failed to open semaphores");
        return 1;
    }

    // Initialize mutex
    pthread_mutex_init(&mutex, NULL);

    // Create producer and consumer threads
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    // Wait for the threads to finish
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    // Cleanup
    sem_close(filled);
    sem_close(empty);
    sem_unlink("/filled_sem");
    sem_unlink("/empty_sem");
    pthread_mutex_destroy(&mutex);

    return 0;
}

