#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/un.h> // gives sockaddr_un

#define SOCKET_PATH "/tmp/mysocket"

int main(){
    int connection_socket = socket(PF_UNIX, SOCK_STREAM, 0);
    if (connection_socket == -1) {
        perror("Client: Failed to create socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_un socket_address;
    memset(&socket_address, 0, sizeof(struct sockaddr_un));
    socket_address.sun_family = AF_UNIX;
    strncpy(socket_address.sun_path, SOCKET_PATH, sizeof(socket_address.sun_path) - 1);

        // 3. Connect to the server
    if (connect(connection_socket, (struct sockaddr*)&socket_address, sizeof(struct sockaddr_un)) == -1) {
        perror("Client: Failed to connect to server");
        close(connection_socket);
        exit(EXIT_FAILURE);
    }


    // write
    const char *message = "Hello from client!";
    int write_res = write(connection_socket, message, strlen(message));  // '\0' is not sent unless we add +1
    if (write_res == -1) {
        perror("Client: Failed to send message");
        close(connection_socket);
        exit(EXIT_FAILURE);
    }


    // read
        char buffer[100];
    int read_res = read(connection_socket, buffer, sizeof(buffer) - 1);  // Leave space for '\0'
    if (read_res > 0) {
        buffer[read_res] = '\0';  // Null-terminate manually
        printf("Client received: %s\n", buffer);
    } else if (read_res == 0) {
        printf("Client: Server closed the connection.\n");
    } else {
        perror("Client: Failed to read from server");
    }


    // 6. Cleanup
    close(connection_socket);
    return 0;



}