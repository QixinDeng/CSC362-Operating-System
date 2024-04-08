/*
    Name: Dr. Qixin Deng
    Date: February 28, 2024
    Description:
    main function to test.

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

#define PAGE_SEQ_LEN 12
#define NUMBER_OF_FRAMES 4

// Function to check if a page is already in a frame
bool isPageInFrames(int page, int frames[]) {
    for (int i = 0; i < NUMBER_OF_FRAMES; i++) {
        if (frames[i] == page) {
            return true;
        }
    }
    return false;
}

void printFramesFifo(int frames[]) {
    // Print the current state of frames
    for (int j = 0; j < NUMBER_OF_FRAMES; j++) {
        if (frames[j] != -1) {
            printf("%d ", frames[j]);
        } else {
            printf("_ ");
        }
    }
    printf("\n");
}

// Function to implement FIFO page replacement
int fifo(int pages[], int frames[]) {
    int pageFaults = 0;
    int pageInsertIndex = 0;

    // Initialize frames to -1, indicating they're initially empty
    for (int i = 0; i < NUMBER_OF_FRAMES; i++) {
        frames[i] = -1;
    }

    for (int i = 0; i < PAGE_SEQ_LEN; i++) {
        int currentPage = pages[i];

        // If the current page is not in frames, we have a page fault
        if (!isPageInFrames(currentPage, frames)) {
            // Replace the oldest page with the current page
            frames[pageInsertIndex] = currentPage;

            // Move to the next frame index, wrapping around if necessary
            pageInsertIndex = (pageInsertIndex + 1) % NUMBER_OF_FRAMES;

            // Increase the count of page faults
            pageFaults++;
        }
        printf("Processing page %d: ", currentPage);
        printFramesFifo(frames);
    }

    return pageFaults;
}

int FIFODemo() {
    int pages[PAGE_SEQ_LEN] = {2, 3, 2, 1, 5, 2, 4, 5, 3, 2, 5, 2};
    int frames[NUMBER_OF_FRAMES];

    int pageFaults = fifo(pages, frames);

    printf("Total Page Faults: %d\n", pageFaults);
    return 0;
}