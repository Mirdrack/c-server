#include <stdio.h>
#include "utils.h"

void *handle_client(void *arg) {
    printf("Hello client thread!\n");
}
