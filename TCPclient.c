#include <netinet/in.h>

#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <unistd.h>


int
main(void)
{
    // Create our socket
    int socketfd;
    socketfd =  socket(AF_INET, SOCK_STREAM, 0);
    // Specify our socket address (server)
    struct sockaddr_in server_address;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(3490);
    // Connect to socket (server)
    // connect and store connection status in variable
    int connection_status = connect(socketfd, (struct sockaddr *)&server_address, sizeof server_address);
    // Handle error
    if (connection_status < 0)
    {
        printf("========== CONNECTION ERROR ==========\n");
        printf("message: Could'nt connect to server socket, try again later...\n\n");
        close(socketfd);
        return 1;
    }
    // Receive data from server
    char data_buff[256];
    recv(socketfd, &data_buff, sizeof data_buff, 0);
    // Log data
    printf("========== MESSAGE FROM SERVER 🐽 ==========\n");
    printf("message: %s\n\n", data_buff);
    printf("%d", socketfd);
    // close socket
    close(socketfd);
    return 0;
}
