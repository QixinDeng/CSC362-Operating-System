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

#define PAGE_SEQ_LEN 12
#define NUMBER_OF_FRAMES 4

// Structure for representing a page in memory
typedef struct {
    int number; // Page number
    int lastUsedTime; // The last time this page was accessed
} PageFrame;

void initializePageFrames(PageFrame frames[]) {
    for (int i = 0; i < NUMBER_OF_FRAMES; i++) {
        frames[i].number = -1; // -1 indicates that the frame is initially empty
        frames[i].lastUsedTime = -1; // Initialize last used time as -1
    }
}

void printFrames(PageFrame frames[]) {
    for (int i = 0; i < NUMBER_OF_FRAMES; i++) {
        if (frames[i].number != -1) {
            printf("%d(%d) ", frames[i].number, frames[i].lastUsedTime);
        } else {
            printf("- ");
        }
    }
    printf("\n");
}

// Function to find the least recently used page
int findLRUPageFrameIndex(PageFrame frames[]) {
    int lruIndex = 0;
    int earliestTime = frames[0].lastUsedTime;

    for (int i = 1; i < NUMBER_OF_FRAMES; i++) {
        if (frames[i].lastUsedTime < earliestTime) {
            lruIndex = i;
            earliestTime = frames[i].lastUsedTime;
        }
    }

    return lruIndex;
}

void LRUPageReplacement(int pages[]) {
    PageFrame frames[NUMBER_OF_FRAMES];
    initializePageFrames(frames);

    int pageFaults = 0;
    int time = 0; // To keep track of the time each page was last used

    for (int i = 0; i < PAGE_SEQ_LEN; i++) {
        int currentPage = pages[i];
        int found = 0;

        // Check if the page is already in one of the frames
        for (int j = 0; j < NUMBER_OF_FRAMES; j++) {
            if (frames[j].number == currentPage) {
                frames[j].lastUsedTime = time++; // Update the last used time
                found = 1;
                break;
            }
        }

        // If the page was not found in the frames
        if (!found) {
            int lruIndex = findLRUPageFrameIndex(frames);
            frames[lruIndex].number = currentPage; // Replace the LRU page with the new page
            frames[lruIndex].lastUsedTime = time++; // Update the last used time
            pageFaults++;
        }
        printf("Processing page %d: ", currentPage);
        printFrames(frames);
    }

    printf("Total Page Faults: %d\n", pageFaults);
}

int LRUDemo() {
    int pages[PAGE_SEQ_LEN] = {1, 2, 3, 4, 1, 2, 5, 1, 2, 3, 4, 5};

    printf("Starting LRU Page Replacement Simulation\n");
    LRUPageReplacement(pages);

    return 0;
}
