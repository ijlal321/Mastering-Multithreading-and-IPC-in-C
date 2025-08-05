#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include<sys/socket.h>
#include<sys/un.h>
#include<sys/select.h>
#include<unistd.h>



#define SOCKET_PATH "/tmp/multiplexing"

int main(){
    int connection_socket = socket(PF_UNIX, SOCK_STREAM, 0);
    int bufffer_size = 50;
    char buffer[bufffer_size];

    struct sockaddr_un addr;
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strncpy(addr.sun_path, SOCKET_PATH, sizeof(addr.sun_path));
    
    int connection_res = connect(connection_socket, (struct sockaddr *)&addr, sizeof(addr));
    if (connection_res != 0){
        printf("Connection falied\n");
        exit(EXIT_FAILURE);
    }

    int is_successful;
    read(connection_socket, &is_successful, sizeof(int));
    if (is_successful == 0){
        printf("Looks like no place for new client, bye\n");
        exit(EXIT_FAILURE);
    }

    printf("COnnection made successfully\n");

    while(1){
        printf("Enter data: ");
        fgets(buffer, bufffer_size, stdin);

        buffer[bufffer_size-1] = '\0';

        printf("data being send to server is: %s \n", buffer);

        write(connection_socket, buffer, strlen(buffer));
        printf("data sent to server successful\n");
        
        read(connection_socket, buffer, strlen(buffer));
        printf("data received from server back is: %s\n", buffer);
        
        sleep(1);
    }
    
}
