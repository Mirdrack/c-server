#ifndef SERVER_H
#define SERVER_H

#include <stddef.h>

void *handle_client(void *arg);
void build_http_response(const char * filename,
    const char *file_ext,
    char *reponse,
    size_t *response_len);

#endif // SERVER_H
