# Memory Monitor C++

this is a bare-bones memory monitor written in C++. None of that high-level abstraction bloat. We are talking directly to the Linux kernel to see how much RAM our process is actually eating.

Basically, this script parses the system logs in real-time, allocates a chunk of heap memory, and watches the RSS (Resident Set Size) spike and drop.

## How it works

We aren't using external profilers here. We are digging into the process filesystem.

### 1. Reading `/proc/self/stat`

Linux stores process info in text files. We use `std::fstream` to open `/proc/self/stat`. This file is full of raw numbers.

- We iterate through the file stream, ignoring the first few tokens to get to the RSS value.
- **RSS (Resident Set Size)** = the number of pages the process has in real physical memory.

### 2. Page size calculation

Getting the RSS count isn't enough because that's just the number of "pages".

- We call `sysconf(_SC_PAGESIZE)` to get the system's page size (usually 4KB).
- Math is simple: `rss_count * page_size = total_bytes_used`.
- We use `long` types to make sure we don't overflow when handling big memory addresses.

### 3. Heap stress test

In `main()`, we simulate a load:

1. **Initial check:** print baseline usage.  
2. **Allocation:** we fire up `new int[1024 * 1024 * 10]`. This puts a massive array on the heap.  
3. **Dirtying pages:** we loop through and fill the array. This is crucial—Linux is lazy. It won't actually map physical RAM until you write to it.  
4. **Cleanup:** we call `delete[]` to free the memory.

## Run it

Make sure you are on a Linux machine (or WSL). This won't work on Windows native because `/proc/` doesn't exist there.

```bash
g++ monitor.cpp -o monitor
./monitor
