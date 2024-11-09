#include "includes.h"

int readFile(const std::string &filename, bool benchmark)
{
    std::lock_guard<std::mutex> lock(queue_mutex);
    File *file = findFile(filename);
    if (!file)
    {
        if (!benchmark)
        {
            std::cerr << "Error: File '" << filename << "' not found.\n";
        }
        return -1;
    }
    if (!benchmark)
    {
        std::cout << "Reading file '" << filename << "': " << file->content << '\n';
    }
    return 0;
}