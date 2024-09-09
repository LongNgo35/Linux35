#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <net/if.h>
#include <arpa/inet.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <pthread.h>
#include <unistd.h>

#define DEFAULT_IP_ADDRESS         "172.22.48.1"
#define CONNECTED_CLIENT_NUMBERS   100
typedef struct {
    char ip_address[16];
    uint16_t port_number;
} connected_client_t;

uint32_t server_fd = 0;
uint32_t client_fd = 0;
uint32_t current_connected_client = 0;
connected_client_t connected_client[100];


void perform_help()
{
    printf("\n------USER INTERFACE LIST------\n");
    printf("1. help\n");
    printf("2. myip\n");
    printf("3. myport\n");
    printf("4. connect\n");
    printf("5. list\n");
    printf("6. terminate\n");
    printf("7. send\n");
    printf("8. exit\n");
}

int perform_myip(int server_fd)
{
    // local variable 
    struct ifreq ifr;
    char socket_ip[16];
    int ret = 0;

    // get ipv4 address
    ifr.ifr_addr.sa_family = AF_INET;
    memcpy(ifr.ifr_name, "ens33", IFNAMSIZ - 1);
    ioctl(server_fd, SIOCGIFADDR, &ifr);
    strcpy(socket_ip, inet_ntoa(((struct sockaddr_in*)&ifr.ifr_addr)->sin_addr));
   
    // display information
    printf("\nIp address: %s\n", socket_ip);
   
    return 0;
}

int perform_myport(int server_fd)
{
    struct sockaddr_in server_addr = {};
    int len = sizeof(server_addr);
    int socket_port = 0;
    int ret = 0;

    // get socket port value
    ret = getsockname(server_fd, (struct sockaddr *)&server_addr, &len);
    if(ret == -1) {
        perror("Get socket name and");
        return ret;
    }
    socket_port = ntohs(server_addr.sin_port);

    printf("Port: %d\n", socket_port);
    return 0;
}

int perform_connect()
{
    // declaration local variable
    uint8_t ip_destination[16];
    uint16_t port_no = 0;
    struct sockaddr_in socket_server;

    // get input value
    printf("Enter the destination: ");
    scanf("%s", ip_destination);
    printf("Enter the port no: ");
    scanf("%d", &port_no);

    memset(&socket_server, 0, sizeof(socket_server));
    
    // validata ip address
    socket_server.sin_family = AF_INET;
    socket_server.sin_port = htons(port_no);
    if(inet_pton(AF_INET, DEFAULT_IP_ADDRESS, &socket_server.sin_addr) == -1)
    {
        perror("Invalid address/ address not support\n");
        exit(EXIT_FAILURE);
        return -1;
    }

    // perform connecting
    if(connect(client_fd, (struct sockadd*)&socket_server, sizeof(socket_server)) < 0)
    {
        perror("Connection failed");
        printf("Destiation ip address: %s\n", ip_destination);
        printf("Port no: %d\n", port_no);
        close(client_fd);
        return -1;
    }

    // save connected address
    memcpy(connected_client[current_connected_client].ip_address, ip_destination, strlen(ip_destination));
    connected_client[current_connected_client].port_number = port_no;
    current_connected_client++;

    printf("Connect to the server successfully\n");
}

void *listen_thread_handle(void *arg1)
{
    struct sockaddr_in client_address;
    socklen_t client_socket_len = sizeof(client_address);
    while (1)
    {
        if((new_socket = accept(server_fd, (struct sockaddr *)(&client_address), &client_socket_len)) < 0)
        {
            perror("Accept failed\n");
        };

        // get ip address & port number
        inet_ntop(AF_INET, &client_address.sin_addr, connected_client[current_connected_client].ip_address, INET_ADDRSTRLEN);
        connected_client[current_connected_client].port_number = ntohs(client_address.sin_port);
    }
    
}

void perform_list()
{ 
    if (current_connected_client > 0)
    {
        uint32_t connection = 0;
        printf("id: IP address              Port No.");
        for (connection = 0; connection < current_connected_client; connection++)
        {
            printf("%d: %s                      %d\n", connection, connected_client[connection].ip_address, connected_client[connection].port_number);
        }
    }
    else
    {
        printf("There is no connection avaiable\n");
    }
}

int main(int argc, char* argv[])
{
    // local variable
    int port_number = 0;
    int option = 0;
    struct sockaddr_in server_ipv4_config;
    struct sockaddr_in client_ipv4_config;
    int ret = 0;
    pthread_t socket_listen_thread;

    //close(server_fd);

    /* 1. validate input parameter*/
    if (argc != 2) {
        perror("The number of argument is in valid\n");
    }
    // get port number
    port_number = atoi(argv[1]);

    /* 2. get socket file descriptor */
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(server_fd == -1) {
        perror("Cannot create endpoint for the socket\n");
        return 0;
    }

    client_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(client_fd == -1) {
        perror("Cannot create endpoint for the socket\n");
        return 0;
    }

    printf("Server fd value: %d\n", server_fd);

    /* 3. config socket parameter */
    server_ipv4_config.sin_family = AF_INET;
    server_ipv4_config.sin_port = htons(port_number);
    server_ipv4_config.sin_addr.s_addr = INADDR_ANY;

    client_ipv4_config.sin_family = AF_INET;
    client_ipv4_config.sin_port = htons(1234);
    client_ipv4_config.sin_addr.s_addr = INADDR_ANY;

    /* 4. assign socket address */
    ret = bind(server_fd, (const struct sockaddr *)(&server_ipv4_config), sizeof(server_ipv4_config));
    if (ret < 0) {
        perror("Cannot assign address for ipv4 socket\n");
        return ret;
    }

    ret = bind(client_fd, (const struct sockaddr *)(&client_ipv4_config), sizeof(client_ipv4_config));
    if (ret < 0) {
        perror("Cannot assign address for ipv4 socket\n");
        return ret;
    }
    
    /* 5. set listen mode */
    ret = listen(server_fd, 5);
    if (ret == -1) {
        perror("Cannot set listen mode for socket\n");
        return -1;
    }

    printf("Config successfully, server is in listening mode and waiting for a connection from client\n");

    //pthread_create(&socket_listen_thread, NULL, listen_thread_handle, NULL);

    while(1) {
        printf("\nEnter your option: ");
        scanf("%d", &option);

        switch (option)
        {
            case 1:
                perform_help();
                break;
            case 2:
                perform_myip(server_fd);
                break;
            case 3:
                perform_myport(server_fd);
                break;
            case 4:
                perform_connect();
                break;
            case 5:
                perform_list();
                break;
            case 8:
                exit(0);
            default:
                printf("Invalid option, please try with option value from 1 to 8\n");
                break;
        }
    }

    //close(server_fd);
    return 0;
}
