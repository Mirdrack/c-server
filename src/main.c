#include "server.h"
#include <errno.h>
#include <netinet/in.h>
#include <pthread.h>
#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

enum
{
    PORT = 8080
};

enum
{
    MAX_CONNECTIONS = 10
};

volatile sig_atomic_t keep_running = 1;

void handle_signal(int sig)
{
    if (sig == SIGINT || sig == SIGTERM)
    {
        keep_running = 0;
    }
}

void setup_signal_handler()
{
    struct sigaction signal_action;
    memset(&signal_action, 0, sizeof(signal_action));
    signal_action.sa_handler = handle_signal;
    // Important: do NOT set SA_RESTART so syscalls are interrupted
    sigaction(SIGINT, &signal_action, NULL);
    sigaction(SIGTERM, &signal_action, NULL);
}

int main()
{
    setup_signal_handler();

    printf("Hello C server...\n");

    int server_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in server_addr;
    int bind_operation = -1; // Bind should change it to 0

    // Create server sockets
    if (server_fd < 0)
    {
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
    if (listen(server_fd, MAX_CONNECTIONS) < 0)
    {
        perror("Listen for connections failed");
        exit(EXIT_FAILURE);
    }

    // Hadling connections
    while (keep_running)
    {
        // Client info
        struct sockaddr_in client_addr;
        socklen_t client_addr_len = sizeof(client_addr);
        int *client_fd = malloc(sizeof(int));

        // Accept client connection
        *client_fd = accept(server_fd, (struct sockaddr *)&client_addr, &client_addr_len);
        if (*client_fd < 0)
        {
            if (errno == EINTR)
            {
                // Interrupted by signal, check keep_running
            }
            else
            {
                perror("Accept failed");
            }
            free(client_fd);
            continue;
        }

        // Create new thread to handle the request
        pthread_t thread_id = {0};
        pthread_create(&thread_id, NULL, handle_client, (void *)client_fd);
        pthread_detach(thread_id);
    }
    close(server_fd);
    printf("Server shut down gracefully\n");

    return 0;
}
