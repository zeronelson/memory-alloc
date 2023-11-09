#include <iostream>
#include <cstdlib>
#include <cmath>

using namespace std;

// Constants
#define MEMORY_SIZE 100

// Data structure to represent a block in memory
struct Block {
    int size;  // Block size (positive for allocated, negative for free)
};

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
int nextFit(Block memory[], int memorySize, int requestSize, int& lastAllocated) {
    for (int i = lastAllocated; i < memorySize; i++) {
        if (memory[i].size >= requestSize) {
            memory[i].size -= requestSize;
            lastAllocated = i;
            return i;
        }
    }

    for (int i = 0; i < lastAllocated; i++) {
        if (memory[i].size >= requestSize) {
            memory[i].size -= requestSize;
            lastAllocated = i;
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

// Function to generate request sizes from a normal distribution
int generateRequestSize(double average, double stddev) {
    double result;
    do {
        result = average + stddev * cos(2 * M_PI * rand() / RAND_MAX) * sqrt(-2 * log((double)rand() / RAND_MAX));
    } while (result < 1 || result > MEMORY_SIZE - 1);

    return (int)result;
}

// Updated simulation loop
double simulationLoop(Block memory[], int memorySize, double averageRequestSize, double stddevRequestSize, int (*allocationStrategy)(Block[], int, int, int&)) {
    int holesExamined = 0;
    int requestCount = 0;
    int lastAllocated = 0; // Initialize lastAllocated

    while (true) {
        // Choose a random number s for the request size
        int requestSize = generateRequestSize(averageRequestSize, stddevRequestSize);

        // Attempt to allocate memory using the specified allocation strategy
        int allocatedBlock = allocationStrategy(memory, MEMORY_SIZE, requestSize, lastAllocated);
        if (allocatedBlock != -1) {
            // Allocation succeeded
            holesExamined += allocatedBlock + 1; // Count the holes examined for this request
            requestCount++;
        } else {
            // Allocation failed, exit the loop
            break;
        }
    }

    // Calculate and return the average number of holes examined
    return (requestCount > 0) ? (double)holesExamined / requestCount : 0.0;
}

int main() {
    Block memory[MEMORY_SIZE];

    // Initialization
    initializeMemory(memory, MEMORY_SIZE);

    // Set simulation parameters
    int simulationSteps = 100;  // Replace with your desired number of simulation steps
    double averageRequestSize = 10.0;  // Replace with your desired average request size
    double stddevRequestSize = 3.0;  // Replace with your desired standard deviation for request size

    // Run the simulation for each allocation strategy
    cout << "First-Fit - Average Holes Examined: " << simulationLoop(memory, MEMORY_SIZE, averageRequestSize, stddevRequestSize, &firstFit) << endl;
    cout << "Next-Fit - Average Holes Examined: " << simulationLoop(memory, MEMORY_SIZE, averageRequestSize, stddevRequestSize, &nextFit) << endl;
    cout << "Best-Fit - Average Holes Examined: " << simulationLoop(memory, MEMORY_SIZE, averageRequestSize, stddevRequestSize, &bestFit) << endl;
    cout << "Worst-Fit - Average Holes Examined: " << simulationLoop(memory, MEMORY_SIZE, averageRequestSize, stddevRequestSize, &worstFit) << endl;

    return 0;
}

