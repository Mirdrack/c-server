#include <server.h>

#include <fcntl.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>

#include "utils.h"

#define BUFFER_SIZE 104857600
#define MAX_FILE_EXT_LEN 32

void build_http_response(const char *file_name, const char *file_ext, char *response,
                         size_t *response_len)
{

    // Build HTTP header
    const char *mime_type = get_mime_type(file_ext);
    char *header = (char *)malloc((size_t)BUFFER_SIZE * sizeof(char));
    snprintf(header, BUFFER_SIZE,
             "HTTP/1.1 200 OK\r\n"
             "Content-Type: %s\r\n"
             "\r\n",
             mime_type);

    printf("D: %s\n", file_ext);
    printf("D: %s\n", file_name);

    // If file doesn't exist, response with a 404
    int file_fd = open(file_name, O_RDONLY);
    if (file_fd == -1)
    {
        snprintf(response, BUFFER_SIZE,
                 "HTTP/1.1 404 Not Found\r\n"
                 "Content-Type: text/plain\r\n"
                 "\r\n"
                 "404 Not Found");
        *response_len = strlen(response);
        free(header);
        return; // Function must return void
    }

    // Get file size for Content-Length
    struct stat file_stat;
    fstat(file_fd, &file_stat);
    // off_t file_size = file_stat.st_size;

    // Copy header to response buffer
    *response_len = 0;
    memcpy(response, header, strlen(header));
    *response_len = strlen(header);

    // Copy file to response buffer
    ssize_t bytes_read = 0;
    while ((bytes_read = read(file_fd, response + *response_len, BUFFER_SIZE - *response_len)) > 0)
    {
        *response_len += bytes_read;
    }
    free(header);
    close(file_fd);
}

void *handle_client(void *arg)
{
    printf("New client thread!\n");

    int client_fd = *((int *)arg);
    char *buffer = (char *)malloc((size_t)BUFFER_SIZE * sizeof(char));

    ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);

    if (bytes_received > 0)
    {
        // Check if the request is a GET
        regex_t regex;
        regcomp(&regex, "^GET /([^ ]*) HTTP/1", REG_EXTENDED);
        regmatch_t matches[2];

        if (regexec(&regex, buffer, 2, matches, 0) == 0)
        {
            // Extract filename from request and decode URL
            buffer[matches[1].rm_eo] = '\0';
            const char *url_encoded_file_name = buffer + matches[1].rm_so;
            char *file_name = url_decode(url_encoded_file_name);

            // Get file extention
            char file_ext[MAX_FILE_EXT_LEN];
            strcpy(file_ext, get_file_extention(file_name));

            // Build HTTP Response
            char *response = (char *)malloc((size_t)BUFFER_SIZE * 2 * sizeof(char));
            size_t response_len = 0;
            build_http_response(file_name, file_ext, response, &response_len);
            send(client_fd, response, response_len, 0);

            free(response);
            free(file_name);
        }
        regfree(&regex);
    }
    close(client_fd);
    free(arg);
    free(buffer);
    return NULL;
}
