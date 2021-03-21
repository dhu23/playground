#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>

#define PORT 8080

int main(int argc, char const* argv[])
{
    int server_fd;

    // creating socket file descriptor
    server_fd = socket(AF_INET, SOCK_STREAM, 0);
    if (server_fd == 0)
    {
        perror("Socket failed\n");
        exit(EXIT_FAILURE);
    }
    printf("created socket %d\n", server_fd);

    // forcefully attaching socket to the port 8080
    int opt = 1;
    if (setsockopt(
            server_fd, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, 
            &opt, sizeof(opt)))
    {
        perror("setsockopt reuseaddr and reuseport\n");
        exit(EXIT_FAILURE);
    }
    printf("set the socket to REUSDADDR|REUSEPORT\n");

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);
    if (bind(server_fd, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        perror("bind failed\n");
        exit(EXIT_FAILURE);
    }
    printf("bound to port 8080\n");
    
    if (listen(server_fd, 3) < 0)
    {
        perror("listen failed");
        exit(EXIT_FAILURE);
    }
    printf("listening to port 8080\n");

    int count = 0;
    while (1) {
        int new_socket;
        int addrlen = sizeof(address); // input and output
        printf("waiting for a new connection, i=%d\n", count);
        new_socket = accept(server_fd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
        if (new_socket < 0)
        {
            perror("accept");
            exit(EXIT_FAILURE);
        }
        printf(
            "accepted a new connection and created new socket %d, len=%d\n", 
            new_socket, addrlen);

        char buffer[1024] = {0};

        int valread = read(new_socket, buffer, 1024);
        printf("buffer read in:%s\n", buffer);

        char* hello = "Hello from server";
        send(new_socket, hello, strlen(hello), 0);
        printf("Hello message sent\n");
        ++count;
    }
    return 0;
}
