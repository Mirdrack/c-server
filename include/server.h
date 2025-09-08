#ifndef SERVER_H
#define SERVER_H

#include <stddef.h>

enum
{
    BUFFER_SIZE = 104857600
};

enum
{
    MAX_FILE_EXT_LEN = 32
};

void *handle_client(void *arg);
void build_http_response(const char *file_name, const char *file_ext, char *response,
                         size_t *response_len);

#endif // SERVER_H
