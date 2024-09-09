#include <stdio.h>
#include <sys/types.h>
#include <signal.h>
#include <sys/syscall.h>
#include <unistd.h>

sigset_t sigsta;

void signal_handler(int signum)
{
    printf("Signal %d was sent to this process :D\n", signum);
}

int check_blocked_signal(int signum)
{
    int ret = 0;
    int signum_status;
    
    // get current mask
    sigprocmask(SIG_BLOCK, NULL, &sigsta);

    // check signum is set or not
    if(sigismember(&sigsta, signum)) {
        printf("Signal number %d is blocked\n", signum);
        ret = 1;
    } else {
        printf("Signal number %d is unblock\n", signum);
        ret = 0;
    }
    return ret;
}

void block_signal(int signum)
{
    if(check_blocked_signal(signum)) {
        printf("Signal number %d already blocked\n", signum);
    } else {
        printf("Blocking signal number %d\n", signum);
        sigaddset(&sigsta, signum);
        sigprocmask(SIG_SETMASK, &sigsta, NULL);
        printf("Blocking signal %d is completed\n", signum);

    }
}

void un_block_signal(int signum)
{
    if(check_blocked_signal(signum)) {
        printf("Unblocking signal number %d\n", signum);
        sigdelset(&sigsta, signum);
        sigprocmask(SIG_SETMASK, &sigsta, NULL);
        printf("Unblocking signal %d is completed\n", signum);
    } else {
        printf("Signal number %d already unblocked\n", signum);
    }
}

int main()
{
    int option = 0;
    int signum = 0;
    while (1)
    {
        printf("\n------ MENU ------\n");
        printf("1. Check blocked signal\n");
        printf("2. Block a signal\n");
        printf("3. Unblock a signal\n");
        
        printf("Enter signal number: ");
        scanf("%d", &signum);

        printf("Enter your option: ");
        scanf("%d", &option);
        
        if (option == 1) {
            printf("Option 1 is selected\n");
            check_blocked_signal(signum);
        } else if (option == 2) {
            printf("Option 2 is selected\n");
            block_signal(signum);
        } else if (option == 3) {
            printf("Option 3 is selected\n");
            un_block_signal(signum);
        } else {
            printf("Invalid option, please type again");
        }
    }
    

    return 0;
}