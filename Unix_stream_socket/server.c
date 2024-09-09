#include <stdio.h>
#include <string.h>
#include <sys/un.h>
#include <unistd.h>
#include <sys/socket.h>


#define SERVER_SOCK_PATH  "./server_path"
#define MESSAGE  "Xin chao, toi la Ngo Truong Long\0"

int main(int argc, char* argv[])
{
    int socket_fd = 0;
    int client_fd = 0;
    struct sockaddr_un unix_addr;
    char read_buffer[100];
    int ret = 0;

    unlink(SERVER_SOCK_PATH);

    // create endpoint for communicate
    socket_fd = socket(AF_UNIX, SOCK_STREAM, 0);
    if (socket_fd == -1) {
        perror("Cannot create socket for communicate\n");
        return -1;
    }

    // config for unix socket address
    memset(&unix_addr, 0, sizeof(unix_addr));
    unix_addr.sun_family = AF_UNIX;
    strncpy(unix_addr.sun_path, SERVER_SOCK_PATH, sizeof(unix_addr) - 1);

    /* assign a name to server socket
    server need to be assigned a path for it's socket for listen the connection
    SERVER: create a server + assign name (path to socket file) for this server
    CLIENT: only need to know the path to server for perform connect */
    ret = bind(socket_fd, (struct sockaddr *)&unix_addr, sizeof(unix_addr));
    
    if(ret == -1) {
        perror("Cannot bind socket name to socket__fd");
        return ret;
    }

    // mark socket_fd to passive socket => it will be used to accept incoming connection
    ret = listen(socket_fd, 5);
    if (ret == -1) {
        printf("Cannot mark socket_fd %d", socket_fd);
    }

    printf("bind successfully\n");

    client_fd = accept(socket_fd, NULL, NULL);
    if (client_fd == -1) {
        printf("Error connection with client\n");
        return -1;
    }

    printf("Accept connect from client_fd %d\n", client_fd);

    // read data from client_fd
    ret = read(client_fd, read_buffer, sizeof(read_buffer));
    if(ret > 0) {
        printf("Read %d bytes from client_fd %d successfully\n", ret, client_fd);
    }
    printf("Read from client_fd: %d, message: %s\n", client_fd, read_buffer);

    // write data to client
    ret = write(client_fd, MESSAGE, strlen(MESSAGE));
    if(ret > 0) {
        printf("Write %d byte to client_fd %d successfully\n", ret, client_fd);
    }

    close(client_fd);
    close(socket_fd);
    unlink(SERVER_SOCK_PATH);

    return 0;
 
}