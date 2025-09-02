#include "utils.h"

#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *url_decode(const char *src)
{
    size_t src_len = strlen(src);
    char *decoded = malloc(src_len + 1);
    if (!decoded)
    {
        return NULL;
    }

    size_t decoded_len = 0;

    for (size_t i = 0; i < src_len; i++)
    {
        if (src[i] == '%' && i + 2 < src_len)
        {
            unsigned int hex_val = 0;
            if (sscanf(src + i + 1, "%2x", &hex_val) == 1)
            {
                decoded[decoded_len++] = (char)hex_val;
                i += 2; // skip two hex digits
                continue;
            }
        }
        decoded[decoded_len++] = src[i];
    }

    decoded[decoded_len] = '\0';
    return decoded;
}

const char *get_file_extention(const char *filename)
{
    const char *dot = strrchr(filename, '.');
    if (!dot || dot == filename)
    {
        return "";
    }
    return dot + 1;
}

struct supported_mime_types
{
    const char *ext;
    const char *mime;
};

const char *get_mime_type(const char *file_ext)
{
    struct supported_mime_types mime_types[] = {
        {"html", "text/html"}, {"htm", "text/html"},   {"txt", "text/plain"},
        {"jpg", "image/jpeg"}, {"jpeg", "image/jpeg"}, {"png", "image/png"},
    };

    size_t num_types = sizeof(mime_types) / sizeof(mime_types[0]);

    for (size_t i = 0; i < num_types; ++i)
    {
        if (strcasecmp(file_ext, mime_types[i].ext) == 0)
        {
            return mime_types[i].mime;
        }
    }

    return "application/octet-stream"; // Default MIME type
}
