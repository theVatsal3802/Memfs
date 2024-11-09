#include "includes.h"

int listFiles(bool long_format, bool benchmark)
{
    std::lock_guard<std::mutex> lock(queue_mutex);
    if (!benchmark)
    {
        std::cout << "Listing files:\n";
    }
    for (const auto &file : filesystem)
    {
        if (file.in_use)
        {
            if (!benchmark)
            {
                long_format ? std::cout << "- " << file.filename << "\tSize: " << file.content.size()
                                        << " bytes\tCreated: " << std::ctime(&file.creation_time)
                            : std::cout << "- " << file.filename << '\n';
            }
        }
    }
    return 0;
}