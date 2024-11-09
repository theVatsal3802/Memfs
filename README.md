# Design Document: memFS (In-Memory File System)

## Overview

**memFS** is a command-line-based, temporary file storage system that operates in RAM. It supports commands for file manipulation and uses multi-threading to enhance performance.

## Key Requirements

1. **File Commands**: Create, write, delete, read, and list files with parameters.
2. **Concurrency**: Multi-threaded processing for `create`, `write`, and `delete` commands.
3. **Efficient Data Storage**: In-memory structures for rapid data access.
4. **Error Handling**: Manage errors and batch processing for commands.

---

## 1. Data Structures

### File Structure

Each file in memFS includes metadata and content:

- **Name**: Unique file identifier.
- **Size**: File size in bytes.
- **Content**: Stores file data.
- **Creation Date**: Timestamp of creation.
- **Last Modified Date**: Updated on each modification.

### File System Structure

Manages all files in memory:

- **File Table**: A map (`std::unordered_map`) stores files by name.
- **Mutex for Concurrency**: Protects shared resources during operations.

---

## 2. Command Logic

### 2.1. Create Command

Creates files with optional multi-threading.

- **Syntax**: `create [-n count] filenames`
- **Steps**:
  - Parse filenames, check for duplicates, allocate memory, and add to the file table.
  - Spawn threads for multiple files.

### 2.2. Write Command

Writes data to files.

- **Syntax**: `write [-n count] filename "text"`
- **Steps**:
  - Parse filenames, verify existence, write data, and update metadata.
  - Supports multi-threaded writing.

### 2.3. Delete Command

Deletes files.

- **Syntax**: `delete [-n count] filenames`
- **Steps**:
  - Parse filenames, check existence, remove files from the file table, and free memory.
  - Supports multi-threaded deletion.

### 2.4. Read Command

Displays file content.

- **Syntax**: `read filename`
- **Steps**:
  - Parse filename, verify existence, and display content.

### 2.5. List Command (ls)

Lists files, with optional details.

- **Syntax**: `ls` or `ls -l`
- **Steps**:
  - List all filenames and display metadata if `-l` flag is set.

### 2.6. Exit Command

Terminates memFS.

- **Syntax**: `exit`
- **Steps**:
  - Free memory, close threads, and clean up resources.

---

## 3. Concurrency and Synchronization

### 3.1. Threading Model

`create`, `write`, and `delete` commands use threads for concurrent file processing.

### 3.2. Shared Resource Management

- **Mutex Locks**: `fs_mutex` locks file table modifications.
- **Atomic Operations**: For shared counters to prevent race conditions.

### 3.3. Memory Management

- Dynamic allocation for file content.
- Resource cleanup for `delete` and `exit` commands.

---

## 4. Error Handling and Edge Cases

### Error Messages

- Display errors for duplicate creation, non-existent file access, and invalid syntax.

### Edge Cases

- **Duplicate Creation**: Creating a file with an existing name.
- **Multi-Threaded Errors**: Handling conflicting operations.
- **Memory Limitations**: Return error if out of memory.

---

## 5. Command-Line Interpreter Design

Parses and validates each command, managing multi-threaded execution where needed.

---

## Benchmarking Statistics

The following benchmarking steps have been performed on the `memFS` file system. The time in the table is in milliseconds.

|             | `100 Commands` | `1000 Commands` | `10000 Commands` | `100000 Commands` |
| ----------- | -------------- | --------------- | ---------------- | ----------------- |
| `1 Thread`  | 13ms           | 338ms           | 3447ms           | 14538ms           |
| `2 Thread`  | 37ms           | 213ms           | 3295ms           | 21109ms           |
| `4 Thread`  | 133ms          | 355ms           | 3429ms           | 26494ms           |
| `8 Thread`  | 66ms           | 324ms           | 3256ms           | 17756ms           |
| `16 Thread` | 99ms           | 357ms           | 2613ms           | 24596ms           |

- The CPU utilization during each of these benchmarking steps is 47% on average.
- The memory utilization is also around 40% during the operations.
- The above measurements have been measured on Virtual Machine running Ubuntu 24.04.1 LTS.

---

## Design Decisions

- The file system does not print the output during the benchmarking process.
- The file system can switch between user mode and benchmarking mode as follows:

```bash
./main                               // for user-mode
./main 1                             // for benchmarking-mode
```

- The file system uses mutex locks and conditional variables for locking the system whenever critical section has to be accessed.

## Some important instructions

- If the `makefile` does not run, then run the following 2 commands in the terminal:

```bash
g++ main.cpp -o main            // To compile the file system
./main                          // If the system has to run in user-mode
./main 1                        // If the system has to run in benchmarking-mode
```
