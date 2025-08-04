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

int Client_Socket[MAX_NR_CLIENTS];

void *thread_callbk(void *args)
{
    int client_nr = (int)(intptr_t)args;
    int client_socket = Client_Socket[client_nr];

    printf("Client %d is connected and waiting for messages in new thread\n", client_nr);

    int sum = 0;
    while(1){
        int buffer;
        int size = read(client_socket, &buffer, sizeof(buffer));
        if (size <= 0){
            printf("some issue reading data from client\n");
            exit(EXIT_FAILURE);
        }
        if (buffer == 0){
            printf("Total sum is 100\n");
            break;
        }
        printf("buffer we got is: %d \n", buffer);
    }

    int write_res = write(client_socket, &sum, sizeof(sum));
    if (write_res <= 0){
        printf("error writing to client\n");
        exit(EXIT_FAILURE);
    }

    close(client_socket);
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
        Client_Socket[i] = accept(connection_socket, (struct sockaddr *)&sock_addr, &addr_len);

        pthread_create(&threads[i], &thread_attr, thread_callbk, (void *)(intptr_t)(i));

    }
    
    pthread_attr_destroy(&thread_attr);

    for (int i = 0; i < MAX_NR_CLIENTS; i++){
        pthread_join(threads[i], NULL);
    }
    
    close(connection_socket);
    unlink(SOCKET_PATH);
}
