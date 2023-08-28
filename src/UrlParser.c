#include "Routes.h"
#include "Response.h"
#include "UrlParser.h"
#include "constants.h"

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
    if (url == NULL)
    {
        perror("urlParser malloc");
        return NULL;
    }
    char viewRootDir[1000];
    strcpy(viewRootDir, ROOT_DIR);

    url->method = method;
    if (strstr(route, ".css") != NULL || strstr(route, ".js") != NULL)
    {
        strcat(viewRootDir, route);
        url->filePath = viewRootDir;
        url->statusCode = SUCCESS;
        url->file_ext = "css";
        return url;
    }

    struct RouteNode *destination = searchRoutes(initialRoute, route);
    if (destination == NULL)
    {
        url->filePath = NON_EXSISTENT_ROUTE;
        url->statusCode = NOT_FOUND;
        url->file_ext = SUCCESS;

        return url;
    }
    strcat(viewRootDir, destination->value);
    url->filePath = viewRootDir;
    url->statusCode = SUCCESS;
    url->file_ext = get_filename_ext(destination->value);
    printf("\n%s\n", url->filePath);
    return url;
}