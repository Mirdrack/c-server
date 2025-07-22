#include <stdio.h>
#include "utils.h"
#include <regex.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>

#define BUFFER_SIZE 104857600

void *handle_client(void *arg) {
    printf("Hello client thread!\n");

    int client_fd = *((int *)arg);
    char *buffer = (char *)malloc(BUFFER_SIZE * sizeof(char));

    ssize_t bytes_received = recv(client_fd, buffer, BUFFER_SIZE, 0);

    if (bytes_received > 0) {
        // Check if the request is a GET
        regex_t regex;
        regcomp(&regex, "^GET /([^ ]*) HTTP/1", REG_EXTENDED);
        regmatch_t matches[2];

        if (regexec(&regex, buffer, 2, matches, 0) == 0)
        {
            // Extract filename from request and decode URL
            buffer[matches[1].rm_eo];
            const char *url_encoded_file_name = buffer + matches[1].rm_so;
            char *file_name = url_decode(url_encoded_file_name);

            // Get file extention
            char file_ext[32];
            strcpy(file_ext, get_file_extention(file_name));

            // Build HTTP Response
            char *response = (char *)malloc(BUFFER_SIZE * 2 * sizeof(char));
            size_t response_len;
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


void build_http_response(const char *filename,
    const char *file_ext,
    char *reponse,
    size_t *response_len) {

    // Build HTTP Header
    const char *mime_type = get_mime_type(file_ext);
}


char *url_decode(const char *src) {
    size_t src_len = strlen(src);
    char *decoded = malloc(src_len + 1);
    size_t decoded_len = 0;

    // Decode %2 to hex
    for (size_t i = 0; i < src_len; i++) {
        if (src[i] == '%' && i + 2 < src_len) {
            int hex_val;
            sscanf(src + i + 1, "%2x", &hex_val);
            decoded[decoded_len++] = hex_val;
        }
        else {
            decoded[decoded_len++] = src[i];
        }

    }

    // Add null terminator to the decoded string
    decoded[decoded_len] = '\0';
    return decoded;

}


const char *get_file_extention(const char *filename) {
    const char *dot = strchr(filename, '.');
    if (!dot || dot == filename) {
        return "";
    }
    return dot + 1;

}

struct supported_mime_types {
    const char *ext;
    const char *mime;
};

const char *get_mime_type(const char *file_ext) {
    struct supported_mime_types mime_types[] = {
        { "html", "text/html" },
        { "htm",  "text/html" },
        { "txt",  "text/plain" },
        { "jpg",  "image/jpeg" },
        { "jpeg", "image/jpeg" },
        { "png",  "image/png" },
    };

    size_t num_types = sizeof(mime_types) / sizeof(mime_types[0]);

    for (size_t i = 0; i < num_types; ++i) {
        if (strcasecmp(file_ext, mime_types[i].ext) == 0) {
            return mime_types[i].mime;
        }
    }

    return "application/octet-stream"; // Default MIME type

}
