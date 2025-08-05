#include<stdlib.h>
#include<stdio.h>
#include<unistd.h>
#include<sys/socket.h>
#include<sys/select.h>
#include<sys/un.h>
#include <string.h>

#define SOCKET_PATH "/tmp/multiplexing"

int main(){
    int connection_socket = socket(PF_UNIX, SOCK_STREAM, 0);
    
    struct sockaddr_un addr;

    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path) - 1);

    int connect_res = connect(connection_socket, (struct sockaddr *)&addr, sizeof(struct sockaddr_un));    
    if (connect_res <= 0){
        printf("Cannot connect to server\n");
        exit(EXIT_FAILURE);
    }
    while(1){
        char input_message[50];
        fgets(input_message, sizeof(input_message), stdin);
        // always check for 1. overflow, also remove \n from input.
        // brute for now
        input_message[sizeof(input_message)-1] = '\0';
        printf("data to sent is: %s\n", input_message);
        write(connection_socket, &input_message, strlen(input_message));

        printf("data sent successfuly\n");
    }
}