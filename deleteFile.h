#include "includes.h"

int deleteFile(const std::string &filename, bool benchmark)
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
    file->in_use = false;
    if (!benchmark)
    {
        std::cout << "File '" << filename << "' deleted.\n";
    }
    return 0;
}
