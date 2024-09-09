#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <sys/wait.h>

#define SHM_KEY "1234"
#define SHM_SIZE 100

int main()
{
    // create system V segment memory
    int shm_id = shmget(SHM_KEY, SHM_SIZE, IPC_CREAT | 0666);
    if (shm_id == -1)
    {
        perror("Cannot create segment memory\n");
    }

    int *shared_data = (int *)shmat(shm_id, NULL, 0);
    if (shared_data == (void *)-1)
    {
        perror("Cannot assign segment memory\n");
    }

    pid_t process_id = fork();
    if (process_id < 0)
    {
        perror("Cannot create new process\n");
    }
    else if (process_id == 0)
    {
        // child process
        sleep(1);
        printf("The value child process get from parent process is: %d\n", *shared_data);
        shmdt(shared_data);
        exit(EXIT_SUCCESS);
    }
    else
    {
        // parent process
        // write data to shared memory
        *shared_data = 10;
        printf("The parent process already written value = 10 to shared memory\n");
        wait(NULL);
        shmdt(shared_data); // Giải phóng bộ nhớ chia sẻ
        shmctl(shm_id, IPC_RMID, NULL); // Xóa bộ nhớ chia sẻ
    }
    return 0;
    
    
}

