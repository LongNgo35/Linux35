#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define LIMIT_QUEUE_LEN   5
#define MAX_CONNECTION    10
#define MAX_READ_BUFF     100   

typedef struct sockaddr sockaddr;
typedef struct
{
    uint8_t ip_address;
    uint32_t port_no;
    int fd;
} socket_info_t;

void chat_with_client(int client_fd)
{
    int num_read = 0;
    char read_buffer[MAX_READ_BUFF];

    memset(read_buffer, 0, MAX_READ_BUFF);
    while(1)
    {
        num_read = read(client_fd, (void *)read_buffer, MAX_READ_BUFF);
        if (num_read == -1)
        {
            perror("Cannot read from client\n");
        }
        if (strcmp(read_buffer, "exit") == 1)
        {
            break;
        }
        printf("Client's message: %s", read_buffer);
    }
}


int main(int argc, char *argv[])
{
    int port_no = 0;
    int server_fd = 0;
    int client_fd = 0;
    int curent_client_number = 0;
    struct sockaddr_in server_addr = {};
    struct sockaddr_in client_addr = {};
    socklen_t sock_len = 0;
    socket_info_t client_info[MAX_CONNECTION];

    if (argc != 2)
    {
        perror("The program should be run with port number as a 2nd parameter");
        return -1;
    }

    // create socket
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Cannot create new socket");
        return -1;
    }

    // get port no
    port_no = atoi(argv[1]);
    // reset value
    memset(&server_addr, 0, sizeof(server_addr));
    memset(&client_addr, 0, sizeof(client_addr));

    // config server parameter
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_no);
    server_addr.sin_addr.s_addr = INADDR_ANY;
    
    // bind socket file descriptor with config parameters
    if (bind(server_fd, (sockaddr *)(&server_addr), sizeof(server_addr)) < 0)
    {
        perror("Cannot bind socket with config parameters");
        return -1;
    }

    if (listen(server_fd, LIMIT_QUEUE_LEN) < 0)
    {
        perror("Cannot set socket server to listen mode");
        return -1;
    }

    while(1)
    {
        printf("Server is listening at port: %d\n", port_no);

        if((client_fd = accept(server_fd, (sockaddr *)&client_addr, &sock_len)) < 0)
        {
            perror("Cannot accept new connection");
            return -1;
        }

        // save client info
        client_info[curent_client_number].fd = client_fd;
        inet_ntop(AF_INET, &client_addr.sin_addr, client_info[curent_client_number].ip_address, INET_ADDRSTRLEN);
        client_info[curent_client_number].port_no = port_no;
        curent_client_number++;

        // call a funtion
        chat_with_client(client_fd);

    }
    close(server_fd);
    return 0;
    
    
}
