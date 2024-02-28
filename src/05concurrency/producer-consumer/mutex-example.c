/*
    Name: Dr. Qixin Deng
    Date: February 28, 2024
    Description:
    The code is a C program demonstrating the use of mutexes for synchronizing
    access to a shared buffer between producer and consumer threads. It defines a
    fixed-size buffer and uses a mutex to ensure that only one thread can access
    the buffer at any given time to prevent race conditions. Producers insert items
    into the buffer, and consumers remove items, with both operations guarded by the
    mutex. The program highlights the importance of thread synchronization in scenarios
    where multiple threads interact with shared resources.

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
#include <unistd.h>

#define BUFFER_SIZE 5

static int buffer[BUFFER_SIZE];
static int count = 0; // Number of items in the buffer
static int in = 0; // Next place to write
static int out = 0; // Next place to read

static pthread_mutex_t mutex;

// Producer function
static void* producer(void* arg) {
    for(int i = 0; i < 20; i++) {
            pthread_mutex_lock(&mutex);
            if(count < BUFFER_SIZE) { // Check if there's space in the buffer
                // Produce an item
                buffer[in] = i;
                in = (in + 1) % BUFFER_SIZE;
                count++;
                printf("Produced: %d, count = %d \n", i, count);
                pthread_mutex_unlock(&mutex);
            }
            else{
                i--;
                pthread_mutex_unlock(&mutex);
            }
        sleep(2);
        }
    return NULL;
}

// Consumer function
static void* consumer(void* arg) {
    for(int i = 0; i < 20; i++) {
            pthread_mutex_lock(&mutex);
            if(count > 0) { // Check if there's an item in the buffer
                // Consume an item
                int item = buffer[out];
                out = (out + 1) % BUFFER_SIZE;
                count--;
                printf("Consumed: %d\n", item);
                pthread_mutex_unlock(&mutex);
            }
            else {
                i--;
                pthread_mutex_unlock(&mutex);
            }
        sleep(1);
        }
    return NULL;
}

int mutexExample() {
    pthread_mutex_init(&mutex, NULL);
    pthread_t prod, cons;

    // Create producer and consumer threads
    pthread_create(&prod, NULL, producer, NULL);
    pthread_create(&cons, NULL, consumer, NULL);

    // Wait for the threads to finish
    pthread_join(prod, NULL);
    pthread_join(cons, NULL);

    // Cleanup
    pthread_mutex_destroy(&mutex);

    return 0;
}
