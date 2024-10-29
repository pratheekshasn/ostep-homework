/*
Write a program that creates two children, and connects the stan-
dard output of one to the standard input of the other, using the
pipe() system call.
*/

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

int main()
{
    int pipefd[2];
    if (pipe(pipefd) == -1)
    {
        std::cerr << "Pipe failed" << std::endl;
        exit(1);
    }

    int rc1 = fork();
    if (rc1 < 0)
    {
        std::cerr << "Fork failed" << std::endl;
        exit(1);
    }
    else if (rc1 == 0)
    {
        close(pipefd[0]);
        dup2(pipefd[1], STDOUT_FILENO);
        close(pipefd[1]);
        execl("/bin/ls", "ls", NULL);
    }
    else
    {
        int rc2 = fork();
        if (rc2 < 0)
        {
            std::cerr << "Fork failed" << std::endl;
            exit(1);
        }
        else if (rc2 == 0)
        {
            close(pipefd[1]);
            dup2(pipefd[0], STDIN_FILENO);
            close(pipefd[0]);
            execl("/usr/bin/wc", "wc", "-l", NULL);
        }
        else
        {
            close(pipefd[0]);
            close(pipefd[1]);
            wait(NULL);
            wait(NULL);
        }
    }
    return 0;
}