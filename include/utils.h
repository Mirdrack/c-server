#ifndef UTILS_H
#define UTILS_H

void *handle_client(void *arg);
void build_http_response(const char * filename,
    const char *file_ext,
    char *reponse,
    size_t *response_len);
char *url_decode(const char *src);
const char *get_file_extention(const char *filename);
const char *get_mime_type(const char *file_ext);

#endif // UTILS_H
