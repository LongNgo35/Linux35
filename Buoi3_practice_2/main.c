#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int temp = 0;
    int rv = 0;
    int status;
    printf("Initial value of temp is: %d\n", temp);

    pid_t child_process;

    child_process = fork();
    if (0 == child_process)
    {
        printf("Child process is created successfully and it's performing this line of code\n");
        printf("Child process's pid: %d\n", getpid());
        // sleep(8);
        while(1);
        exit(6);
        
    }
    else
    {
        printf("Parent process is performing this line of code\n");
        printf("Parent process is waiting for child process\n");
        rv = wait(&status);
        printf("pid of child process just terminate is %d\n", rv);
        if (WIFEXITED(status))
        {
            printf("Normally terminate\n");
            printf("Return value: %d\n", WEXITSTATUS(status));
          
        }
        else if (WIFSIGNALED(status))
        {
            printf("Force terminate by signal\n");
            printf("Return value: %d\n", WTERMSIG(status));
        }
        
        
        
    }
    //printf("The, what is the process which execute out of if/else statement %d\n", getpid());
    
    return 0;
}
