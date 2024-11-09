#include "includes.h"

// Constants
const int MAX_QUEUE_SIZE = 100;
const int MAX_FILES = 100;
const size_t MAX_CONTENT_SIZE = 1024;

// File structure
struct File
{
    std::string filename;
    std::string content;
    bool in_use = false;
    std::time_t creation_time;
};

// Filesystem storage
std::vector<File> filesystem(MAX_FILES);

// Task structure for thread pool
struct Task
{
    std::function<void()> function;
};

// Task queue management
std::queue<Task> task_queue;
std::mutex queue_mutex;
std::condition_variable task_available;
bool stop_threads = false;
int counter;

// Thread pool variables
std::vector<std::thread> threads;
int thread_count;

// Enqueue task to task queue
void enqueueTask(const Task &task)
{
    std::unique_lock<std::mutex> lock(queue_mutex);
    if (task_queue.size() < MAX_QUEUE_SIZE)
    {
        task_queue.push(task);
        task_available.notify_one();
    }
}

// Dequeue task from task queue
Task dequeueTask()
{
    std::unique_lock<std::mutex> lock(queue_mutex);
    task_available.wait(lock, []
                        { return !task_queue.empty() || stop_threads; });
    if (stop_threads)
    {
        return {NULL};
    }
    Task task = task_queue.front();
    task_queue.pop();
    return task;
}

// Worker function for threads
void worker()
{
    while (true)
    {
        Task task = dequeueTask();
        if (!task.function)
        {
            break;
        }
        task.function();
        counter--;
    }
}

// Initialize thread pool
void initializeThreadPool(int n)
{
    thread_count = n;
    counter = 0;
    for (int i = 0; i < thread_count; ++i)
    {
        threads.emplace_back(worker);
    }
}

// Cleanup thread pool
void cleanupThreadPool()
{
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop_threads = true;
        task_available.notify_all();
    }
    for (auto &thread : threads)
    {
        if (thread.joinable())
        {
            thread.join();
        }
    }
    threads.clear();
}

// Find a file by filename
File *findFile(const std::string &filename)
{
    for (auto &file : filesystem)
    {
        if (file.in_use && file.filename == filename)
        {
            return &file;
        }
    }
    return NULL;
}

// Find an empty slot in the filesystem
File *findEmptyFileSlot()
{
    for (auto &file : filesystem)
    {
        if (!file.in_use)
        {
            return &file;
        }
    }
    return NULL;
}

// Check if thread pool is empty
bool isPoolEmpty()
{
    return task_queue.empty();
}