#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <unistd.h>
#include "Response.h"
#include "constants.h"
#include "MimeTypes.h"

static char *get_mime_type(char *file_ext);

struct Response *load_resource(struct uriStruct *filename)
{
    char *mime_type = get_mime_type(filename->file_ext);
    dealloc_mimes(initializeMime("html", "text/html"));

    char *resp_header = (char *)malloc(BUFFER_SIZE * sizeof(char));
    sprintf(resp_header, "HTTP/1.1 200 OK\r\nContent-Type: %s\r\n\r\n", mime_type);

    struct Response *response = malloc(sizeof(struct Response));
    char *response_data = (char *)malloc(BUFFER_SIZE * 2 * sizeof(char));
    memset(response_data, 0, BUFFER_SIZE * 2 * sizeof(char));
    printf("mime_type: %s\n", mime_type);
    int html_data = open(filename->filePath, O_RDONLY);
    if (strcmp(filename->file_ext, ".ico") == 0 && html_data == -1)
    {
        printf("i exec\n");
        sprintf(response_data, "HTTP/1.1 204 No Content \r\n\r\n");
        response->response = response_data;
        response->response_len = strlen(response->response);
        free(resp_header);
        free(response_data);
        close(html_data);
        return response;
    }
    if (html_data == -1)
    {
        sprintf(response_data, "HTTP/1.1 404 Not Found\r\nContent-Type: text/plain\r\n\r\n404 Not Found");
        response->response = response_data;
        response->response_len = strlen(response->response);
        free(resp_header);
        free(response_data);
        close(html_data);
        return response;
    }

    size_t response_data_len = 0;
    lseek(html_data, 0, SEEK_SET);
    struct stat file_stat;
    fstat(html_data, &file_stat);
    off_t file_size = file_stat.st_size;

    memcpy(response_data, resp_header, strlen(resp_header));
    response_data_len += strlen(resp_header);

    ssize_t bytes_read;
    while ((bytes_read = read(html_data, response_data + response_data_len, BUFFER_SIZE - response_data_len)) > 0)
    {
        response_data_len += bytes_read;
    }
    response_data[response_data_len] = '\0';
    response->response = response_data;
    response->response_len = response_data_len;
    free(resp_header);
    free(response_data);
    close(html_data);
    return response;
}

static char *get_mime_type(char *file_ext)
{
    struct MimeType *initMime = initializeMime("html", "text/html");
    addMime(initMime, "css", "text/css");
    addMime(initMime, "ico", "image/vnd.microsoft.icon");
    addMime(initMime, ".ico", "image/vnd.microsoft.icon");
    addMime(initMime, "jpeg", "image/jpeg");
    addMime(initMime, "jpg", "image/jpeg");
    addMime(initMime, "js", "text/javascript");
    addMime(initMime, "json", "application/json");
    addMime(initMime, "txt", "text/plain");

    struct MimeType *searched_mime = searchMime(initMime, file_ext);
    if (searched_mime == NULL)
    {
        return "MIME_NOT_FOUND";
    }
    return searched_mime->value;
}

