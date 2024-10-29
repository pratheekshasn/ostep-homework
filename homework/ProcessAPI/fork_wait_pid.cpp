/*
Write a slight modification of the previous program, this time us-
ing waitpid() instead of wait(). When would waitpid() be
useful?
*/

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

int main()
{
    pid_t parent_pid = getpid();
    int rc = fork();
    if (rc < 0)
    {
        std::cerr << "Fork failed" << std::endl;
        exit(1);
    }
    else if (rc == 0)
    {
        int wc = waitpid(parent_pid, NULL, 0);
        std::cout << "Child process" << std::endl;
        std::cout << "Wait returns: " << wc << std::endl;
    }
    else
    {
        // rc has the PID of the child process here.
        // int wc = waitpid(rc, NULL, 0);
        std::cout << "Parent process" << std::endl;
        // std::cout << "Wait returns: " << wc << std::endl;
    }
}

/*
waitpid() would be useful when you want to wait for a specific child process to terminate.
The above answer was given by Co-pilot.

waitpid() is useful when the parent needs to run before the child process. ???
But is this even valid? What happens if the parent runs first? Does the child become orphaned?
*/