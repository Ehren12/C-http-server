#include <netinet/in.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <sys/socket.h>
#include <sys/types.h>

#include <unistd.h>

#define SIZE 1024

int main(void)
{
    // Get and store our html data
    FILE *html_data;
    html_data = fopen("index.html", "r");
    if (html_data == NULL)
    {
        printf("File doesnt exsist\n");
        return 1;
    }
    char response_data[SIZE];
    fgets(response_data, SIZE, html_data);
    printf("SENT: %s", response_data);
    // HTTP header
    char http_header[SIZE * 2] = "HTTP/1.1 200 OK\r\n\n";
    strcat(http_header, response_data);
    // Create out socket
    int socketfd;
    socketfd = socket(AF_INET, SOCK_STREAM, 0);
    // describe our socket address
    struct sockaddr_in server_address;
    server_address.sin_addr.s_addr = INADDR_ANY;
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(5001);

    bind(socketfd, (struct sockaddr *)&server_address, sizeof server_address);
    listen(socketfd, 3);

    int new_fd;
    while (1)
    {
        char client_msg[4096] = "";
        new_fd = accept(socketfd, NULL, NULL);

        read(new_fd, client_msg, 4095);
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
        send(new_fd, &http_header, sizeof http_header, 0);
        close(new_fd);
    }

    return 0;
}