#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

#define MAX_SIZE    30
#define DELIMETER   6

typedef struct {
    char name[MAX_SIZE];
    char dob[MAX_SIZE];
    char country[MAX_SIZE];
} Student;

// global variable
Student student;
char file_name[] = "test.txt";
int fd = 0;
int total_info_size = 0;
pthread_mutex_t lock;
pthread_cond_t cond_1;
pthread_cond_t cond_2;
pthread_cond_t cond_3;

void clear_data(Student *arg)
{
    for (size_t i = 0; i < MAX_SIZE; i++) {
        arg->name[i] = '\0';
        arg->dob[i] = '\0';
        arg->country[i] = '\0';
    } 
}

void remove_new_line(char *arr, int size)
{
    int i = 0;
    for (i = 0; i < size; i++) {
        if (arr[i] == '\n')
        {
            arr[i] = 0x20;
            arr[i + 1] = 0x20;
            arr[i + 2] = 0x20;
            break;
        }
    }
}

void *get_info(void *arg)
{
    int start = 0;
    while(1)
    {
        printf("\n------ GET DATA FROM USER ------\n");

        pthread_mutex_lock(&lock);

        if (start > 0) {
            pthread_cond_wait(&cond_1, &lock);
        }
        start = 1;

        // clear data before process
        clear_data(&student);

        printf("Enter your name: ");
        fgets(student.name, MAX_SIZE, stdin);
        remove_new_line(student.name, strlen(student.name));
        printf("student name length: %ld\n", strlen(student.name));

        printf("Enter your date of birth: ");
        fgets(student.dob, MAX_SIZE, stdin);
        remove_new_line(student.dob, strlen(student.dob));
        printf("student dob length: %ld\n", strlen(student.dob));

        printf("Enter your country: ");
        fgets(student.country, MAX_SIZE, stdin);
        printf("student country length: %ld\n", strlen(student.country));
        //remove_new_line(student.country, strlen(student.country));

        total_info_size = strlen(student.name) + strlen(student.dob) + strlen(student.country);

        pthread_cond_signal(&cond_2);

        pthread_mutex_unlock(&lock);

        printf("------ SUCCESSFULLY GET DATA FROM USER ------\n");
        //sleep(1);
    }
}

void *write_to_file(void *arg)
{
    int written_bytes = 0;

    while(1)
    {
        pthread_cond_wait(&cond_2, &lock);
        // printf("\n------ WRITE DATA TO %s ------\n", file_name);
        // write data to test.txt
        written_bytes = write(fd, student.name, strlen(student.name));
        if (written_bytes <= 0) {
            printf("cannot write student name to %s\n", file_name);
            return NULL;
        }

        written_bytes = write(fd, student.dob, strlen(student.dob));
        if (written_bytes <= 0) {
            printf("cannot write student date of birth to %s\n", file_name);
            return NULL;
        }

        written_bytes = write(fd, student.country, strlen(student.country));
        if (written_bytes <= 0) {
            printf("cannot write student country to %s\n", file_name);
            return NULL;
        }
        // printf("------ SUCCESSFULLY WRITE DATA TO %s ------\n", file_name);
        pthread_cond_signal(&cond_3);
    }

    

}

void *read_from_file(void *arg)
{
    int read_bytes = 0;
    Student tmp_student;
    int start = 0;
    
    while(1)
    {
        pthread_cond_wait(&cond_3, &lock);

        //printf("\n------ READ DATA FROM %s ------\n", file_name);

        // change cursor position
        if (start == 0) {
            if(lseek(fd, 0, SEEK_SET) == -1) {
                printf("cannot perfom lseek_1");
                return NULL;
            }
        } else {
            if(lseek(fd, (-1 * total_info_size), SEEK_END) == -1){
                printf("cannot perfom lseek");
                return NULL;
            }
        }
        start = 1;
        
        clear_data(&tmp_student);
        
        // read data from test.txt
        read_bytes = read(fd, tmp_student.name, strlen(student.name));
        printf("student name length: %ld\n", strlen(student.name));

        if (read_bytes <= 0) {
            printf("cannot read student name from %s\n", file_name);
            return NULL;
        } else {
            printf("student name: %s\n", tmp_student.name);
        }

        read_bytes = read(fd, tmp_student.dob, strlen(student.dob));
        printf("student dob length: %ld\n", strlen(student.dob));
        if (read_bytes <= 0) {
            printf("cannot read student date of birth from %s\n", file_name);
            return NULL;
        } else {
            printf("student dob: %s\n", tmp_student.dob);
        }

        read_bytes = read(fd, tmp_student.country, strlen(student.country));
        printf("student country length: %ld\n", strlen(student.country));
        if (read_bytes <= 0) {
            printf("cannot read student country from %s\n", file_name);
            return NULL;
        } else {
            printf("student country: %s\n", tmp_student.country);
        }

        // printf("------ SUCCESSFULLY READ DATA FROM %s ------\n", file_name);
        pthread_cond_signal(&cond_1);
    }

    
}

int main()
{
    pthread_t thread[3];

    fd = open(file_name, O_CREAT | O_RDWR | O_APPEND, 0666);
    if (fd == -1) {
        printf("cannot open %s", file_name);
    }
    printf("%s was opened successfully\n", file_name);
    

    // create and start threads
    pthread_create(&thread[0], NULL, get_info, NULL);
    pthread_create(&thread[1], NULL, write_to_file, NULL);
    pthread_create(&thread[2], NULL, read_from_file, NULL);
    
    pthread_join(thread[0], NULL);
    pthread_join(thread[1], NULL);
    pthread_join(thread[2], NULL);

    pthread_mutex_destroy(&lock);
    pthread_cond_destroy(&cond_1);
    pthread_cond_destroy(&cond_2);
    pthread_cond_destroy(&cond_3);


    return 0;
}