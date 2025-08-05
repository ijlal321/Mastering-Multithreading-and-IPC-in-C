#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include<sys/socket.h>
#include<string.h>
#include<unistd.h>
#include <sys/un.h>
#include <sys/select.h>

#define SOCKET_PATH "/socket/multiplexing"

#define MAX_CLIENTS 3

int clients_fds[MAX_CLIENTS];

int main(){
    fd_set readfds;
    int max_fd; 
    int connection_socket = socket(PF_UNIX, SOCK_STREAM, 0);

    struct sockaddr_un addr;
    
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path)-1);

    unlink(SOCKET_PATH);

    bind(connection_socket, (struct sockaddr *)&addr, (socklen_t)sizeof(addr));

    listen(connection_socket, 3);

    while(1){
        FD_ZERO(&readfds);
        FD_SET(connection_socket, &readfds);
        max_fd = connection_socket;
        for (int i = 0; i < MAX_CLIENTS; i++){
            if (clients_fds[i] == 0) continue;
            FD_SET(clients_fds[i], &readfds);
            if (max_fd < clients_fds[i]){
                max_fd = clients_fds[i];
            }
        }

    }

    
}