/*
** "helpers.c" contains useful helper functions to be used when creating a web server.
** The functions here are specifically to be used by the http server.
*/

#include <errno.h>
#include <netinet/in.h>
#include <signal.h>
#include <arpa/inet.h>
#include <sys/wait.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

// Returns address of a given socket. 
// Compatible with IPv4 and IPv6
void *
get_in_addr(struct sockaddr *sa)
{
    if (sa->sa_family == AF_INET)
    {
        return &(((struct sockaddr_in*)sa)->sin_addr);
    }
    return &(((struct sockaddr_in6*)sa)->sin6_addr);
}


// SIGACTION HELPERS
/* 
** Responsible for killing (reaping) all child
** processes that could possibly still be alive,
** taking system resources 
*/

// FUNC
void
sigchild_handler(int s)
{
    int saved_errno = errno;
    while (waitpid(-1, NULL, WNOHANG) > 0);
    errno = saved_errno;
}

// FUNC
void
kill_zombie_processes()
{
    struct sigaction zombie_killer_action;
    zombie_killer_action.sa_handler = sigchild_handler;
    sigemptyset(&zombie_killer_action.sa_mask);
    if (sigaction(SIGCHLD, &zombie_killer_action, NULL) == -1)
    {
        perror("sigaction");
        exit(1);
    }
}