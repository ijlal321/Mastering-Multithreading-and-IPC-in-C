// server.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <netinet/in.h>

#define PORT 8080

void *client_handler(void *arg) {
    int client_socket = *(int *)arg;
    free(arg);
    char buffer[1024] = {0};

    read(client_socket, buffer, sizeof(buffer));
    printf("Received from client: %s\n", buffer);

    char *response = "Message received";
    send(client_socket, response, strlen(response), 0);

    close(client_socket);
    return NULL;
}

void *listener_thread(void *arg) {
    int server_fd, new_socket;
    struct sockaddr_in address;
    int addrlen = sizeof(address);

    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0) {
        perror("Socket failed");
        pthread_exit(NULL);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address)) < 0) {
        perror("Bind failed");
        close(server_fd);
        pthread_exit(NULL);
    }

    if (listen(server_fd, 3) < 0) {
        perror("Listen failed");
        close(server_fd);
        pthread_exit(NULL);
    }

    printf("Server is listening on port %d...\n", PORT);

    while (1) {
        new_socket = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen);
        if (new_socket < 0) {
            perror("Accept failed");
            continue;
        }

        pthread_t tid;
        int *pclient = malloc(sizeof(int));
        *pclient = new_socket;

        pthread_create(&tid, NULL, client_handler, pclient);
        pthread_detach(tid);  // Auto-cleanup after thread finishes
    }
}

int main() {
    pthread_t listener;
    pthread_create(&listener, NULL, listener_thread, NULL);

    pthread_join(listener, NULL);  // Wait forever (or Ctrl+C to stop)
    return 0;
}