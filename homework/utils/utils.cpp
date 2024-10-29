#include <iostream>
#include "utils.h"
#include <unistd.h>

int read_char_from_pipe(int pipe[2], char* buf)
{
    if (read(pipe[0], buf, 1) == -1)
    {
        std::cerr << "read() failed\n";
        return 1;
    }
    else
    {
        // std::cout << "Process " << getpid() << ": Read from pipe on CPU " << sched_getcpu() << " data: " << buf << std::endl;
        return 0;
    }
}

int write_char_to_pipe(int pipe[2], char* buf)
{
    if (write(pipe[1], buf, 1) == -1)
    {
        std::cerr << "write() failed\n";
        return 1;
    }
    else
    {
        // std::cout << "Process " << getpid() << ": Wrote to pipe on CPU " << sched_getcpu() << " data: " << buf << std::endl;
        return 0;
    }
}

int set_cpu_affinity(int cpu = 0)
{
    cpu_set_t mask;
    CPU_ZERO(&mask);
    CPU_SET(cpu, &mask);
    if (sched_setaffinity(0, sizeof(mask), &mask) == -1)
    {
        std::cerr << "sched_setaffinity() failed\n";
        return 1;
    }
    return 0;
}

uint64_t rdtsc() {
    unsigned int lo, hi;
    __asm__ __volatile__ (
        "rdtsc"            // Read time-stamp counter
        : "=a" (lo), "=d" (hi) // Output: lo in eax, hi in edx
    );
    return ((uint64_t)hi << 32) | lo;
}
