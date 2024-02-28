/*
    Name: Dr. Qixin Deng
    Date: February 28, 2024
    Description:
    The provided C program demonstrates the use of UNIX pipes for inter-process
    communication between a producer and a consumer process. It utilizes two pipes:
    mayproduce for producer-to-consumer communication and mayconsume for
    consumer-to-producer acknowledgments. The producer sends a series of messages
    through one pipe, while the consumer reads these messages and sends back acknowledgments
    through the other pipe. The program showcases how pipes can be used for synchronized
    message passing and acknowledgment between processes in a simple producer-consumer model.

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
#include <string.h>

// Global variables for pipes
int mayproduce[2];
int mayconsume[2];

static void producer() {
    char msg[100];
    char ack;

    for (int i = 0; i < 5; i++) {
        // Prepare a message
        snprintf(msg, sizeof(msg), "Message %d", i + 1);

        // Write the message to the consumer
        write(mayconsume[1], msg, strlen(msg) + 1);
        printf("Produced: %s\n", msg);

        // Wait for acknowledgment from the consumer
        read(mayproduce[0], &ack, 1);
    }
}

static void consumer() {
    char msg[100];

    while (read(mayconsume[0], msg, sizeof(msg)) > 0) {//pipes in blocking mode (the default behavior) will cause the reading process to wait for data to become available.
        printf("Consumed: %s\n", msg);

        // Send acknowledgment to the producer
        write(mayproduce[1], "1", 1);
    }
}


int pipeExample() {
    // Create pipes
    if (pipe(mayproduce) == -1 || pipe(mayconsume) == -1) {
        perror("pipe");
        exit(EXIT_FAILURE);
    }

    // Fork a new process
    pid_t pid = fork();

    if (pid == -1) {
        perror("fork");
        exit(EXIT_FAILURE);
    } else if (pid == 0) {
        // Child process: Consumer
        consumer();
    } else {
        // Parent process: Producer
        producer();
    }

    // Close pipes in the main process
    close(mayproduce[0]);
    close(mayproduce[1]);
    close(mayconsume[0]);
    close(mayconsume[1]);

    return 0;
}

