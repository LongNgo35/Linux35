#include <stdio.h>
#include <unistd.h>
#include <signal.h>

#define OVERRIDED_SIGNALS   3

void override_sigint(int signum)
{
    if (signum == SIGINT)
    {
        printf("SIGINT is override by override_sigint function %d\n", signum);
    } else if (signum == SIGUSR1) {
        printf("SIGUSR1 is override by override_sigint function %d\n", signum);
    } else if (signum == SIGUSR2) {
        printf("SIGUSR2 is override by override_sigint function %d\n", signum);
    } else {
        printf("Other signal\n");
    }
}

int main()
{
    // local variable
    struct sigaction sigact[OVERRIDED_SIGNALS];

    // processing
    printf("Bai tap 8\n");

    // config signal
    for (size_t i = 0; i < OVERRIDED_SIGNALS; i++)
    {
        sigact[i].sa_handler = override_sigint;
        sigemptyset(&sigact[i].sa_mask);
        sigact[i].sa_flags = SA_RESTART;
    }

    // register signal
    if(sigaction(SIGINT, &sigact[0], NULL) == -1) {
        printf("cannot install an action for SIGINT\n");
        return -1;
    } else {
        printf("SIGINT is installed successfully\n");
    }

    if(sigaction(SIGUSR1, &sigact[1], NULL) == -1) {
        printf("cannot install an action for SIGINT\n");
        return -1;
    } else {
        printf("SIGINT is installed successfully\n");
    }

    if(sigaction(SIGUSR2, &sigact[2], NULL) == -1) {
        printf("cannot install an action for SIGINT\n");
        return -1;
    } else {
        printf("SIGINT is installed successfully\n");
    }

    while(1) {
        sleep(1);
    }

}