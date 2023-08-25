#include <netinet/in.h>

#include <stdio.h>
#include <stdlib.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <unistd.h>

#define PORT 5001
#define BACKLOG 5

int
main (void)
{
    // Create a socket.
    int server_socketfd;
    int client_socketfd;
    server_socketfd = socket(AF_INET, SOCK_STREAM, 0);
    // Specify our server address
    struct sockaddr_in server_address;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(PORT);
    // Bind to a port
    if ((bind(server_socketfd, (struct sockaddr *)&server_address, sizeof server_address)) == -1)
    {
        printf("========== BINDING ERROR ==========\n");
        printf("Could'nt bind to port %d\n\n", PORT);
        close(server_socketfd);
        return 1;
    }
    // Liten on that port.
    listen(server_socketfd, BACKLOG);
    // Accept client and store in client socket
    client_socketfd = accept(server_socketfd, NULL, NULL);
    // Cretae data buffer and send data to it
    char *data_buff = "You have reached me";
    send(client_socketfd, &data_buff, sizeof data_buff, 0);
    // Close socket
    close(server_socketfd);

    return 0;
}