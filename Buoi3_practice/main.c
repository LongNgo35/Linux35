#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int main()
{
    int temp = 0;
    printf("Initial value of temp is: %d\n", temp);

    pid_t child_process;

    child_process = fork();
    if (0 == child_process)
    {
        printf("Child process is created successfully and it's performing this line of code\n");
        // printf("temp value in child process is %d\n", temp);
        // printf("temp address in parent process is %p\n", &temp);
        printf("Child_process: %d, parent process %d\n", getpid(), getppid());
        // while (1);
        
    }
    else
    {
        printf("Parent process is performing this line of code\n");
        // temp++;
        // printf("temp value in parent process is %d\n", temp);
        // printf("temp address in parent process is %p\n", &temp);
        printf("parent process %d\n", getpid());
        //while(1);
    }
    printf("The, what is the process which execute out of if/else statement %d\n", getpid());
    
    return 0;
}
