#include "threadPool.h"
#include "cli.h"

int main(int argc, char *argv[])
{
    bool benchmark = false;
    auto start = std::chrono::high_resolution_clock::now();
    if (argc == 2)
    {
        std::cout << "Entering benchmarking mode\n";
        benchmark = true;
        std::cout << "Starting timer\n";
        std::cout << "\nExecuting all commands... \nPlease wait...\n";
        start = std::chrono::high_resolution_clock::now();
    }
    int n = 8;

    initializeThreadPool(n);
    if (!benchmark)
    {
        std::cout << "Welcome to memFS!\n";
    }
    commandLineInterface(benchmark);
    cleanupThreadPool();
    if (benchmark)
    {
        auto end = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
        std::cout << "\n\nTime taken to execute benchmarking commands: " << duration.count() << " milliseconds.\n";
    }
    return 0;
}
