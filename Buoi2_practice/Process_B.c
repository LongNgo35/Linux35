#include <stdio.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/stat.h>
#include <string.h>
#include <sys/file.h>

int main()
{
    int fd = 0;
    int used_bytes = 0;
    char *fp = "test.txt";
    char *text = "Xin chao, toi ten la Ngo Truong Long2\n";
    char for_reading[100];

    // open the file
    fd = open(fp, O_RDWR);
    if (fd < 0) {
        printf("cannot open the file: %s", fp);
        close(fd);
        return fd;
    }
    
    if(flock(fd, LOCK_EX) < 0) {
        printf("Cannot lock the file\n");
        close(fd);
        return 1;
    }

    // write to the file
    used_bytes = write(fd, text, strlen(text));
    if (used_bytes < 0) {
        printf("Cannot write to the file\n");
        close(fd);
        return used_bytes;
    } else {
        printf("Write successfully %d bytes to file : %s\n", used_bytes, fp);
    }

    // read from a file
    used_bytes = lseek(fd, 0, SEEK_SET);
    if (used_bytes < 0) {
        printf("Cannot change file cursor\n");
        close(fd);
        return used_bytes;
    }
    
    used_bytes = read(fd, for_reading, strlen(text));
    if (used_bytes < 0) {
        printf("Cannot read to the file\n");
        return used_bytes;
        close(fd);
    } else {
        printf("read successfully %d bytes to file %s\n ", used_bytes, fp);
        printf("data: %s", for_reading);
    }

    sleep(60);

    if(flock(fd, LOCK_UN) < 0) {
        printf("Cannot unlock the file\n");
        close(fd);
        return 1;
    }

    printf("Completed\n");

    close(fd);
    return 0;
}