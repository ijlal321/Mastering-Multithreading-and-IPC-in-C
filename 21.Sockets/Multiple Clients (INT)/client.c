#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/un.h>

#define SOCKET_PATH "/tmp/mysocket"

int main()
{
    int connection_socket = socket(PF_UNIX, SOCK_STREAM, 0);
    if (connection_socket == -1)
    {
        perror("Client: Failed to create socket");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_un socket_address;
    memset(&socket_address, 0, sizeof(struct sockaddr_un));
    socket_address.sun_family = AF_UNIX;
    strncpy(socket_address.sun_path, SOCKET_PATH, sizeof(socket_address.sun_path) - 1);

    // 3. Connect to the server
    if (connect(connection_socket, (struct sockaddr *)&socket_address, sizeof(struct sockaddr_un)) == -1)
    {
        perror("Client: Failed to connect to server");
        close(connection_socket);
        exit(EXIT_FAILURE);
    }

    // write
    while (1)
    {
        int input_data;
        scanf("%d", &input_data);
        printf("the user entered %d\n", input_data);
        int write_res = write(connection_socket, &input_data, sizeof(input_data));
        if (write_res <= 0){
            printf("Error writin data to server %d", write_res);
            exit(EXIT_FAILURE);
        }

        if (input_data == 0){
            break;
        }
    }

    // read
    int server_res;
    int read_res = read(connection_socket, &server_res, sizeof(server_res));
    if (read_res <= 0){
        printf("error reading data from server\n");
        exit(EXIT_FAILURE);
    }
    printf("Server Sent: %d\n", server_res);

    // 6. Cleanup
    close(connection_socket);
    return 0;
}