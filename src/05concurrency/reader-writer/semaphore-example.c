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

// Define named semaphore names
#define SEM_READER_COUNT "/reader_count_sem"
#define SEM_WRITER "/writer_sem"

// Global semaphores
sem_t *reader_count_sem;
sem_t *writer_sem;

// Global variable to count the number of readers
int reader_count = 0;

// Shared resource
int shared_resource = 0;

void *reader(void *arg) {
    while (1) {
        sem_wait(reader_count_sem);
        reader_count++;
        if (reader_count == 1) {
            sem_wait(writer_sem); // Prevent writers while readers are reading
        }
        sem_post(reader_count_sem);

        // Reading
        printf("Reader reads: %d\n", shared_resource);
        sleep(1);

        sem_wait(reader_count_sem);
        reader_count--;
        if (reader_count == 0) {
            sem_post(writer_sem); // Allow writers if no readers are reading
        }
        sem_post(reader_count_sem);

        sleep(2);
    }
}

void *writer(void *arg) {
    while (1) {
        sem_wait(writer_sem); // Wait for exclusive access
        // Writing
        shared_resource++;
        printf("Writer writes: %d\n", shared_resource);
        sleep(2);
        sem_post(writer_sem); // Release access for other writers or readers
        sleep(3);
    }
}


int semRWExample() {
    pthread_t readers[NUM_READERS], writers[NUM_WRITERS];
    int i;
    sem_unlink(SEM_READER_COUNT);
    sem_unlink(SEM_WRITER);
    // Create named semaphores
    reader_count_sem = sem_open(SEM_READER_COUNT, O_TRUNC|O_CREAT, 0666, 1);
    writer_sem = sem_open(SEM_WRITER, O_TRUNC|O_CREAT, 0666, 1);

    // Create reader threads
    for (i = 0; i < NUM_READERS; i++)
        pthread_create(&readers[i], NULL, reader, NULL);

    // Create writer threads
    for (i = 0; i < NUM_WRITERS; i++)
        pthread_create(&writers[i], NULL, writer, NULL);

    // Join reader threads
    for (i = 0; i < NUM_READERS; i++)
        pthread_join(readers[i], NULL);

    // Join writer threads
    for (i = 0; i < NUM_WRITERS; i++)
        pthread_join(writers[i], NULL);

    // Close and unlink named semaphores
    sem_close(reader_count_sem);
    sem_close(writer_sem);
    sem_unlink(SEM_READER_COUNT);
    sem_unlink(SEM_WRITER);

    return 0;
}