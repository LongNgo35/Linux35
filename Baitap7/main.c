#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <signal.h>

int child_ret = 0;
int status = 0;

void sigact_handler(int signum)
{
    printf("Signal handler was called\n");
    child_ret = wait(&status);
    if (child_ret < 0) {
        perror("reset handler was call but incorrect\n");
    }
    else {
        if (WIFEXITED(status)) {
            printf("The child process terminate normally with return value: %d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status)) {
            printf("the child process was force terminated by using signal with sigid: %d\n", WTERMSIG(status));
        } else {
            // do nothing
        }
    }
}

int main()
{
    int ret = 0;
    // init signal hanlder variable
    struct sigaction sigact;

    // config sigact parameter
    sigact.sa_handler = sigact_handler;
    sigemptyset(&sigact.sa_mask);
    sigact.sa_flags = SA_RESTART | SA_NOCLDSTOP;

    // install new signal
    ret = sigaction(SIGCHLD, &sigact, NULL);
    if (ret == -1)
    {
        perror("Cannot install signal\n");
        return ret;
    }
    
    // create new process
    ret = fork();
    if(ret < 0) {
        perror("Cannot create new process\n");
        return ret;
    }
    else if(ret == 0) {
        // child proces will handle
        printf("Chill process id: %d, parent process id: %d\n", getpid(), getppid());
        exit(6);
    }
    else {
        printf("Parent process id: %d\n", getpid());
        // infinite loop
        while(1) {
            sleep(1);
        }
        
    }
    return ret;
}