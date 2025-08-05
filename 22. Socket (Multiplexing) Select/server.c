#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <sys/un.h>
#include <sys/select.h>

#define SOCKET_PATH "/tmp/multiplexing"

#define MAX_CLIENTS 3

int clients_fds[MAX_CLIENTS] = {0};

int main()
{
    fd_set readfds;
    int max_fd;
    int buffer_size = 50;
    char buffer[buffer_size];
    int connection_socket = socket(PF_UNIX, SOCK_STREAM, 0);

    struct sockaddr_un addr;

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    unlink(SOCKET_PATH);

    bind(connection_socket, (struct sockaddr *)&addr, (socklen_t)sizeof(addr));

    listen(connection_socket, 3);
    while (1)
    {
        FD_ZERO(&readfds);
        FD_SET(connection_socket, &readfds);
        max_fd = connection_socket;
        for (int i = 0; i < MAX_CLIENTS; i++)
        {
            if (clients_fds[i] == 0){
                continue;
            }
            FD_SET(clients_fds[i], &readfds);
            if (max_fd < clients_fds[i])
            {
                max_fd = clients_fds[i];
            }
        }
        select(max_fd + 1, &readfds, NULL, NULL, NULL);
        // new client connection
        if (FD_ISSET(connection_socket, &readfds)){
            int accepted = 0;
            int new_client = accept(connection_socket, NULL, NULL);
            for(int i = 0; i < MAX_CLIENTS; i++){
                if (clients_fds[i] == 0){
                    clients_fds[i] = new_client;
                    printf("New client Connected\n");
                    accepted = 1;
                    break;
                }
            }   
            if (accepted == 0){
                printf("No space for new client\n");
                // char no_space_message[] = "No space available, declined";
                // write(new_client, no_space_message, sizeof(no_space_message));
                close(new_client);
                printf("Connection Terminated.\n");
            }
            continue;
        }

        for (int i = 0; i < MAX_CLIENTS; i++){
            int cur_client = clients_fds[i];
            if (FD_ISSET(cur_client, &readfds)){
                read(cur_client, buffer, buffer_size);
                buffer[buffer_size-1] = '\0';
                printf("read from server: %s", buffer);
                continue;
            }
        }
    }
    close(connection_socket);
    unlink(SOCKET_PATH);
    return 0;

}