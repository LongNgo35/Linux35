#include <stdio.h>
#include <sys/stat.h>
#include <time.h>

int main()
{
    // open or create the file
    FILE *file = fopen("test.txt", "w");
    if(!file) {
        perror("The file cannot open\n");
        return 1;
    }

    // write a string to the file
    fprintf(file, "Hello world\n");

    // close the file 
    fclose(file);

    struct stat file_properties;
    if(stat("./test.txt", &file_properties) < 0) {
        perror("cannot get the file properties\n");
        return 1;
    }

    printf("File type: %s\n", (S_ISDIR(file_properties.st_mode) ? "Directory" : "File"));
    printf("File name: test.txt\n");
    printf("The most recently access: %s\n", ctime(&file_properties.st_mtime));
    printf("File size: %ld\n", file_properties.st_size);


    return 0;
}