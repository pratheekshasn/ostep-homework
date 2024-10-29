/*
Write a program that opens a file (with the open() system call)
and then calls fork() to create a new process. Can both the child
and parent access the file descriptor returned by open()? What
happens when they are writing to the file concurrently, i.e., at the
same time?
*/

#include <iostream>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>

int main()
{
    int fd = open("file.txt", O_CREAT | O_WRONLY | O_TRUNC, S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH);
    if (fd < 0)
    {
        std::cerr << "open failed\n";
        exit(1);
    }

    int rc = fork();
    if (rc < 0)
    {
        std::cerr << "fork failed\n";
        exit(1);
    }
    else if (rc == 0)
    {
        // Child process
        const char* child_msg = "Child process\n";
        write(fd, child_msg, strlen(child_msg));
    }
    else
    {
        // Parent process

        // Wait for the child process to finish
        int rc_wait = wait(NULL);

        const char* parent_msg = "Parent process\n";
        write(fd, parent_msg, strlen(parent_msg));
    }
}