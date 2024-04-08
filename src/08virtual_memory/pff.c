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

#define MAX_PAGES 200
#define MAX_FRAMES 10
#define UPPER_PFF_LIMIT 7
#define LOWER_PFF_LIMIT 4
#define ADJUSTMENT_INTERVAL 10  // Interval after which the frame count can be adjusted

// Function to check if a page is in the set
int isInMemory(int page, int frames[], int frameCount) {
    for (int i = 0; i < frameCount; i++) {
        if (frames[i] == page) {
            return 1; // Page is in memory
        }
    }
    return 0; // Page is not in memory
}

// PFF algorithm simulation
void simulatePFF(int pages[]) {
    int frames[MAX_FRAMES] = {0};
    int frameCount = 3; // Starting frame count
    int pageFaults = 0;
    int referenceCounter = 0;

    for (int i = 0; i < MAX_PAGES; i++) {
        referenceCounter++;
        if (!isInMemory(pages[i], frames, frameCount)) {
            // Page fault occurred
            pageFaults++;
            printf("Time: %d; Page fault for page %d\n", i, pages[i]);
        }
        else{
            printf("Time %d; Page %d hit!\n", i, pages[i]);
        }

        // Simulate adding the page to the set (in the simplest case, replace the oldest one)
        if (!isInMemory(pages[i], frames, frameCount)) {
            frames[(i % frameCount)] = pages[i];
        }

        // Check if we need to adjust the frame count after every ADJUSTMENT_INTERVAL references
        if (referenceCounter % ADJUSTMENT_INTERVAL == 0) {
            if (pageFaults >= UPPER_PFF_LIMIT && frameCount < MAX_FRAMES) {
                frameCount++; // Increase the frame count
                printf("Increasing frame count to %d due to high page fault frequency.\n", frameCount);
            } else if (pageFaults <= LOWER_PFF_LIMIT && frameCount > 1) {
                frameCount--; // Decrease the frame count
                printf("Decreasing frame count to %d due to low page fault frequency.\n", frameCount);
            }
            // Reset page fault count for the next interval
            pageFaults = 0;
        }

    }
}

int PFFDemo() {
    int pages[MAX_PAGES];
    // Generate a sequence of page references to trigger both increase and decrease in frame count
    for (int i = 0; i < MAX_PAGES; i++) {
        if (i % 40 < 20) {
            // Alternate between a narrow range and a wider range of pages
            pages[i] = rand() % 5;
        } else {
            pages[i] = rand() % 10 + 5;
        }
    }

    printf("Starting PFF algorithm simulation...\n");
    simulatePFF(pages);

    return 0;
}

