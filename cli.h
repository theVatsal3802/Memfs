#include "createFile.h"
#include "writeFile.h"
#include "deleteFile.h"
#include "readFile.h"
#include "listFiles.h"

#include <fstream>

// Command-line interface for interaction
void commandLineInterface(bool benchmark = false)
{
    std::string command;
    std::ifstream fin;
    fin.open("benchmark.txt");
    if (benchmark && !fin.is_open())
    {
        std::cout << "Could not find or open benchmark.txt\n";
    }
    while (true)
    {
        while (counter > 0)
            ;
        if (!benchmark)
        {
            std::cout << "memfs> ";
            std::getline(std::cin, command);
        }
        else
        {
            if (!std::getline(fin, command))
            {
                break;
            }
        }
        if (command.empty() && !benchmark)
        {
            continue;
        }

        size_t pos = command.find(' ');
        std::string cmd = (pos == std::string::npos) ? command : command.substr(0, pos);
        std::string args = (pos == std::string::npos) ? "" : command.substr(pos + 1);

        if (cmd == "create")
        {
            if (args.find("-n") == 0)
            {
                size_t num_pos = args.find(' ', 3);
                int count = std::stoi(args.substr(3, num_pos - 3));
                counter = count;
                size_t start = num_pos + 1;
                for (int i = 0; i < count; i++)
                {
                    size_t end = args.find(' ', start);
                    std::string filename = args.substr(start, end - start);
                    start = end + 1;
                    enqueueTask({[=]
                                 { createFile(filename, benchmark); }});
                }
            }
            else
            {
                counter = 1;
                enqueueTask({[=]
                             { createFile(args, benchmark); }});
            }
        }
        else if (cmd == "write")
        {
            if (args.find("-n") == 0)
            {
                size_t num_pos = args.find(' ', 3);
                int count = std::stoi(args.substr(3, num_pos - 3));
                counter = count;
                size_t start = num_pos + 1;
                for (int i = 0; i < count; i++)
                {
                    size_t end = args.find(' ', start);
                    std::string filename = args.substr(start, end - start);
                    start = end + 1;
                    end = args.find('"', start + 1);
                    std::string content = args.substr(start + 1, end - start - 1);
                    start = end + 2; // Move past the closing quote and space
                    enqueueTask({[=]
                                 { writeFile(filename, content, benchmark); }});
                }
            }
            else
            {
                size_t content_pos = args.find(' ');
                std::string filename = args.substr(0, content_pos);
                std::string content = args.substr(content_pos + 1);
                counter = 1;
                enqueueTask({[=]
                             { writeFile(filename, content, benchmark); }});
            }
        }
        else if (cmd == "delete")
        {
            if (args.find("-n") == 0)
            {
                size_t num_pos = args.find(' ', 3);
                int count = std::stoi(args.substr(3, num_pos - 3));
                counter = count;
                size_t start = num_pos + 1;
                for (int i = 0; i < count; i++)
                {
                    size_t end = args.find(' ', start);
                    std::string filename = args.substr(start, end - start);
                    start = end + 1;
                    enqueueTask({[=]
                                 { deleteFile(filename, benchmark); }});
                }
            }
            else
            {
                counter = 1;
                enqueueTask({[=]
                             { deleteFile(args, benchmark); }});
            }
        }
        else if (cmd == "read")
        {
            counter = 1;
            enqueueTask({[=]
                         { readFile(args, benchmark); }});
        }
        else if (cmd == "ls")
        {
            counter = 1;
            bool long_format = (args == "-l");
            enqueueTask({[=]
                         { listFiles(long_format, benchmark); }});
        }
        else if (cmd == "exit")
        {
            counter = 0;
            break;
        }
        else
        {
            counter = 0;
            std::cerr << "Unknown command: " << cmd << '\n';
        }
    }
    if (benchmark)
    {
        fin.close();
    }
}