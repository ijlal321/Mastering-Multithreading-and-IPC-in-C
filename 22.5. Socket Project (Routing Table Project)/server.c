#include<stdlib.h>
#include<stdio.h>
#include<stdbool.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<sys/select.h>
#include<unistd.h>

#define MAX_CLINETS 3
#define SOCKET_PATH "/tmp/multiplexing"

int Client_Sockets[MAX_CLINETS + 1] = {-1}; // meaning non initialized
    // need +1 for master socket

int main(){
    fd_set fdset, master_set;
    int max_fds;
    int bufffer_size = 50;
    char buffer[bufffer_size];
    int connection_socket = socket(PF_UNIX, SOCK_STREAM, 0);

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path));

    unlink(SOCKET_PATH);


    bind(connection_socket,(struct sockaddr *)&addr, sizeof(addr));
    listen(connection_socket,3);

    FD_ZERO(&master_set);  // vv imp step. Never forget to init your fd.
    FD_SET(connection_socket, &master_set);
    max_fds = connection_socket;
    printf("Starting loop \n");
    while(1){
        FD_ZERO(&fdset);
        fdset = master_set;

        select(max_fds + 1, &fdset, NULL, NULL, NULL);
        printf("Select passed \n");
        // check for new connection
        if (FD_ISSET(connection_socket, &fdset)){
            int new_client = accept(connection_socket, NULL, NULL);
            bool is_space_available = 0;
            for (int i = 0; i < MAX_CLINETS; i++){
                if (Client_Sockets[i] == -1){
                    Client_Sockets[i] = new_client;
                    FD_SET(new_client, &master_set);
                    is_space_available = true;
                    if (max_fds < new_client){
                        max_fds = new_client;
                    }
                    printf("New Connection Made Successful. \n");
                    break;
                }
            }

            if (is_space_available){
                int write_data = 1; // connection successful from server side.
                write(new_client, &write_data, sizeof(int));
            }else{
                printf("No space available for new connection. Closing \n");
                int write_data = 0; // meaning no space available, disconnect.
                write(new_client, &write_data, sizeof(int));
                close(new_client);
            }
            continue;
        } 

        // old connection talk
        for (int i = 0; i < MAX_CLINETS; i++){
            int cur_client = Client_Sockets[i];
            if (FD_ISSET(cur_client, &fdset)){
                read(cur_client, buffer, bufffer_size-1);
                buffer[bufffer_size-1] = '\0';
                printf("Data received: %s \n", buffer);
                write(cur_client, buffer, strlen(buffer));
            }   
        }
    }

    close(connection_socket);
    unlink(SOCKET_PATH);

}
