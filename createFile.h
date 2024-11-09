#include "includes.h"

int createFile(const std::string &filename, bool benchmark)
{
    std::lock_guard<std::mutex> lock(queue_mutex);
    if (findFile(filename))
    {
        if (!benchmark)
        {
            std::cerr << "Error: File '" << filename << "' already exists.\n";
        }
        return -1;
    }
    File *file = findEmptyFileSlot();
    if (!file)
    {
        if (!benchmark)
        {
            std::cerr << "Error: Filesystem is full.\n";
        }
        return -1;
    }
    file->filename = filename;
    file->content.clear();
    file->in_use = true;
    file->creation_time = std::time(NULL);
    if (!benchmark)
    {
        std::cout << "File '" << filename << "' created.\n";
    }
    return 0;
}