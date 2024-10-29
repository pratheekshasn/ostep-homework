/*
Write another program using fork(). The child process should
print “hello”; the parent process should print “goodbye”. You should
try to ensure that the child process always prints first; can you do
this without calling wait() in the parent?
*/

#include <iostream>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <unistd.h>
#include "utils.h"
#include <string.h>

int setup_shared_memory()
{
    // ftok to generate unique key
    key_t key = ftok("shmfile", 65);

    // shmget returns an identifier in shmid
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);
    return shmid;
}

int main()
{
    // ftok to generate unique key
    key_t key = ftok("shmfile", 42);

    // shmget returns an identifier in shmid
    int shmid = shmget(key, 1024, 0666 | IPC_CREAT);

    int rc = fork();
    if (rc < 0)
    {
        std::cerr << "fork failed\n";
        exit(1);
    }
    else if (rc == 0)
    {
        // Child process
        std::cout << "hello\n";
        
        // Attach the segment to the process's address space
        char* shared_mem = static_cast<char*>(shmat(shmid, nullptr, 0));
        if (shared_mem == (char*)-1) {
            perror("shmat");
            return 1;
        }

        // Write data to the shared memory
        const char* message = "1";
        strncpy(shared_mem, message, 1);

        shmdt(shared_mem);
    }
    else
    {
        // Parent process
        int shm_id = shmget(key, 1, 0666);
        if (shm_id == -1) {
            perror("shmget");
            return 1;
        }
        
        while(true)
        {
            // Attach the segment to the process's address space
            char* shared_mem = static_cast<char*>(shmat(shm_id, nullptr, 0));
            if (shared_mem == (char*)-1) {
                perror("shmat");
                return 1;
            }
            if (shared_mem[0] == '1') {
                shmdt(shared_mem);
                break;
            }
            else{
                shmdt(shared_mem);
            }
        }

        std::cout << "goodbye\n";
        
        // Mark the segment to be destroyed after detachment
        shmctl(shm_id, IPC_RMID, nullptr);
    }
}