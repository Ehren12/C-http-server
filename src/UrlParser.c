#include "Routes.h"
#include "Response.h"
#include "UrlParser.h"

#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

char *get_filename_ext(char *filename)
{
    char *dot = strrchr(filename, '.');
    if (!dot || dot == filename)
        return "";
    return dot + 1;
}

struct uriStruct *urlParser(char *route, struct RouteNode *initialRoute, int client_socketfd, char *method)
{
    struct uriStruct *url = malloc(sizeof(struct uriStruct));
    char viewRootDir[1000];
    if (viewRootDir == NULL)
        return NULL;
    strcpy(viewRootDir, "view/");

    url->method = method;
    if (strstr(route, ".css") != NULL || strstr(route, ".js") != NULL)
    {
        strcat(viewRootDir, route);
        url->filePath = viewRootDir;
        url->statusCode = "200 OK";
        url->file_ext = "css";
        return url;
    }

    struct RouteNode *destination = searchRoutes(initialRoute, route);
    if (destination == NULL)
    {
        url->filePath = "OWLSERVER_NO_ROUTE";
        url->statusCode = "404 Not Found";
        url->file_ext = "NULL";

        return url;
    }
    strcat(viewRootDir, destination->value);
    url->filePath = viewRootDir;
    url->statusCode = "200 OK";
    url->file_ext = get_filename_ext(destination->value);
    printf("\n%s\n", url->filePath);
    return url;
}