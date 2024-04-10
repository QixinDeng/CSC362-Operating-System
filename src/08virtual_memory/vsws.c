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
#include <stdlib.h>

#define MAX_PAGES 400
#define MAX_FRAMES 20

typedef struct {
    int pageNumber;
    int useBit;
} Frame;

void initializeFramesVSWS(Frame frames[], int size) {
    for (int i = 0; i < size; i++) {
        frames[i].pageNumber = -1; // Indicates an empty frame
        frames[i].useBit = 0;
    }
}

int findPage(int pageNumber, Frame frames[], int size) {
    for (int i = 0; i < size; i++) {
        if (frames[i].pageNumber == pageNumber) {
            return i; // Page found
        }
    }
    return -1; // Page not found
}

void simulateVSWS(int pages[], int M, int L, int Q) {
    Frame frames[MAX_FRAMES];
    initializeFramesVSWS(frames, MAX_FRAMES);

    int frameCount = 0;
    int pageFaults = 0;
    int lastSampleTime = 0;
    int currentTime = 0;

    for (currentTime = 0; currentTime < MAX_PAGES; currentTime++) {
        int pageIndex = findPage(pages[currentTime], frames, frameCount);

        if (pageIndex == -1) { // Page fault
            pageFaults++;
            if (frameCount < MAX_FRAMES) {
                frames[frameCount].pageNumber = pages[currentTime];
                frames[frameCount].useBit = 1;
                frameCount++;
            }
        } else {
            frames[pageIndex].useBit = 1; // Mark the page as used
        }

        // Check the VSWS policy conditions
        if ((currentTime - lastSampleTime == L) || (pageFaults == Q)) {
            // Scan use bits and adjust the resident set
            for (int i = 0; i < frameCount; i++) {
                if (frames[i].useBit == 0) {
                    frames[i] = frames[--frameCount]; // Remove the page
                    i--; // Adjust loop index after removal
                } else {
                    frames[i].useBit = 0; // Reset the use bit for the next interval
                }
            }


            printf("Sample at time %d, resident set size: %d\n", currentTime, frameCount);
            lastSampleTime = currentTime;
            pageFaults = 0; // Reset page faults for the next interval
        }
    }
}

int VSWSDemo() {
    int pages[MAX_PAGES];
    // Generate a sequence of page references
    for (int i = 0; i < MAX_PAGES; i++) {
        pages[i] = rand() % 15; // Assume 15 different pages
    }

    int M = 10; // Minimum duration of the sampling interval
    int L = 20; // Maximum duration of the sampling interval
    int Q = 11;  // Allowed page faults between sampling instances

    printf("Starting VSWS algorithm simulation...\n");
    simulateVSWS(pages, M, L, Q);

    return 0;
}
