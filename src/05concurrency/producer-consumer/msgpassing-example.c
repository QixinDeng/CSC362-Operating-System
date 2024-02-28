/*
    Name: Dr. Qixin Deng
    Date: February 28, 2024
    Description:
    The provided code is a C program that demonstrates message passing between
    producer and consumer processes using System V message queues. It defines a
    message structure and includes functions for both producers and consumers to
    send and receive messages through a message queue. Producers generate messages
    with a number and their ID, while consumers receive any available messages.
    The program manages multiple producers and consumers, ensuring synchronized
    access to the message queue, and concludes with cleaning up the message queue
    to prevent resource leaks.

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
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/wait.h>
#include <time.h>

#define QUEUE_PERMS 0666
#define NUM_PRODUCERS 2
#define NUM_CONSUMERS 3

static struct message {
    char msg_text[100];
};

static void producer(int msgid, int producer_id) {
    struct message msg;

    for (int i = 0; i < 3; ++i) { // Produce 10 messages
        int num = i; // Generate a random number
        snprintf(msg.msg_text, sizeof(msg.msg_text), "Producer %d: %d", producer_id, num);

        if (msgsnd(msgid, &msg, sizeof(msg.msg_text), 0) == -1) {
            perror("Producer: msgsnd failed");
            exit(EXIT_FAILURE);
        }

        printf("Producer %d sent: %s\n", producer_id, msg.msg_text);
        sleep(1); // Slow down the production rate
    }
}

static void consumer(int msgid, int consumer_id) {
    struct message msg;

    for (int i = 0; i < 2; ++i) { // Each consumer reads 20 messages
        // Receive any type of message
        if (msgrcv(msgid, &msg, sizeof(msg.msg_text), 0, 0) == -1) {
            perror("Consumer: msgrcv failed");
            exit(EXIT_FAILURE);
        }

        printf("Consumer %d received: %s\n", consumer_id, msg.msg_text);
        sleep(1); // Slow down the consume rate
    }
}

int messagePassing() {
    int msgid;
    key_t key = ftok("producer_consumer_ipc", 65); // Generate unique key
    msgid = msgget(key, QUEUE_PERMS | IPC_CREAT);
    if (msgid == -1) {
        perror("msgget failed");
        exit(EXIT_FAILURE);
    }

    for (int i = 0; i < NUM_PRODUCERS + NUM_CONSUMERS; ++i) {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) { // Child process
            if (i < NUM_PRODUCERS) {
                producer(msgid, i);
            } else {
                consumer(msgid, i-2);
            }
            exit(0); // Child exits after its task
        }
    }

    // Parent waits for all child processes
    for (int i = 0; i < NUM_PRODUCERS + NUM_CONSUMERS; ++i) {
        wait(NULL);
    }

    // Cleanup message queue in parent
    if (msgctl(msgid, IPC_RMID, NULL) == -1) {
        perror("msgctl failed");
        exit(EXIT_FAILURE);
    }

    return 0;
}
