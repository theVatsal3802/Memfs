#include "includes.h"

int writeFile(const std::string &filename, const std::string &content, bool benchmark)
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
    file->content = content.substr(0, MAX_CONTENT_SIZE - 1);
    if (!benchmark)
    {
        std::cout << "Content written to file '" << filename << "'.\n";
    }
    return 0;
}