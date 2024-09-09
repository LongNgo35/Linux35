#include <stdio.h>
#include <unistd.h>
#include <pthread.h>

// delare struct human
typedef struct {
    char *name;
    int yob;
    char *pn;
    char *country;
} human;

// define global variable
pthread_t thread_1;
pthread_t thread_2;

// function handler
void *thread_handler(void *arg)
{
    pthread_t self;
    self = pthread_self();
    if (pthread_equal(thread_1, self))
    {
        printf("Thread 1 execute this line with\n");
    }
    else if (pthread_equal(thread_2, self))
    {
        human *human_arg = (human*)(arg);
        printf("Thread 2 execute this line with\n");
        printf("Human name: %s\n", human_arg->name);
        printf("Human year of birth: %d\n", human_arg->yob);
        printf("Human phone number: %s\n", human_arg->pn);
        printf("Human country: %s\n", human_arg->country);
    }
    else
    {
        printf("This line has been execute by a thread which other than Thread 1 and Thread 2\n");
    }  
}

int main()
{
    human NgoLong = {
        .name = "Ngo Truong Long",
        .yob = 2000,
        .pn = "0399855285",
        .country = "Ninh Binh - Viet Nam"
    };
    
    // create thread 1
    pthread_create(&thread_1, NULL, thread_handler, &thread_1);

    // create thread 2
    pthread_create(&thread_2, NULL, thread_handler, &NgoLong);

    while (1)
    {
        sleep(1);
    }
    

    return 0;
}