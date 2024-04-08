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

#define PAGE_SEQ_LEN 12
#define NUMBER_OF_FRAMES 4

typedef struct {
    int pageNumber;
    int useBit;
} Frame;

void initializeFrames(Frame frames[]) {
    for (int i = 0; i < NUMBER_OF_FRAMES; i++) {
        frames[i].pageNumber = -1; // -1 indicates that the frame is empty
        frames[i].useBit = 0; // Initialize use bit to 0
    }
}

int findFrame(Frame frames[], int pageNumber) {
    for (int i = 0; i < NUMBER_OF_FRAMES; i++) {
        if (frames[i].pageNumber == pageNumber) {
            return i; // Return index of the frame if page is found
        }
    }
    return -1; // Return -1 if the page is not found in any frame
}

void displayFrames(Frame frames[]) {
    printf("Frames: ");
    for (int i = 0; i < NUMBER_OF_FRAMES; i++) {
        if (frames[i].pageNumber != -1) {
            printf("%d(%d) ", frames[i].pageNumber, frames[i].useBit);
        } else {
            printf("Empty ");
        }
    }
    printf("\n");
}

void clockPageReplacement(int pages[], Frame frames[]) {
    initializeFrames(frames);

    int pointer = 0; // Points to the oldest frame
    int pageFaults = 0;

    for (int i = 0; i < PAGE_SEQ_LEN; i++) {
        int page = pages[i];
        printf("Processing page: %d\n", page);
        int frameIndex = findFrame(frames, page);

        if (frameIndex == -1) { // Page fault
            while (frames[pointer].useBit == 1) {
                frames[pointer].useBit = 0; // Clear the use bit
                pointer = (pointer + 1) % NUMBER_OF_FRAMES; // Move the pointer to the next frame
            }
            // Replace the page at pointer with the new page
            frames[pointer].pageNumber = page;
            frames[pointer].useBit = 1; // Set the use bit
            pointer = (pointer + 1) % NUMBER_OF_FRAMES; // Move the pointer to the next frame

            pageFaults++;
        } else {
            frames[frameIndex].useBit = 1; // Set the use bit since the page was accessed
        }

        displayFrames(frames);
    }

    printf("\nTotal page faults: %d\n", pageFaults);
}

int clockDemo() {
    int pages[PAGE_SEQ_LEN] = {2, 3, 2, 1, 5, 2, 4, 5, 3, 2, 5, 2};
    Frame frames[NUMBER_OF_FRAMES];

    printf("Clock Page Replacement Algorithm Simulation\n\n");
    clockPageReplacement(pages, frames);

    return 0;
}
