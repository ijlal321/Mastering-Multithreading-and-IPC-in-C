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
        char input_data[50];
        printf("enter your data: ");
        fgets(input_data, sizeof(input_data), stdin);

        /*
            fget explanation. it will always read n-1 size input. and attach \n in end if more size.

            data[5]
            input -> 123456enter
            data-> 1,2,3,4,\0     (rest is still in buffer 5,6,\n)

            input -> 1,2,enter
            data-> 1,2,\n,\0,\0

        */

        // Check if newline is included in input data
        char *newline = strchr(input_data, '\n');
        if (newline) // newline in input data, meaning no overflow
        {
            *newline = '\0'; // Replace newline with null terminator
        }
        else // in case of overflow, flush extra
        {
            int ch;
            while ((ch = getchar()) != '\n' && ch != EOF)
                ; // Flush stdin
        }

        printf("You entered: \"%s\"\n", input_data);

        int write_res = write(connection_socket, input_data, strlen(input_data) + 1);
        // +1 to include null terminator too (as strlen dont include it)

        if (write_res <= 0)
        {
            printf("Error writin data to server %d", write_res);
            exit(EXIT_FAILURE);
        }

        if (strcmp(input_data, "0") == 0)
        {
            break;
        }
    }

    // read
    int server_res;
    int read_res = read(connection_socket, &server_res, sizeof(server_res));
    if (read_res <= 0)
    {
        printf("error reading data from server\n");
        exit(EXIT_FAILURE);
    }
    printf("Server Sent: %d\n", server_res);

    // 6. Cleanup
    close(connection_socket);
    return 0;
}