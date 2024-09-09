#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define LIMIT_QUEUE_LEN   5
#define MAX_CONNECTION    10
#define MAX_SENT_BUFF     100   

typedef struct sockaddr sockaddr;
typedef struct
{
    uint8_t ip_address;
    uint32_t port_no;
    int fd;
} socket_info_t;

int main(int argc, char *argv[])
{
    int port_no = 0;
    int client_fd = 0;
    int curent_client_number = 0;
    struct sockaddr_in server_addr = {};
    socklen_t sock_len = 0;
    socket_info_t client_info[MAX_CONNECTION];
    char sent_message[MAX_SENT_BUFF];

    if (argc != 3)
    {
        perror("The program should be run with destination ip address and port number as a 2nd and 3rd parameters");
        return -1;
    }

    // create socket
    if((client_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        perror("Cannot create new socket");
        return -1;
    }

    // get port no
    port_no = atoi(argv[2]);
    // reset value
    memset(&server_addr, 0, sizeof(server_addr)); 

    // config server parameter
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port_no);
    if(inet_pton(AF_INET, argv[1], &server_addr.sin_addr) == -1)
    {
        perror("Cannot use destination ip address");
        return -1;
    }
    
    // // bind socket file descriptor with config parameters
    // if (bind(client_fd, (sockaddr *)(&server_addr), sizeof(server_addr)) < 0)
    // {
    //     perror("Cannot bind socket with config parameters");
    //     return -1;
    // }

    // if (listen(client_fd, LIMIT_QUEUE_LEN) < 0)
    // {
    //     perror("Cannot set socket server to listen mode");
    //     return -1;
    // }


    while(1)
    {
        printf("Server is listening at port: %d\n", port_no);

        if((connect(client_fd, (sockaddr *)&server_addr, &sock_len)) < 0)
        {
            perror("Cannot accept new connection");
            return -1;
        }

        memset(sent_message, 0, MAX_SENT_BUFF);
      
        send(client_fd, sent_message, strlen(sent_message), 0);
        printf("Message was sent: %s", sent_message);

    }
    close(client_fd);
    return 0;
    
    
}
