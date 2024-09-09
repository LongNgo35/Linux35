#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>

#define VAL_NUM  100

sem_t semaphore;
pthread_t thread_odd;
pthread_t thread_even;

void *display_odd(void *arg)
{
    sem_wait(&semaphore);
    for (size_t i = 0; i < VAL_NUM; i++)
    {
        (i % 2 != 0) ?  printf("Odd numbers from 0 to 10 is: %ld\n", i) : (void)0; 
    }
    sem_post(&semaphore);
}

void *display_even(void *arg)
{
    sem_wait(&semaphore);
    for (size_t i = 0; i < VAL_NUM; i++)
    {
        (i % 2 == 0) ?  printf("Even numbers from 0 to 10 is: %ld\n", i) : (void)0; 
    }
    sem_post(&semaphore);
}


int main()
{
    int ret = 0;
    
    // create new semaphore
    ret = sem_init(&semaphore, 0, 1);
    if(ret == -1)
    {
        perror("Cannot create new semaphore\n");
    }
    
    // create new thread
    ret = pthread_create(&thread_odd, NULL, display_odd, NULL);
    if (ret != 0)
    {
        perror("Cannot create new thread\n");
    }

    ret = pthread_create(&thread_even, NULL, display_even, NULL);
    if(ret != 0)
    {
        perror("Cannot create new thread\n");
    }

    pthread_join(thread_odd, NULL);
    pthread_join(thread_even, NULL);

    sem_destroy(&semaphore);

    return 0; 
}