#ifndef UTILS_H
#define UTILS_H

void *handle_client(void *arg);
void build_http_response();
char *url_decode(const char *src);
const char *get_file_extention(const char *filename);

#endif // UTILS_H
