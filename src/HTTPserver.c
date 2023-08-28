#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <signal.h>
#include "HTTPserver.h"
#include "helpers.h"
#define BACKLOG 10

// This function starts a HTTP (actually TCP) server.
// PORT is a port on the SERVER which is of type HTTPserver *. 
void 
start_server(HTTPserver *server, char* port)
{
    // Define starter variables.
    struct addrinfo hints, *server_info, *p;
    socklen_t sin_size;
    int yes=1;
    int status;
    // Make sure all properties of hints is set to 0.
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    // Use getaddrinfo function to populate linked list of possible sockets.    
    if ((status = getaddrinfo(NULL, port, &hints, &server_info)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
    }
    // Iterate over linked list and connect to the first good one.
    for (p = server_info; p != NULL; p = p->ai_next)
    {
        if ((server->socket = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("server: socket");
            continue;
        }

        if (setsockopt(server->socket, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            perror("setsockopt");
        }

        if (bind(server->socket, p->ai_addr, p->ai_addrlen) == -1)
        {
            close(server->socket);
            perror("server: bind");
            continue;
        }

        break;
    }
    // Free linked list
    freeaddrinfo(server_info);
    // Error handeling
    if (p == NULL)
    {
        fprintf(stderr, "server: bind\n");
        exit(1);
    }
    // Listen to binded port
    if (listen(server->socket, BACKLOG) == -1)
    {
        perror("listen");
        exit(1);
    }
    // Kill zombie processes
    kill_zombie_processes();

    printf("Waiting for connections...\n");
}