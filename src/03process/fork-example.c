/*
    Name: Dr. Qixin Deng
    Date: February 28, 2024
    Description:
    The code is a C program demonstrating the use of the fork() system call to
    create child processes.It contains two main functions, forkExample() and
    nestedFork(). In forkExample(), the process forks to create a child that
    writes a message to a file, while the parent waits for the child to finish
    and then reads the message from the file. In nestedFork(), the program
    illustrates a more complex scenario with nested forks, creating a hierarchy
    of parent and child processes, each identified by their process IDs.

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
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

int forkExample() {
    int fd;
    pid_t pid;
    char buffer[1024];
    ssize_t bytes_written, bytes_read;

    pid = fork();
    if (pid == -1) {
        perror("fork failed");
        exit(-1);
    } else if (pid == 0) {
        // Child process
        fd = open("child_output.txt", O_WRONLY | O_CREAT | O_TRUNC, 0644); // Open a file for writing
        printf("Running child process!\n");
        char msg[1024];
        int len = snprintf(msg, sizeof(msg), "This is child process with PID: %d\n", getpid());
        bytes_written = write(fd, msg, len); // Write to the file

        close(fd);
        sleep(3);

        exit(0);
    } else {
        // Parent process
        int status;
        wait(&status); // Wait for the child process to finish
        fd = open("child_output.txt", O_RDONLY); // Open the file for reading
        bytes_read = read(fd, buffer, sizeof(buffer) - 1); // Read from the file

        buffer[bytes_read] = '\0'; // Null-terminate the string
        printf("Parent process [%d] reading: %s", getpid(), buffer); // Print the content read from the file
        close(fd);
    }
    sleep(3);
    return 0;
}


int nestedFork() {
    //parent process pid=6018
    printf("process PID=%d\n", getpid());

    pid_t pid1 = fork(); // First fork

    if (pid1 == -1) {
        perror("First fork failed");
        return 1;
    } else if (pid1 == 0) {
        // process pid=6019
        printf("process PID=%d, PPID=%d\n", getpid(), getppid());

        pid_t pid2 = fork(); // Fork from the first child

        if (pid2 == -1) {
            perror("Fork from first child failed");
            return 1;
        } else if (pid2 == 0) {
            // process pid=6010
            printf("process PID=%d, PPID=%d\n", getpid(), getppid());
        } else {
            wait(NULL); // Wait for the grandchild to finish
        }
    } else {
        wait(NULL); // Wait for the first child to finish
        pid_t pid3 = fork();

        if (pid3 == -1) {
            perror("Second fork in main parent failed");
            return 1;
        } else if (pid3 == 0) {
            // process id=6111
            printf("process PID=%d, PPID=%d\n", getpid(), getppid());
        } else {
            wait(NULL); // Wait for the second child to finish
        }
    }
    return 0;
}
