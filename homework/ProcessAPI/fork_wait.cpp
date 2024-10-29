/*
Now write a program that uses wait() to wait for the child process
to finish in the parent. What does wait() return? What happens if
you use wait() in the child?
*/

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

int main()
{
    int rc = fork();
    if (rc < 0)
    {
        std::cerr << "Fork failed" << std::endl;
        exit(1);
    }
    else if (rc == 0)
    {
        // int wc = wait(NULL);
        std::cout << "Child process" << std::endl;
        // std::cout << "Wait returns: " << wc << std::endl;
    }
    else
    {
        int wc = wait(NULL);
        std::cout << "Parent process" << std::endl;
        std::cout << "Wait returns: " << wc << std::endl;
    }
}

/*
When the child process calls wait(), the parent process seems to run first.
But the value returned by wait() is -1 when the child process calls it,
and the value is the PID of the child process when the parent process calls it.

Prompt given by Co-pilot: This is because the child process has already
terminated by the time it calls wait(), so wait() returns -1. The parent process,
on the other hand, waits for the child process to terminate, so wait()
returns the PID of the child process.
*/