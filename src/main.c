#include <stdio.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <pthread.h>


#define PORT 8080

int main() {
    printf("Hello C server...\n");

    int server_fd;
    struct sockaddr_in server_addr;
    int bind_operation;

    // Create server sockets
    if((server_fd = socket(AF_INET, SOCK_STREAM, 0))) {
        perror("Server socket creation failed");
        exit(EXIT_FAILURE);
    }

    // Configuration
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = INADDR_ANY;
    server_addr.sin_port = htons(PORT);

    bind_operation = bind(server_fd, (struct sockaddr *)&server_addr, sizeof(server_addr));
    if (bind_operation < 0)
    {
        perror("Binding socket to port failed");
    }

    // Listen for connections
    if (listen(server_fd, 10) < 0) {
        perror("Listen for connections failed");
        exit(EXIT_FAILURE);
    }

    // Hadling connections
    while (1) {
        // Client info
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int *client_fd = malloc(sizeof(int));

        // Accept client connection
        *client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (*client_fd < 0)
        {
            perror("Accept failed");
            continue;
        }

    }

    return 0;
}
