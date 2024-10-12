/*
Measure the cost of a system call. For example,
you could repeatedly call a simple system call (e.g., performing a 0-byte
read), and time how long it takes; dividing the time by the number of
iterations gives you an estimate of the cost of a system call.
One thing you’ll have to take into account is the precision and accu-
racy of your timer. A typical timer that you can use is gettimeofday();
read the man page for details. What you’ll see there is that gettimeofday()
returns the time in microseconds since 1970; however, this does not mean
that the timer is precise to the microsecond. Measure back-to-back calls
to gettimeofday() to learn something about how precise the timer re-
ally is; this will tell you how many iterations of your null system-call
test you’ll have to run in order to get a good measurement result. If
gettimeofday() is not precise enough for you, you might look into
using the rdtsc instruction available on x86 machines.
*/

#include <iostream>
#include <sys/time.h>
#include <unistd.h>
#include <cstdint>

#define ITERATIONS 1000000

void inline repeatedly_call_system_call() {
    for (int i = 0; i < ITERATIONS; i++)
        read(0, NULL, 0);
}

uint64_t rdtsc() {
    unsigned int lo, hi;
    __asm__ __volatile__ (
        "rdtsc"            // Read time-stamp counter
        : "=a" (lo), "=d" (hi) // Output: lo in eax, hi in edx
    );
    return ((uint64_t)hi << 32) | lo;
}

int use_rdtsc() {
    uint64_t start = rdtsc();

    repeatedly_call_system_call();

    uint64_t end = rdtsc();

    std::cout << "CPU cycles: " << (end - start) << std::endl;

    return (end - start) / ITERATIONS;
}

long long use_get_time_of_day() {
    struct timeval start, end;
    gettimeofday(&start, NULL);
    
    repeatedly_call_system_call();
    
    gettimeofday(&end, NULL);
    long long start_time = start.tv_sec * 1000000 + start.tv_usec;
    long long end_time = end.tv_sec * 1000000 + end.tv_usec;
    std::cout << "Time taken for " << ITERATIONS << " iterations: " << (end_time - start_time) << " microseconds" << std::endl;
    long long sys_call_duration = (end_time - start_time) / ITERATIONS;
    std::cout << "Time taken for each iteration: " << sys_call_duration << " microseconds" << std::endl;
    return sys_call_duration;
}

int main() {
    std::cout << "Using get_time_of_day(): " << use_get_time_of_day() << std::endl;
    std::cout << "Using rdtsc(): " << use_rdtsc() << std::endl;
}
