#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>


int main()
{
    char *paragraph_1 = "Xin chao toi la nguoi Viet Nam\n";
    char *paragraph_2 = "Va toi ten la Ngo Truong Long\n";
    //int file = open("text.txt", O_WRONLY | O_APPEND | O_CREAT, S_IRUSR | S_IWUSR);
    int file = open("text.txt", O_WRONLY | O_APPEND);
    if(file < 0)
    {
        perror("cannot open the file\n");
        return 1;
    }

    int written_bytes = write(file, paragraph_1, strlen(paragraph_1) - 1);
    if(written_bytes < 0)
    {
        perror("Cannot write to this file\n");
        close(file);
        return 1;
    }

    // move the cursor to the first position
    off_t offset = lseek(file, 0, SEEK_SET);
    if (offset == (off_t)(-1))
    {
        perror("Cannot change the cursor position\n");
        close(file);
        return offset;
    }
    written_bytes = write(file, paragraph_2, strlen(paragraph_2) - 1);
    if (written_bytes < 0)
    {
        perror("Cannot write to this file2\n");
        close(file);
        return 1;
    }

    printf("Write to this file successfuly\n");
    close(file);
    return 0;
}