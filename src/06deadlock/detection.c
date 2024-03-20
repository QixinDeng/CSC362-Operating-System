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
#include <stdlib.h>
#include<stdbool.h>

#define P 5  // Number of processes
#define R 3  // Number of resource types

int WFG[P][P] = {{0}};  // Wait-For-Graph

void populateWFG(int req[P][R], int avail[R], int alloc[P][R]) {
    for (int i = 0; i < P; ++i) {
        for (int j = 0; j < R; ++j) {
            if (req[i][j] > avail[j]) {
                for (int k = 0; k < P; ++k) {
                    if (alloc[k][j] > 0) {
                        WFG[i][k] = 1;  // Process i is waiting for resources held by process k
                    }
                }
                /*
                 * At this point, even if process i is waiting for additional resources, the critical information
                 * that it is waiting (and hence a potential participant in a deadlock) is already captured by adding
                 * the relevant edges in the WFG. Adding more edges for other resources it's waiting for won't change
                 * its status as a waiting process nor will it affect the detection of a cycle (deadlock) in the graph,
                 * because a deadlock is characterized by a set of processes waiting for each other in a circular
                 * chain, regardless of the specific resources they're waiting for. Therefore, once a process is
                 * identified as waiting for a resource, further checks for other resources in the context of
                 * populating the WFG for deadlock detection are redundant.
                 */
                break;
            }
        }
    }
}


int dfs(int node, int visited[], int current_path[]) {
    if (!visited[node]) {
        visited[node] = 1;
        current_path[node] = 1;

        for (int i = 0; i < P; i++) {
            // Check if there's an edge from the current node to node 'i'
            if (WFG[node][i]) {
                // If 'i' is on the current path, a cycle is detected
                if (current_path[i]) return 1;

                // If 'i' has not been visited, visit it and check for a cycle
                if (!visited[i])
                    if (dfs(i, visited, current_path)) return 1;
            }
        }
    }
    current_path[node] = 0;  // Remove the node from recursion stack
    return 0;
}

bool isDeadlock() {
    int visited[P] = {0};
    int current_path[P] = {0};
    //ensure that all components of the graph are explored, particularly in cases where the graph might be disconnected
    for (int i = 0; i < P; i++) {
        if (dfs(i, visited, current_path))
            return true;  // Deadlock detected
    }

    return false;  // No deadlock
}

int detection() {
    int req[P][R] = {
            {0, 0, 1},  // P0
            {1, 0, 0},  // P1
            {0, 1, 0}   // P2
    };
    int avail[R] = {0, 0, 0};


    int alloc[P][R] = {
            {1, 1, 0},  // P0
            {0, 2, 1},  // P1
            {1, 1, 0}   // P2
    };

    // Populate the WFG based on current resource allocation state
    populateWFG(req, avail, alloc);

    // Check for deadlocks
    if (isDeadlock()) {
        printf("Deadlock detected.\n");
    } else {
        printf("No deadlock detected.\n");
    }

    return 0;
}
