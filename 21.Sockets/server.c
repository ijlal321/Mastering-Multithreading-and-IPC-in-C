#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>

#include <sys/un.h> // whats this header ?
 // defines the struct sockaddr_un used with UNIX domain sockets

#define SOCKET_PATH "/tmp/mysocket" // Each path means a channel for communicating. No Duplicate.

int main()
{
    // create a new server socket for unix commmunication
    int connection_socket = socket(PF_UNIX, SOCK_STREAM, 0);
        /*           int socket(int domain, int type, int protocol);
        PF_UNIX -> Protocol Family UNIX,	Describes the family of protocols the socket uses
        later we use AF_UNIX -> Address Family	Describes the format of addresses (e.g., UNIX paths, IP+port)
        but we can also use AF_UNIX here and it will work, bcz both are just integer 1 macro. 
        2nd Parameter — type -> SOCK_STREAM (TCP) or SOCK_DGRAM(UDP) etc
        3rd Parameter — protocol -> sually set to 0 to let the OS choose the default protocol for the given domain/type pair. 
        */

    if (connection_socket == -1){
        perror("Socket Creation Failed.");
        exit(EXIT_FAILURE);
    }

    struct sockaddr_un socket_Address;
    memset(&socket_Address, 0, sizeof(struct sockaddr_un));
    socket_Address.sun_family = AF_UNIX;
    // strcpy(socket_Address.sun_path, SOCKET_PATH);
    strncpy(socket_Address.sun_path, SOCKET_PATH, sizeof(socket_Address.sun_path) - 1); // better than strcpy. Bcz not buffer overflow issue.

    unlink(SOCKET_PATH);


    int bind_res = bind(connection_socket, (struct sockaddr *)&socket_Address, sizeof(struct sockaddr_un)); // or sizeof(socket_Address) will also work. Same thing
    
    if (bind_res == -1){
        perror("Binding Socket Failed");
        close(connection_socket); // close is same close used to close files. Bcz ofc its a file descriptor at the end
        exit(EXIT_FAILURE);
    }
    
    /*       big Question
    Why do I cast &socket_Address to (struct sockaddr *) in bind() or 
    connect() (from client.c) when it's already a struct sockaddr_un * ?
    Ans:  because the function expects a pointer to a more generic type: struct sockaddr *,
    not struct sockaddr_un *.

    Not really a big issue if you dont cast. But just professional.
    */

    int listen_res = listen(connection_socket, 3); // explanation after accept()
    if (listen_res == -1){
        perror("Failed Listening to Socket");
        close(connection_socket); 
        exit(EXIT_FAILURE);
    }


    
    int client_socket = accept(connection_socket, NULL, NULL);
    if (client_socket == -1){
        perror("Failed accepting Client Connnectino");
        close(connection_socket); 
        exit(EXIT_FAILURE);
    }

    /*
    listen() doesn't block. It just sets up the queue for incoming connections.
    accept() is the blocking call — it waits until a client connects.
            or 
    Listen tells function that tells how many clients can wait.
    Why wait ? bcz no client is entered yet unless we call accept().
            or 
    This of like listen as a limit to persons who can stand behind door.
    Accept takes 1 person inside door

    Accept is also blocking.
    */

    char buffer[50];
    int read_res = read(client_socket, buffer, 50);    
    if (read_res > 0) {
        if (read_res >= sizeof(buffer)){
            printf("Buffer overflow while reading from client. Terminating early");
            buffer[sizeof(buffer) - 1] = '\0';
        }else{
            buffer[read_res] = '\0';   // need to check buffer flow here
        }
        printf("Received from client: %s\n", buffer);
    }else{
        perror("Failed read data from client");
        close(connection_socket); 
        exit(EXIT_FAILURE);
    }

    const char * response  = "This is message from Server. Listen it well";
    write(client_socket, response, strlen(response));

    /*
    read and write return -1 if error
    = 0 if (EOF) client closed socket something.
    >0 --> bytes loaded

    ** also remember, most strings dont have \0 in end. So you need to manually write. **
    */

    close(client_socket);
    close(connection_socket);
    unlink(SOCKET_PATH); 

    return 0;
}
