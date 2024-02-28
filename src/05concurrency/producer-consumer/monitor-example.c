/*
    Name: Dr. Qixin Deng
    Date: February 28, 2024
    Description:
    The code is a C program that implements a monitor using mutexes and condition
    variables to manage concurrent access to a buffer by producer and consumer threads.
    It defines a Monitor struct with a buffer, counters, and synchronization primitives.
    Functions init_monitor, producer, and consumer initialize the monitor, produce items
    to the buffer, and consume items from the buffer, respectively. The monitorExample
    function initializes the monitor and creates the producer and consumer threads,
    demonstrating a classic producer-consumer problem with thread synchronization in C.

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
#include "os-examples.h"
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#define BUFFER_SIZE 5

// Monitor structure
typedef struct {
    int buffer[BUFFER_SIZE];
    int in;
    int out;
    int count;
    pthread_cond_t not_full;
    pthread_cond_t not_empty;
    pthread_mutex_t mutex;
} Monitor;


// Initialize the monitor
void init_monitor(Monitor *mon) {
    mon->in = 0;
    mon->out = 0;
    mon->count = 0;
    pthread_cond_init(&mon->not_full, NULL);
    pthread_cond_init(&mon->not_empty, NULL);
    pthread_mutex_init(&mon->mutex, NULL);
}

// Producer function
static void *producer(void *arg) {
    pthread_t id = pthread_self();
    Monitor *mon = (Monitor *)arg;
    int item = 0;
    int i = 20;
    while (i--) {
        // Produce item
        item++;

        // Wait if buffer is full
        if (mon->count == BUFFER_SIZE)
        {
            pthread_cond_wait(&mon->not_full, &mon->mutex);
        }


        // Insert item into buffer
        mon->buffer[mon->in] = item;
        mon->in = (mon->in + 1) % BUFFER_SIZE;
        mon->count++;

        // Signal that buffer is not empty
        printf("Producer id: % d produced: %d count %d \n", id, item, mon->count);
        sleep(1);
        pthread_cond_signal(&mon->not_empty);
    }
    return NULL;
}

// Consumer function
static void *consumer(void *arg) {
    pthread_t id = pthread_self();
    Monitor *mon = (Monitor *)arg;
    int item;
    int i = 20;
    while (i--) {
        // Wait if buffer is empty
        if (mon->count == 0)
            pthread_cond_wait(&mon->not_empty, &mon->mutex);


        // Remove item from buffer
        item = mon->buffer[mon->out];
        mon->out = (mon->out + 1) % BUFFER_SIZE;
        mon->count--;

        // Signal that buffer is not full
        printf("Consumer id : %d consumed: %d\n", id, item);
        sleep(2);
        pthread_cond_signal(&mon->not_full);
    }
    return NULL;
}


int monitorExample() {
    pthread_t producer_thread, consumer_thread;
    Monitor mon;

    // Initialize the monitor
    init_monitor(&mon);

    // Create producer and consumer threads
    pthread_create(&producer_thread, NULL, producer, (void *)&mon);
    pthread_create(&consumer_thread, NULL, consumer, (void *)&mon);


    // Wait for threads to finish (which will never happen due to infinite loop)
    pthread_join(producer_thread, NULL);
    pthread_join(consumer_thread, NULL);

    return 0;
}
