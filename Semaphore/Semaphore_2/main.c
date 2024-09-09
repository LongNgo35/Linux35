#include <stdio.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#define MAX_VALUE   100
#define SEM_NAME     "/NGOLONG"
#define SHM_KEY      "TRUONGLONG"
#define SHM_SIZE     MAX_VALUE

// gloabal variable
sem_t *sem;

int *shared_data = NULL;

// This function will be executed by child process
void display_odd_numbers()
{
    sem_wait(sem);
    // for (size_t i = 0; i < MAX_VALUE; i++)
    // {
    //     (i % 2 != 0) ? printf("Odd number: %ld\n", i) : (0);
    // }
    for (size_t i = 0; i < MAX_VALUE; i++)
    {
        shared_data[i] = i;
    }

    printf("Successfully write %d values to share buffer beetwen 2 process \n", MAX_VALUE);
    sem_post(sem);
}

// This function will be executed by parent process
void display_even_number()
{
    sem_wait(sem);
    // for (size_t i = 0; i < MAX_VALUE; i++)
    // {
    //     (i % 2 == 0) ? printf("Even number: %ld\n", i) : (0);
    // }
    for (size_t i = 0; i < MAX_VALUE; i++)
    {
        printf("Data at buffer: %d\n", shared_data[i]);
    }

    printf("Successfully read values from share buffer beetwen 2 process\n");
    sem_post(sem);
}


int main()
{
    int shm_id = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shm_id == -1)
    {
        perror("Cannot create segment memory\n");
    }

    shared_data = (int *)shmat(shm_id, NULL, 0);
    if (shared_data == (void *)-1)
    {
        perror("Cannot assign segment memory\n");
    }

    sem_unlink (SEM_NAME);

    sem = sem_open(SEM_NAME, O_CREAT | O_EXCL, 0666, 1);
    if (!sem)
    {
        perror("Cannot create semaphore\n");
    } 
    // create new process
    pid_t process_id = fork();
    if (process_id < 0)
    {
        perror("Cannot create new process\n");
    }
    else if (process_id == 0)
    {
        // child process
        //printf("Child process is running\n");
        display_odd_numbers();
    } 
    else
    {
        // parent process
        //printf("Parent process is running\n");
        sleep(0.1);
        display_even_number();
        wait(NULL);
    }

    sem_destroy(sem);
    return 0;
    
}