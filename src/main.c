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

#define SIZE 1024
#define PORT "5000"
#define BACKLOG 10

void *get_in_addr(struct sockaddr *sa);

int main(void)
{
    // Load HTML unto server
    FILE *html_data;
    html_data = fopen("view/index.html", "r");
    if (html_data == NULL)
    {
        printf("File doesnt exsist\n");
        return 1;
    }
    
    char response_data[SIZE];
    fgets(response_data, SIZE, html_data);
    fclose(html_data);
    // HTTP header
    char http_header[SIZE * 2] = "HTTP/1.1 200 OK\r\n\n";
    strcat(http_header, response_data);
    // Create out socket
    int server_socketfd, client_socketfd;
    struct addrinfo hints, *server_info, *p;
    struct sockaddr_storage their_addr;
    struct sigaction zombie_killer_action;
    socklen_t sin_size;
    int yes=1;
    int status;
    char s[INET6_ADDRSTRLEN];

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;
    server_socketfd = socket(AF_INET, SOCK_STREAM, 0);
    
    if ((status = getaddrinfo(NULL, PORT, &hints, &server_info)) != 0)
    {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(status));
        return 1;
    }

    for (p = server_info; p != NULL; p = p->ai_next)
    {
        if ((server_socketfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol)) == -1)
        {
            perror("server: socket");
            continue;
        }

        if (setsockopt(server_socketfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1)
        {
            perror("setsockopt");
        }

        if (bind(server_socketfd, p->ai_addr, p->ai_addrlen) == -1)
        {
            perror("server: bind");
            continue;
        }

        break;
    }

    freeaddrinfo(server_info);
    
    if (p == NULL)
    {
        fprintf("server: bind\n");
        exit(1);
    }

    if (listen(server_socketfd, BACKLOG) == -1)
    {
        perror("listen");
        exit(1);
    }

    zombie_killer_action.sa_handler = sigchild_handler;
    sigemptyset(&zombie_killer_action.sa_mask);
    if (sigaction(SIGCHLD, &zombie_killer_action, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }

    printf("Waiting for connections...");

    while (1)
    {
        char client_msg[4096] = "";
        int sin_size = sizeof their_addr;
        client_socketfd = accept(server_socketfd, (struct sockaddr *)&their_addr, &sin_size);
        if (client_socketfd == -1)
        {
            printf("Trouble accepting connection");
            continue;
        }

        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
        printf("Server got connection from %s\n", s);

        read(client_socketfd, client_msg, 4095);
        // parsing client socket header to get HTTP method, route
        char *method = "";
        char *urlRoute = "";

        char *client_http_header = strtok(client_msg, "\n");
        char *header_token = strtok(client_http_header, " ");
        int header_parse_counter = 0;

        while (header_token != NULL)
        {

            switch (header_parse_counter)
            {
            case 0:
                method = header_token;
            case 1:
                urlRoute = header_token;
            }
            header_token = strtok(NULL, " ");
            header_parse_counter++;
        }
        printf("The method is %s\n", method);
        printf("The route is %s\n", urlRoute);
        if (!fork())
        {
            close(server_socketfd);
            if(send(client_socketfd, &http_header, sizeof http_header, 0) == -1)
                printf("Error sending html");
            close(client_socketfd);
            exit(0);
        }
        close(client_socketfd);
    }

    return 0;
}

void *
get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}