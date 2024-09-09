#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>

int main()
{
    pid_t p = fork();
    int child_ret = 0;
    int status = 0;
    if(p < 0) {
        perror("Cannot create new process\n");
    }
    else if(p == 0) {
        // child proces will handle
        printf("Chill process id: %d, parent process id: %d\n", getpid(), getppid());
        while(1);
        exit(6);
    }
    else {
        printf("Parent process id: %d\n", getpid());
        // using wait
        child_ret = wait(&status);
        if (WIFEXITED(status)) {
            printf("The child process terminate normally with return value: %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("the child process was force terminated by using signal with sigid: %d\n", WTERMSIG(status));
        } else {
            // do nothing
        }
    }
    return 0;
}