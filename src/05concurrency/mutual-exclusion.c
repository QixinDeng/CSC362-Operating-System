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

#include "os-examples.h"
#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>


// Shared variables
static int want_to_enter[2] = {0, 0}; // Flags for the two threads wanting to enter the critical section
static int turn = 0; // Variable to control whose turn it is
// Define a structure to hold multiple parameters
typedef struct {
    int process;
    int alg;
} ThreadArgs;

static void enter_critical_section(int process, int alg) {

    /**************First attempt****************/
//    while (turn != process) continue;

    /**************Second attempt****************/
//    while(want_to_enter[1-process]) continue;
//    want_to_enter[process] = 1;

    /**************Third attempt****************/
//    want_to_enter[process] = 1;
//    while(want_to_enter[1-process]) continue;

    /**************Fourth attempt****************/
//    want_to_enter[process] = 1;
//    while(want_to_enter[1-process]){
//        want_to_enter[process] = 0;
//        sleep(1);
//        want_to_enter[process] = 1;
//    }



    if (alg == 0){
        //Dekker's
        printf("Process %d wants to enter the critical section.\n", process);
        want_to_enter[process] = 1; // Indicate that this process wants to enter the critical section
        while (want_to_enter[1 - process]) { // Loop while the other process wants to enter
            if (turn != process) { // If it's not this process's turn,
                want_to_enter[process] = 0; // relinquish the desire to enter the critical section
                while (turn != process); // and wait until it's this process's turn
                want_to_enter[process] = 1; // Then, indicate again that this process wants to enter
            }
        }
    }
    else if(alg == 1){
        //Peterson's
        printf("Process %d wants to enter the critical section.\n", process);
        int other = 1 - process; // Determine the other process
        want_to_enter[process] = 1; // Indicate that this process wants to enter the critical section
        turn = other; // Give the turn to the other process
        while (want_to_enter[other] && turn == other) {
            // Busy wait
        }
    }
    else{
        exit(0);
    }
}

static void leave_critical_section(int process, int alg) {
    /**************First attempt****************/
//    turn = 1 - process;
    /**************Second attempt****************/
//    want_to_enter[process] = 0;

    /**************Third attempt****************/
//    want_to_enter[process] = 0;

    /**************Fourth attempt****************/
//    want_to_enter[process] = 0;

    if(alg == 0){
        printf("Process %d has left the critical section.\n", process);
        turn = 1 - process; // Pass the turn to the other process
        want_to_enter[process] = 0; // Indicate that this process no longer wants to enter the critical section
    }
    else if(alg == 1){
        printf("Process %d has left the critical section.\n", process);
        want_to_enter[process] = 0; // Indicate that this process no longer wants to enter the critical section
    }
    else{
        exit(0);
    }
}

static void* process_function(void* arg) {

    ThreadArgs* args = (ThreadArgs*) arg;

    enter_critical_section(args->process, args->alg);

    // Critical section
    printf("Process %d is in the critical section.\n", args->process);
    sleep(1); // Simulate work in the critical section

    leave_critical_section(args->process,args->alg);

    free(args);
    return NULL;
}

int mutualExclusion() {
    pthread_t threads[2];
    int process_ids[2] = {0, 1};
    int test_round = 2;
    int alg = 0;

    while (test_round--){
        // Create two threads representing two processes
        for (int i = 0; i < 2; i++) {
            ThreadArgs* args = (ThreadArgs*) malloc(sizeof(ThreadArgs)); // Allocate memory for the arguments
            args->process = process_ids[i];
            args->alg = alg;
            pthread_create(&threads[i], NULL, process_function, args);
        }

        // Wait for both threads to complete
        for (int i = 0; i < 2; i++) {
            pthread_join(threads[i], NULL);
        }
    }
    return 0;
}
