#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

// Function to generate request sizes from a normal distribution
int generateRequestSize(double average, double stddev) {
    double result;
    do {
        result = average + stddev * cos(2 * M_PI * rand() / RAND_MAX) * sqrt(-2 * log((double)rand() / RAND_MAX));
    } while (result < 1 || result > MEMORY_SIZE - 1);

    return (int)result;
}

// Simulation loop
double simulationLoop(Block memory[], int memorySize, double averageRequestSize, double stddevRequestSize) {
    int holesExamined = 0;
    int requestCount = 0;

    while (1) {
        // Choose a random number s for the request size
        int requestSize = generateRequestSize(averageRequestSize, stddevRequestSize);

        // Attempt to allocate memory using one of the allocation strategies
        int allocatedBlock = firstFit(memory, MEMORY_SIZE, requestSize);
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

    // Run the simulation
    double averageHolesExamined = 0.0;
    for (int step = 0; step < simulationSteps; step++) {
        averageHolesExamined += simulationLoop(memory, MEMORY_SIZE, averageRequestSize, stddevRequestSize);
    }

    // Calculate and print the average number of holes examined
    if (simulationSteps > 0) {
        averageHolesExamined /= simulationSteps;
        printf("Average Holes Examined: %lf\n", averageHolesExamined);
    } else {
        printf("Simulation steps must be greater than zero.\n");
    }

    return 0;
}
