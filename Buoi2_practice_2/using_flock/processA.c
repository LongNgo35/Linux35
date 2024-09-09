#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <string.h>

#define READING_BUFFER_MAX_SIZE  100

int main()
{
    char *file_name = "test.txt";
    char *file_name_content = "Ngo Long la toi";
    char reading_buffer[READING_BUFFER_MAX_SIZE];
    int interacted_bytes = 0;
    
    int fd = open(file_name, O_RDWR);
    if(flock(fd, LOCK_EX) == -1)
    {
        printf("Cannot lock file: %s\n", file_name);
        return -1;
    }
    else
    {
        printf("lock file %s successfully\n", file_name);
    }

    interacted_bytes = write(fd, file_name_content, strlen(file_name_content));
    if (interacted_bytes <= 0)
    {
        printf("Cannot write to file: %s", file_name);
        return interacted_bytes;
    }
    else
    {
        printf("Writing successful %d to file: %s\n", interacted_bytes, file_name);
    }

    if(lseek(fd, 0, SEEK_SET) < 0)
    {
        printf("Cannot change cursor position of file :%s", file_name);
        return -1;
    }
    

    interacted_bytes = read(fd, reading_buffer, READING_BUFFER_MAX_SIZE);
    if (interacted_bytes <= 0)
    {
        printf("Cannot read from file: %s", file_name);
        return interacted_bytes;
    }
    else
    {
        printf("Reading successful %d to file: %s\n", interacted_bytes, file_name);
        printf("The file content is: %s\n", reading_buffer);
    }

    while(1)
    {
        sleep(1);
    }

    close(fd);
    return 0;
}