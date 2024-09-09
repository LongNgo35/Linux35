#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <sys/types.h>
#include <stdlib.h>
#include <sys/wait.h>

void handler_signal(int signum)
{
    while (1) {
        printf("handler_signal was call with signum: %d\n", signum);
        sleep(2);
    }
}

void *child_thread_handler(void *arg)
{
    // local variable
    pid_t *process_id =  (pid_t *)arg;
    // config signal action
    struct sigaction sigact;
    sigact.sa_handler = handler_signal;
    sigemptyset(&sigact.sa_mask);
    sigaddset(&sigact.sa_mask, SIGINT);
    
    // display information
    printf("The process id contain this child thread is: %d", *process_id);

    // register signal
    sigaction(SIGUSR1, &sigact, NULL);

    pause();
}

int main()
{
    pid_t p = fork();
    int child_ret = 0;
    int status = 0;
    if (p < 0) {
        perror("cannot create new process\n");
    } else if (p == 0) {
        // child process execute this block of code

        /* 1. define variable */
        pid_t process_id = getpid();
        pthread_t child_thread;
        sigset_t sa_mask;

        /* 2. display information */
        printf("CHILD PROCESS is executing with process id: %d, and parent process id: %d\n", process_id, getppid());

        /* set mask */
        sigemptyset(&sa_mask);
        sigprocmask(SIG_UNBLOCK, &sa_mask, NULL);

        /* 3. create a new thread in child process */
        int status = pthread_create(&child_thread, NULL, child_thread_handler, (void *)(&process_id));
        if (status == -1)
        {
            return -1;
        }

        while(1)
        {
            sleep(3);
            printf("Thread main is running here\n");
        }
    } else {
        // parent process execute this block of code
        int status = 0;
        /* 2. display information */
        printf("PARENT PROCESS is executing with process id: %d\n", getpid());

        /* 3. send USR1 signal to child process */
        //status = kill(p, SIGUSR1);

        wait(NULL);
    }
    return 0;
    
    
}


#if 0

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

#endif