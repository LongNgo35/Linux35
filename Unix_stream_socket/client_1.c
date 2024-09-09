#include <stdio.h>
#include <unistd.h>
#include <sys/un.h>
#include <string.h>
#include <fcntl.h>
#include <sys/socket.h>

#define SERVER_SOCK_PATH  "./server_path"
#define SENT_MESSAGE "This message was send form client to server\0"

int main()
{
    // local variable
    int ret = 0;
    int socket_fd = 0;
    struct sockaddr_un unix_addr;

    // create endpoint for connecting
    socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if(socket_fd == -1) {
        perror("Cannot create new endpoint for client socket\n");
        return -1;
    }

    // config parameters for socket
    memset(&unix_addr, 0, sizeof(unix_addr));
    unix_addr.sun_family = AF_UNIX;
    memcpy(unix_addr.sun_path, SERVER_SOCK_PATH, sizeof(SERVER_SOCK_PATH));

    // connect socket fd with address
    ret = connect(socket_fd, (const struct sockaddr*)&unix_addr, sizeof(unix_addr));\
    if (ret == -1) {
        perror("Cannot connect to the server\n");
        return -1;
    }

    printf("Connect to the server successfully\n");
    ret = write(socket_fd, SENT_MESSAGE, strlen(SENT_MESSAGE));
    if (ret <= 0) {
        perror("Cannot write to server\n");
    }
}