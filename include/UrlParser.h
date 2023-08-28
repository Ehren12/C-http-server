
#ifndef URLPARSER_H
#define URLPARSER_H
#include "Routes.h"

struct uriStruct
{
    char *filePath;
    char *statusCode;
    char *method;
    char *file_ext;
};

struct uriStruct *urlParser(char *route, struct RouteNode *initialRoute, int client_socketfd, char *method);

#endif