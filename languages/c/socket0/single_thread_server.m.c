#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/time.h>

#define TRUE 1
#define FALSE 0
#define PORT 8888

int main(int argc, char* argv[])
{
    int max_clients = 30;
    int client_socket[30];
    for (int i = 0; i < max_clients; ++i)
    {
        client_socket[i] = 0; // initialize all sockets
    }

    // create a master socket (listening socket)
    int master_socket = socket(AF_INET, SOCK_STREAM, 0);
    if (master_socket == 0)
    {
        perror("socket failed\n");
        exit(EXIT_FAILURE);
    }
    
    // set master socket to allow multiple connections
    // this is just a good habit, it will work without this
    int opt = TRUE;
    // if (setsockopt(
    //         master_socket, SOL_SOCKET, SO_REUSEADDR, 
    //         (char*)&opt, sizeof(opt)) < 0)
    // {
    //     perror("setsockopt error\n");
    //     exit(EXIT_FAILURE);
    // }

    struct sockaddr_in address;
    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons(PORT);

    // bind the socket to 8888
    if (bind(master_socket, (struct sockaddr*)&address, sizeof(address)) < 0)
    {
        perror("bind failed\n");
        exit(EXIT_FAILURE);
    }
    if (listen(master_socket, 3) < 0)
    {
        perror("listen failed\n");
        exit(EXIT_FAILURE);
    }
    
    printf("listening to 8888, fd=%d\n", master_socket);
    // accept the incoming connection
    int addrlen = sizeof(address);
    puts("Waiting for connections...");

    // a set of socket descriptors
    fd_set readfds;
    char* message = "Single-threaded server: ECHO Daemon v1.0 \r\n";
    char buffer[1025];

    while (TRUE)
    {
        // clear the socket set
        FD_ZERO(&readfds);

        // add master socket to set
        FD_SET(master_socket, &readfds);

        int max_sd = master_socket;
        // add child sockets to set
        for (int i = 0; i < max_clients; ++i)
        {
            int sd = client_socket[i];
            if (sd > 0) { FD_SET(sd, &readfds); }
            if (sd > max_sd) { max_sd = sd; }
        }

        printf("max_sd is %d\n", max_sd);
        // wait for an activity on one of the sockets, timeout is NULL,
        // so it waits indefinitely
        if (select(max_sd+1, &readfds, NULL, NULL, NULL) < 0 && errno != EINTR)
        {
            printf("select error\n");
        }

        // see if something happened on the master socket, if that's the case
        // it is an incoming connection
        if (FD_ISSET(master_socket, &readfds))
        {
            int new_socket = accept(
                master_socket, (struct sockaddr*)&address, (socklen_t*)&addrlen);
            if (new_socket < 0)
            {
                perror("accept failed\n");
                exit(EXIT_FAILURE);
            }

            printf(
                "New connection, sockfd=%d, ip=%s, port=%d\n",
                new_socket, 
                inet_ntoa(address.sin_addr), ntohs(address.sin_port));

            if (send(new_socket, message, strlen(message), 0) != strlen(message))
            {
                perror("send message to new socket failed\n");
            }

            puts("welcome message sent successfully");

            for (int i = 0; i < max_clients; ++i)
            {
                // if position is empty, use it to store the new socket
                if (client_socket[i] == 0)
                {
                    client_socket[i] = new_socket;
                    printf("Adding to list of sockets as %d\n", i);
                    break;
                }
            }
        }

        // otherwise, it is some IO operation on some other socket
        for (int i = 0; i < max_clients; ++i)
        {
            int sd = client_socket[i];
            if (FD_ISSET(sd, &readfds))
            {
                // check if it was for closing, and also read the incoming msg
                int valread = read(sd, buffer, 1024);
                if (valread == 0)
                {
                    getpeername(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
                    printf("Host disconnected, peer ip=%s, peer port=%d\n",
                           inet_ntoa(address.sin_addr), 
                           ntohs(address.sin_port));

                    getsockname(sd, (struct sockaddr*)&address, (socklen_t*)&addrlen);
                    printf("Host disconnected, sock ip=%s, sock port=%d\n",
                           inet_ntoa(address.sin_addr),
                           ntohs(address.sin_port));

                    close(sd);
                    client_socket[i] = 0;
                }
                else
                {
                    buffer[valread] = '\0';
                    printf("read in:%s\n", buffer);
                    send(sd, buffer, strlen(buffer), 0);
                }
            }
        }
    }
    return 0;
}
