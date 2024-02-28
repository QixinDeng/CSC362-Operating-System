/*
    Name: Dr. Qixin Deng
    Date: February 28, 2024
    Description:
    The code is a C program that demonstrates various threading concepts, including
    creating, executing, and synchronizing threads. It uses mutexes to manage access
    to shared resources, such as a counter variable and an array, to illustrate safe
    incrementing and decrementing operations by multiple threads. The program also
    includes functions to measure the performance of threads versus processes and to
    demonstrate parallel versus sequential execution in tasks like summing elements
    of a large array, showcasing the efficiency gains possible with multithreading.

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
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <time.h>

#define NUM 100
#define NUM_THREADS 4
#define ARRAY_SIZE 10000000

static int counter = 0;
static pthread_mutex_t lock;
static int array[ARRAY_SIZE];
static long long sum = 0;

static void* incrementCounter(void* arg) {
    int i;
    for (i = 0; i < 10; i++) {
        pthread_mutex_lock(&lock);
        counter++;
        printf("Thread 1 incremented counter to %d\n", counter);
        pthread_mutex_unlock(&lock);
        sleep(1);
    }
    return NULL;
}

static void* decrementCounter(void* arg) {
    int i;
    for (i = 0; i < 10; i++) {
        pthread_mutex_lock(&lock);
        counter--;
        printf("Thread 2 decremented counter to %d\n", counter);
        pthread_mutex_unlock(&lock);
        sleep(3);
    }
    return NULL;
}

int unitOfExecution() {
    pthread_t thread1, thread2;

    if (pthread_mutex_init(&lock, NULL) != 0) {
        printf("Mutex init failed\n");
        return 1;
    }

    if (pthread_create(&thread1, NULL, incrementCounter, NULL)) {
        printf("Error creating Thread 1\n");
        return 1;
    }

    if (pthread_create(&thread2, NULL, decrementCounter, NULL)) {
        printf("Error creating Thread 2\n");
        return 1;
    }

    if (pthread_join(thread1, NULL)) {
        printf("Error joining Thread 1\n");
        return 1;
    }

    if (pthread_join(thread2, NULL)) {
        printf("Error joining Thread 2\n");
        return 1;
    }

    pthread_mutex_destroy(&lock);

    printf("Final counter value: %d\n", counter);
    printf("Both threads have finished executing.\n");

    return 0;
}


static void* threadFunc(void* arg) {
    return NULL;
}

static void measureThreads() {
    pthread_t threads[NUM];
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    for (int i = 0; i < NUM; i++) {
        pthread_create(&threads[i], NULL, threadFunc, NULL);
    }

    for (int i = 0; i < NUM; i++) {
        pthread_join(threads[i], NULL);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Time taken to create and join %d threads: %f seconds\n", NUM, cpu_time_used);
}

static void measureProcesses() {
    pid_t pids[NUM];
    clock_t start, end;
    double cpu_time_used;

    start = clock();
    for (int i = 0; i < NUM; i++) {
        if ((pids[i] = fork()) == 0) {
            exit(0);
        }
    }

    for (int i = 0; i < NUM; i++) {
        waitpid(pids[i], NULL, 0);
    }
    end = clock();
    cpu_time_used = ((double) (end - start)) / CLOCKS_PER_SEC;

    printf("Time taken to create and wait for %d processes: %f seconds\n", NUM, cpu_time_used);
}

int fasterThread() {
    measureThreads();
    measureProcesses();
    return 0;
}


static double current_time_ns() {
    struct timespec ts;
    clock_gettime(CLOCK_REALTIME, &ts);
    return (double)(ts.tv_sec * 1.0e9 + ts.tv_nsec) / 1000000000.0;
}

static void *calculate_sum(void *threadid) {
    long tid = (long)threadid;
    long long local_sum = 0;
    long long start = (ARRAY_SIZE / NUM_THREADS) * tid;
    long long end = start + (ARRAY_SIZE / NUM_THREADS);

    for (long long i = start; i < end; i++) {
        local_sum += array[i];
    }

    pthread_mutex_lock(&lock);
    sum += local_sum;
    pthread_mutex_unlock(&lock);

    pthread_exit(NULL);
}

int splitAJob () {
    pthread_t threads[NUM_THREADS];
    int rc;
    long t;
    double start, end;
    double time_used;

    // Initialize array
    for(int i = 0; i < ARRAY_SIZE; i++) {
        array[i] = 1;
    }

    // Sequential sum
    start = current_time_ns();
    long long sequential_sum = 0;
    for(int i = 0; i < ARRAY_SIZE; i++) {
        sequential_sum += array[i];
    }
    end = current_time_ns();
    time_used = (double) (end - start);
    printf("\nSequential read time: %f s\n", time_used);

    // Parallel sum
    pthread_mutex_init(&lock, NULL);
    start = current_time_ns();
    for(t = 0; t < NUM_THREADS; t++) {
        rc = pthread_create(&threads[t], NULL, calculate_sum, (void *)t);
        if (rc) {
            printf("ERROR; return code from pthread_create() is %d\n", rc);
            exit(-1);
        }
    }

    for(t = 0; t < NUM_THREADS; t++) {
        pthread_join(threads[t], NULL);
    }
    end = current_time_ns();
    time_used = (double) (end - start);
    printf("\nParallel read time: %f s\n", time_used);

    pthread_mutex_destroy(&lock);
    pthread_exit(NULL);
}






