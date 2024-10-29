/*
Write a program that calls fork() and then calls some form of
exec() to run the program /bin/ls. See if you can try all of the
variants of exec(), including (on Linux) execl(), execle(),
execlp(), execv(), execvp(), and execvpe(). Why do
you think there are so many variants of the same basic call?
*/

#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <cstdlib>

int main() {
    int rc = fork();
    if (rc < 0) {
        std::cerr << "Fork failed" << std::endl;
        exit(1);
    } else if (rc == 0) {
        execl("/bin/ls", "ls", NULL);
        // execle("/bin/ls", "ls", NULL, NULL);
        // execlp("ls", "ls", NULL);
        // execv("/bin/ls", NULL);
        // execvp("ls", NULL);
        // execvpe("ls", NULL, NULL);
    } else {
        wait(NULL);
    }
    return 0;
}

/*
Function	Arguments Type	        PATH Search     Custom Environment	Arguments Format
execl	    Variable-length list	No	            No	                List of arguments, NULL-terminated
execle	    Variable-length list	No	            Yes	                List of arguments, NULL-terminated, with envp at the end
execlp	    Variable-length list	Yes	            No	                List of arguments, NULL-terminated
execv	    Array	                No	            No	                argv array, NULL-terminated
execvp	    Array	                Yes	            No	                argv array, NULL-terminated
execvpe	    Array	                Yes	            Yes	                argv array, NULL-terminated, with envp
*/