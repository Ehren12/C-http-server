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
#include "Response.h"
#include "Routes.h"
#include "UrlParser.h"
#include "constants.h"

void *kickup_server(struct RouteNode *initialRoute)
{
    int client_socketfd;
    HTTPserver httpServer;
    start_server(&httpServer, PORT);
    struct sockaddr_storage their_addr;
    char s[INET6_ADDRSTRLEN];

    for (;;)
    {
        char client_msg[BUFFER_SIZE] = "";
        int sin_size = sizeof their_addr;
        client_socketfd = accept(httpServer.socket, (struct sockaddr *)&their_addr, &sin_size);
        if (client_socketfd == -1)
        {
            perror("accept");
            continue;
        }

        inet_ntop(their_addr.ss_family, get_in_addr((struct sockaddr *)&their_addr), s, sizeof s);
        printf("Server got connection from %s\n", s);

        if (read(client_socketfd, client_msg, BUFFER_SIZE) == -1)
        {
            perror("read");
            continue;
        }
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
        if (method == NULL || urlRoute == NULL || method == "" || urlRoute == "")
            continue;
        struct uriStruct *parsedUrl = urlParser(urlRoute, initialRoute, client_socketfd, method);
        if (parsedUrl == NULL)
            continue;
        struct Response *resource_payload = load_resource(parsedUrl);
        if (!fork())
        {
            close(httpServer.socket);
            if (send(client_socketfd, resource_payload->response, resource_payload->response_len, 0) == -1)
                perror("send");
            free(parsedUrl);
            free(resource_payload);
            close(client_socketfd);
            exit(0);
        }

        close(client_socketfd);
    }
}