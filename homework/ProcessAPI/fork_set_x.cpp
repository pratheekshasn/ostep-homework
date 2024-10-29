/*
Write a program that calls fork(). Before calling fork(), have the
main process access a variable (e.g., x) and set its value to some-
thing (e.g., 100). What value is the variable in the child process?
What happens to the variable when both the child and parent change
the value of x?
*/

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int x = 100;

    int rc = fork();
    if (rc < 0)
    {
        std::cerr << "fork failed\n";
        exit(1);
    }
    else if (rc == 0)
    {
        // Child process
        x += 1;
        std::cout << "Child process: x = " << x << std::endl;
    }
    else
    {
        int rc_wait = wait(NULL);
        // Parent process
        x += 1;
        std::cout << "Parent process: x = " << x << std::endl;
    }
}