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
#include <math.h>

#define MAX_MEM_SIZE 1024 // Maximum memory size
#define MIN_BLOCK_SIZE 32 // Minimum block size

typedef struct Block {
    int size;
    int isFree;
    struct Block *next;
    struct Block *prev;
    struct Block *buddy;
} Block;

Block *head = NULL;

// Initialize memory with a single large block
void initializeMemory() {
    head = (Block *)malloc(sizeof(Block));
    head->size = MAX_MEM_SIZE;
    head->isFree = 1;
    head->next = head->prev = head->buddy = NULL;
}

// Allocate memory using the Buddy System
void *buddySplit(int size) {
    Block *current = head;
    int adjustedSize = (int)pow(2, ceil(log(size) / log(2))); // Adjust size to next power of 2

    if (adjustedSize < MIN_BLOCK_SIZE) adjustedSize = MIN_BLOCK_SIZE;

    while (current) {
        if (current->isFree && current->size >= adjustedSize) {
            while (current->size / 2 >= size) {
                Block *buddy = (Block *)malloc(sizeof(Block));
                buddy->size = current->size / 2;
                current->size /= 2;

                buddy->next = current->next;
                buddy->prev = current;
                buddy->buddy = current;
                buddy->isFree = 1;

                if (current->next) current->next->prev = buddy;
                current->next = buddy;
                current->buddy = buddy;
            }
            current->isFree = 0;
            return current;
        }
        current = current->next;
    }
    return NULL; // No suitable block found
}

// Merge buddies if possible
void buddyMerge(void *ptr) {
    Block *block = (Block *)ptr; // Get block metadata from memory address
    if (block->buddy && block->buddy->isFree && block->size == block->buddy->size) {
        block->isFree = 1;
        block->next = block->buddy->next;
        if (block->buddy->next) block->buddy->next->prev = block->buddy->prev;

        block->size *= 2;
        free(block->buddy);
        block->buddy = block->prev;
        block->isFree = 1;

        buddyMerge(block); // Recursively try to merge with its buddy
    }
}

void printMemoryTree() {
    Block *current = head;
    printf("Memory Blocks:\n");
    while (current != NULL) {
        printf("Block Size: %d, Status: %s\n", current->size, current->isFree ? "Free" : "Allocated");
        current = current->next;
    }
}

int buddySys() {
    initializeMemory();

    // Example usage
    void *ptr1 = buddySplit(100);
    void *ptr2 = buddySplit(200);

    // Print the memory tree
    printMemoryTree(head, 0);
    if(ptr1 != NULL){
        buddyMerge(ptr1);
        printMemoryTree(head, 0);
    }
    if(ptr2 != NULL){
        buddyMerge(ptr2);
        printMemoryTree(head, 0);
    }


    return 0;
}
