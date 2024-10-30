/*
Create a little program that uses a certain amount of memory,
called memory-user.c. This program should take one command-
line argument: the number of megabytes of memory it will use.
When run, it should allocate an array, and constantly stream through
the array, touching each entry. The program should do this indefi-
nitely, or, perhaps, for a certain amount of time also specified at the
command line.
*/

#include <iostream>
#include <unistd.h>

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        std::cerr << "Usage: " << argv[0] << " <number of megabytes>" << std::endl;
        return 1;
    }

    std::cout << "PID: " << getpid() << std::endl;

    int megabytes = std::stoi(argv[1]);
    int n = megabytes * 1024 * 1024 / sizeof(int);
    int* array = new int[n];

    while (true)
    {
        for (int i = 0; i < n; i++)
        {
            array[i] = i;
        }
    }
    return 0;
}