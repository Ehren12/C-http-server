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

int main(void)
{
    // Load HTML unto server

    struct RouteNode *initialRoute = initializeRoute("/", "index.html");

    addRoute(initialRoute, "/about", "about.html");
    addRoute(initialRoute, "/favicon.ico", "favicon.ico");
    addRoute(initialRoute, "/cat", "cat.jpeg");
    addRoute(initialRoute, "/jango", "jango.txt");
    in_order_trav(initialRoute);
    // Create out socket
    int client_socketfd;
    HTTPserver httpServer;
    start_server(&httpServer, PORT);
    struct sockaddr_storage their_addr;
    char s[INET6_ADDRSTRLEN];

    while (1)
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

        read(client_socketfd, client_msg, BUFFER_SIZE);
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
        struct Response *html = load_resource(parsedUrl);
        if (!fork())
        {
            close(httpServer.socket);
            if (send(client_socketfd, html->response, html->response_len, 0) == -1)
                perror("send");
            free(parsedUrl);
            free(html);
            close(client_socketfd);
            exit(0);
        }

        close(client_socketfd);
    }
    dealloc_tree((struct RouteNode *)initializeRoute);
    return 0;
}
