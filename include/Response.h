#ifndef RESPONSE_H
#define RESPONSE_H
#include <sys/types.h>
#include "UrlParser.h"

struct Response {
    char * response;
    size_t response_len;
};

struct Response *load_resource(struct uriStruct *filename);

#endif