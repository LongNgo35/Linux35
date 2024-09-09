#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>


int main()
{
    pid_t p = fork();
    if(p < 0) {
        perror("Cannot create new process\n");
    }
    else if(p == 0) {
        printf("New process was created with pid: %d, ppid: %d\n", getpid(), getppid());
    } else {
        printf("Here is main process, pid: %d\n", getpid());
    }
    printf("Here still is main process, pid: %d\n", getpid());
    return 0;
}