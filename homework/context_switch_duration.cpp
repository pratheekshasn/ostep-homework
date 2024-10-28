/*
Measure the cost of a context switch. 

The lmbench benchmark does so by running two processes on a single CPU, and set-
ting up two UNIX pipes between them; a pipe is just one of many ways
processes in a UNIX system can communicate with one another. The first
process then issues a write to the first pipe, and waits for a read on the
second; upon seeing the first process waiting for something to read from
the second pipe, the OS puts the first process in the blocked state, and
switches to the other process, which reads from the first pipe and then
writes to the second. When the second process tries to read from the first
pipe again, it blocks, and thus the back-and-forth cycle of communication
continues. By measuring the cost of communicating like this repeatedly,
lmbench can make a good estimate of the cost of a context switch. You
can try to re-create something similar here, using pipes, or perhaps some
other communication mechanism such as UNIX sockets.

One difficulty in measuring context-switch cost arises in systems with
more than one CPU; what you need to do on such a system is ensure that
your context-switching processes are located on the same processor. For-
tunately, most operating systems have calls to bind a process to a partic-
ular processor; on Linux, for example, the sched setaffinity() call
is what you’re looking for. By ensuring both processes are on the same
processor, you are making sure to measure the cost of the OS stopping
one process and restoring another on the same CPU.
*/

#include <cstdint>
#include <iostream>
#include <sys/time.h>
#include <unistd.h>

#define ITERATIONS 1000000

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

void write_read_pipes(int pipefds1[2], int pipefds2[2])
{
    char buf[1];
    for (size_t i = 0; i < ITERATIONS; i++)
    {
        read_char_from_pipe(pipefds1, buf);
        write_char_to_pipe(pipefds2, buf);
    }
}

void read_write_pipes(int pipefds1[2], int pipefds2[2])
{
    char buf[1] = {'a'};
    for (size_t i = 0; i < ITERATIONS; i++)
    {
        write_char_to_pipe(pipefds1, buf);
        read_char_from_pipe(pipefds2, buf);
    }
}

uint64_t rdtsc() {
    unsigned int lo, hi;
    __asm__ __volatile__ (
        "rdtsc"            // Read time-stamp counter
        : "=a" (lo), "=d" (hi) // Output: lo in eax, hi in edx
    );
    return ((uint64_t)hi << 32) | lo;
}

int main()
{
    // Create two pipes
    int pipefds1[2], pipefds2[2];
    if (pipe(pipefds1) == -1 || pipe(pipefds2) == -1)
    {
        std::cerr << "pipe() failed\n";
        return 1;
    }

    int fork_res = fork();
    if (fork_res == -1)
    {
        std::cerr << "fork() failed\n";
        return 1;
    }
    else if (fork_res == 0)
    {
        // Child process

        // Set CPU affinity
        set_cpu_affinity(0);

        close(pipefds1[1]);
        close(pipefds2[0]);

        uint64_t start = rdtsc();
        write_read_pipes(pipefds1, pipefds2);        
        uint64_t end = rdtsc();

        std::cout << "CPU cycles for child process: " << (end - start) << std::endl;
        std::cout << "Time taken for each context switch: " << (end - start) / ITERATIONS << " cycles" << std::endl;
    } else
    {
        // Parent process

        // Set CPU affinity
        set_cpu_affinity(0);

        close(pipefds1[0]);
        close(pipefds2[1]);

        uint64_t start = rdtsc();
        read_write_pipes(pipefds1, pipefds2);
        uint64_t end = rdtsc();

        std::cout << "CPU cycles for parent process: " << (end - start) << std::endl;
        std::cout << "Time taken for each context switch: " << (end - start) / ITERATIONS << " cycles" << std::endl;
    }
}