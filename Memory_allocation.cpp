#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <fstream>
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
int firstFit(Block memory[], int memorySize, int requestSize, int& lastAllocated) {
    for (int i = 0; i < memorySize; i++) {
        if (memory[i].size >= requestSize) {
            memory[i].size -= requestSize;
            return i;
        }
    }
    return -1; // Allocation failed
}

// Function to perform memory allocation using Next-Fit strategy
int nextFit(Block memory[], int memorySize, int requestSize, int &lastAllocated) {
    static int currentIndex = 0;

    for (int i = 0; i < memorySize; i++) {
        int index = (currentIndex + i) % memorySize;

        if (memory[index].size >= requestSize) {
            memory[index].size -= requestSize;
            lastAllocated = index;
            currentIndex = index;  // Update currentIndex for the next iteration
            return index;
        }
    }

    return -1; // Allocation failed
}

// Function to perform memory allocation using Best-Fit strategy
int bestFit(Block memory[], int memorySize, int requestSize, int& lastAllocated) {
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
int worstFit(Block memory[], int memorySize, int requestSize, int& lastAllocated) {
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
        result = average + stddev * cos(2 * M_PI * static_cast<double>(rand()) / RAND_MAX) * sqrt(-2 * log(static_cast<double>(rand()) / RAND_MAX));
    } while (result < 1 || result > MEMORY_SIZE - 1);

    return static_cast<int>(result);
}

// Simulation loop
double simulationLoop(Block memory[], int memorySize, double averageRequestSize, double stddevRequestSize, int (*allocationStrategy)(Block[], int, int, int&), int simulationSteps) {
    int holesExamined = 0;
    int requestCount = 0;
    int lastAllocated = 0;

    for (int step = 0; step < simulationSteps; step++) {
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
    return (requestCount > 0) ? static_cast<double>(holesExamined) / requestCount : 0.0;
}

// Function to calculate memory utilization
double calculateMemoryUtilization(Block memory[], int memorySize) {
    int totalAllocatedSize = 0;
    for (int i = 0; i < memorySize; i++) {
        if (memory[i].size > 0) {
            totalAllocatedSize += memory[i].size;
        }
    }
    return static_cast<double>(totalAllocatedSize) / memorySize;
}

int main() {
    Block memory[MEMORY_SIZE];

    // Initialization
    initializeMemory(memory, MEMORY_SIZE);

    // Set simulation parameters
    int simulationSteps = 100;  // Replace with your desired number of simulation steps

    std::ofstream outFile("simulation_data.txt");

    // Run the simulation for each allocation strategy
    for (int i = 0; i < 10; i++) {  // Replace 10 with the number of simulations you want to run
        double averageRequestSize = rand() % 50 + 1;  // Random average request size between 1 and 50
        double stddevRequestSize = rand() % 10 + 1;  // Random standard deviation between 1 and 10

        double avgHolesExaminedFirstFit = simulationLoop(memory, MEMORY_SIZE, averageRequestSize, stddevRequestSize, &firstFit, simulationSteps);
        double memoryUtilizationFirstFit = calculateMemoryUtilization(memory, MEMORY_SIZE);

        initializeMemory(memory, MEMORY_SIZE);  // Reset memory

        double avgHolesExaminedNextFit = simulationLoop(memory, MEMORY_SIZE, averageRequestSize, stddevRequestSize, &nextFit, simulationSteps);
        double memoryUtilizationNextFit = calculateMemoryUtilization(memory, MEMORY_SIZE);

        initializeMemory(memory, MEMORY_SIZE);  // Reset memory

        double avgHolesExaminedBestFit = simulationLoop(memory, MEMORY_SIZE, averageRequestSize, stddevRequestSize, &bestFit, simulationSteps);
        double memoryUtilizationBestFit = calculateMemoryUtilization(memory, MEMORY_SIZE);

        initializeMemory(memory, MEMORY_SIZE);  // Reset memory

        double avgHolesExaminedWorstFit = simulationLoop(memory, MEMORY_SIZE, averageRequestSize, stddevRequestSize, &worstFit, simulationSteps);
        double memoryUtilizationWorstFit = calculateMemoryUtilization(memory, MEMORY_SIZE);

        // Output results
        std::cout << "Simulation " << i+1 << std::endl;
        std::cout << "Average Request Size: " << averageRequestSize << ", Standard Deviation: " << stddevRequestSize << std::endl;
        std::cout << "First-Fit - Average Holes Examined: " << avgHolesExaminedFirstFit << ", Memory Utilization: " << memoryUtilizationFirstFit << std::endl;
        std::cout << "Next-Fit - Average Holes Examined: " << avgHolesExaminedNextFit << ", Memory Utilization: " << memoryUtilizationNextFit << std::endl;
        std::cout << "Best-Fit - Average Holes Examined: " << avgHolesExaminedBestFit << ", Memory Utilization: " << memoryUtilizationBestFit << std::endl;
        std::cout << "Worst-Fit - Average Holes Examined: " << avgHolesExaminedWorstFit << ", Memory Utilization: " << memoryUtilizationWorstFit << std::endl;
        std::cout << "----------------------------------------" << std::endl;

        outFile << i << " " << avgHolesExaminedFirstFit << " " << memoryUtilizationFirstFit << " "
                << avgHolesExaminedNextFit << " " << memoryUtilizationNextFit << " "
                << avgHolesExaminedBestFit << " " << memoryUtilizationBestFit << " "
                << avgHolesExaminedWorstFit << " " << memoryUtilizationWorstFit << std::endl;
    }

    outFile.close();

    return 0;
}