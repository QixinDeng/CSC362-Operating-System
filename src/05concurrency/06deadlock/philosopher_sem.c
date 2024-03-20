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
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h> // For O_CREAT, O_EXEC
#include <sys/stat.h> // For mode constants

#define NUM_PHILOSOPHERS 5
int philosopher_number[NUM_PHILOSOPHERS];

// Names for the semaphores
char fork_names[NUM_PHILOSOPHERS][20];
char room_name[] = "/room_sem";

// Semaphore pointers for forks and room
sem_t* forks[NUM_PHILOSOPHERS];
sem_t* room;

void take_fork(int phil) {
    sem_wait(forks[phil]); // Take left fork
    sem_wait(forks[(phil + 1) % NUM_PHILOSOPHERS]); // Take right fork

    printf("Philosopher %d is eating.\n", phil + 1);
}

void put_fork(int phil) {
    sem_post(forks[phil]); // Put down left fork
    sem_post(forks[(phil + 1) % NUM_PHILOSOPHERS]); // Put down right fork

    printf("Philosopher %d is thinking.\n", phil + 1);
}

void* philosopher(void* num) {
    while (1) {
        int* i = num;

        sleep(1);
        sem_wait(room); // Enter critical region
        take_fork(*i);
        sleep(1); // Eating
        put_fork(*i);
        sem_post(room); // Leave critical region
    }
}

int philosopherSem() {
    pthread_t thread_id[NUM_PHILOSOPHERS];

    // Initialize named semaphores
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        snprintf(fork_names[i], sizeof(fork_names[i]), "/fork_sem_%d", i);
        if ((forks[i] = sem_open(fork_names[i], O_CREAT, 0666, 1)) == SEM_FAILED) {
            perror("sem_open");
            exit(EXIT_FAILURE);
        }
        philosopher_number[i] = i;
    }

    if ((room = sem_open(room_name, O_CREAT, 0666, NUM_PHILOSOPHERS - 1)) == SEM_FAILED) {
        perror("sem_open");
        exit(EXIT_FAILURE);
    }

    // Create philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_create(&thread_id[i], NULL, philosopher, &philosopher_number[i]);
        printf("Philosopher %d is thinking.\n", i + 1);
    }

    // Join philosopher threads
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pthread_join(thread_id[i], NULL);
    }

    // Close and unlink semaphores
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_close(forks[i]);
        sem_unlink(fork_names[i]);
    }
    sem_close(room);
    sem_unlink(room_name);

    return 0;
}

