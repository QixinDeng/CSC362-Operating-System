/*
    Name: Dr. Qixin Deng
    Date: February 28, 2024
    Description:
    The code is a C program that implements mutual exclusion solutions, specifically
    Dekker's and Peterson's algorithms, to control access to a critical section in a
    concurrent programming environment. It uses shared variables to indicate the desire
    of threads to enter the critical section and a turn variable to decide whose turn
    it is to enter. The program creates two threads, each representing a process, and
    alternates between them to demonstrate mutual exclusion and ensure that only one
    thread can execute in the critical section at a time.

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
#include <stdlib.h>
#define NUM_PHILOSOPHERS 5

typedef struct {
    pthread_mutex_t mutex;
    pthread_cond_t forks_ready[NUM_PHILOSOPHERS];
    int forks[NUM_PHILOSOPHERS];
} Pho_Monitor;

Pho_Monitor *pho_monitor = NULL;

static void init_monitor() {
    pho_monitor = (Pho_Monitor*)malloc(sizeof(Pho_Monitor));
    pthread_mutex_init(&pho_monitor->mutex, NULL);
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_cond_init(&pho_monitor->forks_ready[i], NULL);
        pho_monitor->forks[i] = 1; // Initialize all forks as available
    }
}

static void pickup(int philosopher_id) {
    pthread_mutex_lock(&pho_monitor->mutex);
    if (!pho_monitor->forks[philosopher_id]) {
        pthread_cond_wait(&pho_monitor->forks_ready[philosopher_id], &pho_monitor->mutex);
    }
    pho_monitor->forks[philosopher_id] = 0; // Mark the left fork as unavailable
    if (!pho_monitor->forks[(philosopher_id + 1) % NUM_PHILOSOPHERS]) {
        pthread_cond_wait(&pho_monitor->forks_ready[(philosopher_id + 1) % NUM_PHILOSOPHERS], &pho_monitor->mutex);
    }
    pho_monitor->forks[(philosopher_id + 1) % NUM_PHILOSOPHERS] = 0; // Mark the right fork as unavailable
    pthread_mutex_unlock(&pho_monitor->mutex);
    printf("Philosopher %d is eating.\n", philosopher_id);
}

static void putdown(int philosopher_id) {
    pthread_mutex_lock(&pho_monitor->mutex);
    pho_monitor->forks[philosopher_id] = 1; // Mark the left fork as available
    pthread_cond_signal(&pho_monitor->forks_ready[philosopher_id]); // Signal the right philosopher
    pho_monitor->forks[(philosopher_id + 1) % NUM_PHILOSOPHERS] = 1; // Mark the right fork as available
    pthread_cond_signal(&pho_monitor->forks_ready[(philosopher_id + 1) % NUM_PHILOSOPHERS]); // Signal the right philosopher
    pthread_mutex_unlock(&pho_monitor->mutex);
}

static void* philosopher(void *arg) {
    int id = (int)(intptr_t)arg;

    while (1) {
        printf("Philosopher %d is thinking.\n", id);
        sleep(1); // thinking for 1 second
        pickup(id);
        sleep(1); // eating for 1 second
        putdown(id);
    }
    return NULL;
}

int philosopherMon() {
    pthread_t philosophers[NUM_PHILOSOPHERS];
    init_monitor();

    printf("start!\n");
    for (int i = 0; i < NUM_PHILOSOPHERS; ++i) {
        pthread_create(&philosophers[i], NULL, philosopher, (void*)(intptr_t)(i + 1));
    }

    for (int i = 0; i < NUM_PHILOSOPHERS; ++i)
        pthread_join(philosophers[i], NULL);

    return 0;
}
