#ifndef HTTP_SERVER
#define HTTP_SERVER

typedef struct HTTPserver 
{
    int socket;
    char* port;
} HTTPserver;

void start_server(HTTPserver *server, char* port);
#endif