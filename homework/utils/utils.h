#ifndef __utils_h__
#define __utils_h__

#include <cstdint>

int read_char_from_pipe(int pipe[2], char* buf);
int write_char_to_pipe(int pipe[2], char* buf);
int set_cpu_affinity(int cpu);
uint64_t rdtsc();

#endif // __utils_h__