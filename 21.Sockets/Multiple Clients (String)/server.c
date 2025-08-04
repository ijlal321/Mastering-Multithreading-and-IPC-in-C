#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h> // meed for multi-threading
#include <sys/socket.h>

#include <stdint.h> // for intptr_t

// defines the struct sockaddr_un used with UNIX domain sockets
#include <sys/un.h>

#define SOCKET_PATH "/tmp/mysocket"
// string size cannot be greater than 108 - See sockaddr_un implementation.
#define MAX_NR_CLIENTS 3

typedef struct {
    int client_socket;
    int client_id;
} ClientData;


void *thread_callbk(void *args)
{
    ClientData * client_data = (ClientData *)args;

    printf("Client %d is connected and waiting for messages in new thread\n", client_data->client_id);


    while(1){
        char buffer[50];
        int size = read(client_data->client_socket, &buffer, sizeof(buffer));
        if (size <= 0){
            printf("some issue reading data from client\n");
            exit(EXIT_FAILURE);
        }

        // always terminating string. Even in case actual string in small.
        buffer[sizeof(buffer) - 1] = '\0';  

        if (strcmp(buffer, "0") == 0){
            printf("Reached the end of line...byeeee\n");
            break;
        }
        printf("buffer we got is: %s \n", buffer);
    }
/*
    int write_res = write(client_data->client_socket, &sum, sizeof(sum));
    if (write_res <= 0){
        printf("error writing to client\n");
        exit(EXIT_FAILURE);
    }*/

    close(client_data->client_socket);
    return NULL;
}


int main()
{
    int connection_socket = socket(PF_UNIX, SOCK_STREAM, 0);

    struct sockaddr_un sock_addr;

    memset(&sock_addr, 0, sizeof(struct sockaddr_un));
    sock_addr.sun_family = AF_UNIX;
    strncpy(sock_addr.sun_path, SOCKET_PATH, sizeof(sock_addr.sun_path));

    unlink(SOCKET_PATH);

    bind(connection_socket, (struct sockaddr *)&sock_addr, sizeof(struct sockaddr));

    listen(connection_socket, MAX_NR_CLIENTS);

    pthread_t threads[MAX_NR_CLIENTS];
    pthread_attr_t thread_attr;
    pthread_attr_init(&thread_attr);
    pthread_attr_setdetachstate(&thread_attr, PTHREAD_CREATE_JOINABLE);
    
    socklen_t addr_len = sizeof(sock_addr); // need to pass to accept fn

    for (int i = 0; i < MAX_NR_CLIENTS; i++)
        {
        ClientData * new_client_data = malloc(sizeof(ClientData));
        new_client_data->client_socket = accept(connection_socket, (struct sockaddr *)&sock_addr, &addr_len);
        new_client_data->client_id = i;

        pthread_create(&threads[i], &thread_attr, thread_callbk, (void *)new_client_data);
        }
    
    pthread_attr_destroy(&thread_attr);

    for (int i = 0; i < MAX_NR_CLIENTS; i++){
        pthread_join(threads[i], NULL);
    }
    
    close(connection_socket);
    unlink(SOCKET_PATH);
}
