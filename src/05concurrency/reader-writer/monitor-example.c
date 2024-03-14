/*
    Name: Dr. Qixin Deng
    Date: February 28, 2024
    Description:
    The code is a C program demonstrating the use of a monitor structure to synchronize
    access between multiple reader and writer threads to a shared buffer. It uses mutexes
    and condition variables (Monitor) to manage access, ensuring writers have exclusive access and
    readers can access concurrently when no writers are present. The program initializes
    a monitor with a buffer, creates several reader and writer threads, and then synchronizes
    their operations to prevent race conditions, showcasing a classic readers-writers problem
    solution with priority to writers.

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
#include <pthread.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#define BUFFER_SIZE 5  // Size of the buffer

// Monitor structure
typedef struct {
    pthread_mutex_t mutex;          // Mutex for critical section
    pthread_cond_t readers_cond;    // Condition variable for readers
    pthread_cond_t writers_cond;    // Condition variable for writers
    int active_readers;             // Number of readers currently reading
    int waiting_writers;            // Number of writers waiting to write
    int buffer[BUFFER_SIZE];        // Buffer for storing data
    int buffer_count;               // Counter for the number of items in the buffer
} RW_Monitor;

RW_Monitor monitor = {PTHREAD_MUTEX_INITIALIZER, PTHREAD_COND_INITIALIZER, PTHREAD_COND_INITIALIZER, 0, 0, {0}, 0};

static void* reader(void* args) {
    while (1) {  // Continuous reading
        pthread_mutex_lock(&monitor.mutex);

        // Wait until there are no waiting writers
        while (monitor.waiting_writers > 0) {
            pthread_cond_wait(&monitor.readers_cond, &monitor.mutex);
        }

        monitor.active_readers++;
        pthread_mutex_unlock(&monitor.mutex);

        sleep(1);  // Simulate reading time
        printf("Reader % d is reading: ", (int)(intptr_t)args);
        for (int i = 0; i < BUFFER_SIZE; i++) {
            printf("%d ", monitor.buffer[i]);
        }
        printf("Active readers number is %d \n", monitor.active_readers);

        pthread_mutex_lock(&monitor.mutex);
        monitor.active_readers--;

        // If last reader, signal waiting writers if any
        if (monitor.active_readers == 0 && monitor.waiting_writers > 0) {
            pthread_cond_signal(&monitor.writers_cond);
        }
        pthread_mutex_unlock(&monitor.mutex);

    }

    return NULL;
}

static void* writer(void* args) {
    int data = (int)(intptr_t)args;  // Retrieve data passed to thread
    while (1) {  // Continuous writing
        pthread_mutex_lock(&monitor.mutex);

        monitor.waiting_writers++;

        // Wait until the buffer is not being read or written to
        while (monitor.active_readers > 0 || monitor.buffer_count > 0) {
            pthread_cond_wait(&monitor.writers_cond, &monitor.mutex);
        }

        monitor.waiting_writers--;
        monitor.buffer_count = BUFFER_SIZE;  // Indicate buffer is being written to

        // Write data to the entire buffer
        for (int i = 0; i < BUFFER_SIZE; i++) {
            monitor.buffer[i] = data;
        }

        printf("Writer %ld wrote: ", (long)args);
        for (int i = 0; i < BUFFER_SIZE; i++) {
            printf("%d ", monitor.buffer[i]);
        }
        printf("\n");

        monitor.buffer_count = 0;  // Indicate buffer is no longer being written to

        // Signal to all waiting readers and writers
        pthread_cond_broadcast(&monitor.readers_cond);
        if (monitor.waiting_writers > 0) {
            pthread_cond_signal(&monitor.writers_cond);
        }

        pthread_mutex_unlock(&monitor.mutex);
        sleep(1);  // Simulate writing time
    }
    return NULL;
}



int monitorRW() {
    pthread_t readers[3], writers[2];

    // Create reader threads
    for (int i = 0; i < 3; ++i) {
        pthread_create(&readers[i], NULL, reader, (void*)(intptr_t)(i + 1));
    }

    // Create writer threads
    for (int i = 0; i < 2; ++i) {
        pthread_create(&writers[i], NULL, writer, (void*)(intptr_t)(i + 1));  // Passing unique data for each writer
    }

    // Wait for all threads to finish
    for (int i = 0; i < 3; ++i) {
        pthread_join(readers[i], NULL);
    }

    for (int i = 0; i < 2; ++i) {
        pthread_join(writers[i], NULL);
    }

    return 0;
}

