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
#include "StartServer.h"

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
    kickup_server(initialRoute);
    return 0;
}
