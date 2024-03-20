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
#include <stdbool.h>

#define P 5 // Number of processes
#define R 3 // Number of resources


void calculateNeed(int need[P][R], int claim[P][R], int alloc[P][R]) {
    for (int i = 0; i < P; i++)
        for (int j = 0; j < R; j++)
            need[i][j] = claim[i][j] - alloc[i][j];
}

bool findSafeSeq(int avail[R], int claim[P][R], int alloc[P][R]) {
    int need[P][R];
    calculateNeed(need, claim, alloc);

    bool finished[P] = {0}; // track of which processes have completed
    int temp_avail[R]; // This vector represents the available resources at any point during the algorithm's execution.
    for (int i = 0; i < R; i++)
        temp_avail[i] = avail[i];

    int count = 0;
    /*The algorithm enters a loop that continues until all processes are marked as finished. Inside the loop, it
     * searches for a process p that is not yet finished (finished[p] == false) and whose resource needs can be
     * satisfied with the current work vector (i.e., for all j, need[p][j] <= work[j]). If such a process is found,
     * the algorithm simulates the execution of this process by adding its allocated resources back to the work
     * vector (since the process would release these resources upon completion). The process is then marked as
     * finished (finished[p] = true). The loop continues, each time looking for another process that can be satisfied
     * with the updated work vector.
     * */
    while (count < P) {
        bool found = false;
        for (int p = 0; p < P; p++) {
            if (!finished[p]) {
                int j;
                for (j = 0; j < R; j++)
                    if (need[p][j] > temp_avail[j])
                        break;

                if (j == R) {
                    for (int k = 0; k < R; k++)
                        temp_avail[k] += alloc[p][k];

                    finished[p] = true;
                    found = true;
                    count++;
                    printf("Process %d is run to completion!\n", p);
                }
            }
        }

        if (!found) {
            return false; // System is not in a safe state
        }
    }

    return true; // If all processes could finish, system is in a safe state
}

bool isRequestSafe(int request[R], int pID, int avail[R], int alloc[P][R], int claim[P][R]) {
    int need[P][R];
    calculateNeed(need, claim, alloc);

    // Check if request is less than or equal to need
    for (int i = 0; i < R; i++) {
        if (request[i] > need[pID][i]) {
            printf("Process has exceeded its maximum claim.\n");
            return false;
        }
    }

    // Check if request is less than or equal to available
    for (int i = 0; i < R; i++) {
        if (request[i] > avail[i]) {
            printf("Resources are not available.\n");
            return false;
        }
    }

    // Try to allocate requested resources temporarily
    for (int i = 0; i < R; i++) {
        avail[i] -= request[i];
        alloc[pID][i] += request[i];
    }

    // Check if system is still in a safe state after allocation
    bool safe = findSafeSeq(avail, claim, alloc);

    // Rollback the allocation for next checks
    for (int i = 0; i < R; i++) {
        avail[i] += request[i];
        alloc[pID][i] -= request[i];
    }

    return safe;
}

int bankers() {
    int processes[] = {0, 1, 2, 3, 4};

    // Example matrices and vectors
    int claim[P][R] = {{7, 5, 3},
                       {3, 2, 2},
                       {9, 0, 2},
                       {2, 2, 2},
                       {4, 3, 3}
    };

    int alloc[P][R] = {{0, 1, 0},
                       {2, 0, 0},
                       {3, 0, 2},
                       {2, 1, 1},
                       {0, 0, 2}
    };
    int avail[] = {3, 3, 2};

    // Example request
    int request[] = {1, 2, 1}; // Request for process 1
    int pID = processes[0]; // Process making the request

    if (isRequestSafe(request, pID, avail, alloc, claim)) {
        printf("The request can be safely granted.\n");
    } else {
        printf("The request cannot be safely granted.\n");
    }

    return 0;
}

