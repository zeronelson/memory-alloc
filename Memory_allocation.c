#include <stdio.h>
#include <stdlib.h>

// Constants
#define MEMORY_SIZE 100

// Data structure to represent a block in memory
typedef struct Block {
    int size;  // Block size (positive for allocated, negative for free)
} Block;

// Function to initialize memory with random block sizes
void initializeMemory(Block memory[], int memorySize) {
    for (int i = 0; i < memorySize; i++) {
        int size = rand() % (memorySize / 2) + 1; // Random size, max half of memory
        memory[i].size = size;
    }
}

// Function to perform memory allocation using First-Fit strategy
int firstFit(Block memory[], int memorySize, int requestSize) {
    for (int i = 0; i < memorySize; i++) {
        if (memory[i].size >= requestSize) {
            memory[i].size -= requestSize;
            return i;
        }
    }
    return -1; // Allocation failed
}

// Function to perform memory allocation using Next-Fit strategy
int nextFit(Block memory[], int memorySize, int requestSize, int *lastAllocated) {
    for (int i = *lastAllocated; i < memorySize; i++) {
        if (memory[i].size >= requestSize) {
            memory[i].size -= requestSize;
            *lastAllocated = i;
            return i;
        }
    }

    for (int i = 0; i < *lastAllocated; i++) {
        if (memory[i].size >= requestSize) {
            memory[i].size -= requestSize;
            *lastAllocated = i;
            return i;
        }
    }

    return -1; // Allocation failed
}

// Function to perform memory allocation using Best-Fit strategy
int bestFit(Block memory[], int memorySize, int requestSize) {
    int bestFitIdx = -1;
    int bestFitSize = MEMORY_SIZE + 1;

    for (int i = 0; i < memorySize; i++) {
        if (memory[i].size >= requestSize && memory[i].size - requestSize < bestFitSize) {
            bestFitIdx = i;
            bestFitSize = memory[i].size - requestSize;
        }
    }

    if (bestFitIdx != -1) {
        memory[bestFitIdx].size -= requestSize;
        return bestFitIdx;
    }

    return -1; // Allocation failed
}

// Function to perform memory allocation using Worst-Fit strategy
int worstFit(Block memory[], int memorySize, int requestSize) {
    int worstFitIdx = -1;
    int worstFitSize = -1;

    for (int i = 0; i < memorySize; i++) {
        if (memory[i].size >= requestSize && memory[i].size - requestSize > worstFitSize) {
            worstFitIdx = i;
            worstFitSize = memory[i].size - requestSize;
        }
    }

    if (worstFitIdx != -1) {
        memory[worstFitIdx].size -= requestSize;
        return worstFitIdx;
    }

    return -1; // Allocation failed
}

int main() {
    Block memory[MEMORY_SIZE];
    int lastAllocated = 0;

    // Initialization
    initializeMemory(memory, MEMORY_SIZE);

    // Example allocation
    int requestSize = 10;
    int allocatedBlock = firstFit(memory, MEMORY_SIZE, requestSize);
    if (allocatedBlock != -1) {
        printf("Allocated block at index %d\n", allocatedBlock);
    } else {
        printf("Allocation failed.\n");
    }

    return 0;
}
