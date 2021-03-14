#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <netdb.h>
#include <netinet/in.h>

#include <sys/wait.h>
#include <sys/types.h>

#include <string.h>

#define MAX_COUNT 200

void child_process(void)
{
    for (int i = 0; i < MAX_COUNT; ++i)
    {
        printf("     this line is from child, value=%d\n", i);
    }
    printf("     *** child process is done.\n");
}

void parent_process(void)
{
    int i;
    for (int i = 0; i < MAX_COUNT; ++i)
    {
        printf("this line is from parent, value=%d\n", i);
    }
    printf("*** parent is done.\n");
}


void do_processing(int sock)
{
    char buffer[256];
    bzero(buffer, 256);
    int n = read(sock, buffer, 255);

    if (n < 0)
    {
        perror("ERROR reading from socket");
        exit(1);
    }

    printf("Here is a message:%s\n", buffer);
    n = write(sock, "I got your message\n", 18);

    if (n <0)
    {
        perror("ERROR writing to socket");
        exit(1);
    }
}

void handle_child_exit(int signum)
{
    wait(NULL);
}

int main(int argc, char* argv[])
{
    printf("starting up server...\n");
    int sockfd = socket(AF_INET, SOCK_STREAM, 0); // TCP socket
    if (sockfd < 0) 
    {
        perror("ERROR opening socket\n");
        return 1;
    }
    printf("created socket fd successfully:%d\n", sockfd);

    struct sockaddr_in serv_addr;
    int portno = 5001;

    serv_addr.sin_family = AF_INET;
    // from stackoverflow: you typically want to bind to all interfaces
    // if you only want for localhost, then use
    // serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    serv_addr.sin_addr.s_addr = INADDR_ANY;
    serv_addr.sin_port = htons(portno);

    printf("binding socket to %d call\n", portno);
    if (bind(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR on binding\n");
        return 1;
    }
    printf("bound successfully\n");

    // now start listening for clients and here process will go in 
    // sleep mode and will wait for the incoming connection
    if (listen(sockfd, 5)) // 5 in the backlog
    {
        perror("ERROR on listening\n");
        return 1;
    }
    printf("listening on port %d\n", portno);


    struct sockaddr_in cli_addr;
    int clilen = sizeof(cli_addr);

    while (1) {
        // accept actual connection from the client. program blocks here
        // until a client connection
        //
        // From accept(2) man page:
        //
        // If no pending connections are present on the queue, and the socket
        // is not marked as non-blocking, accept() blocks the caller until a 
        // connection is present. If the socket is marked non-blocking and no
        // pending connections are present on the queue, accept() fails with 
        // the error EAGAIN or EWOULDBLOCK
        //
        // in order to be notified of incoming connections on a socket, you can 
        // use select(2), poll(2) or epoll(7). A readable event will be 
        // delivered when a new connection is attempted and you may then call
        // accept() to get a socket for that connection. Alternatively, you 
        // can get the socket to deliver SIGIO when activity occurs on a 
        // socket; see socket(7) for details
        //
        printf("waiting to accept connection\n");
        int newsockfd = accept(sockfd, (struct sockaddr*)&cli_addr, &clilen);
        if (newsockfd < 0)
        {
            perror("ERROR on accept\n");
            return 1;
        }
        printf("after accept call\n");

        // system call fork() is used to create processes. It takes no 
        // arguments and returns a process ID. The purpose of fork() is to
        // create a new process, which becomes the child process of the 
        // caller. After a new child process is created, BOTH processes 
        // will execute the next instruction following the fork system 
        // call. Therefore we have to distinguish the parent from the child
        // this can be done by testing the returned value of fork
        //
        // Both processes start their execution right after fork(). Since 
        // both processes have identical but separate address spaces, 
        // those variables initialized BEFORE the fork() call have the 
        // same values in both address spaces. 
        int pid = fork();

        // when the main program executes fork(), and identical copy of its
        // address space including the program and all data is created. 
        // System call fork() returns the child process ID to the parent 
        // and returns 0 to the child process. The following figure shows
        // that in both address spaces there is a valid pid. The one in the
        // parent receives the child process ID and the one in the child
        // receives 0.
        if (pid < 0)
        {
            perror("ERROR on fork");
            return 1;
        }

        // since data is copied to child process, the child process would
        // close the socket that was created in the main process. And the 
        // main process would close the connection from the client and let
        // the child process to handle it.
        if (pid == 0)
        {
            // this is the client process that works on the request
            close(sockfd);
            do_processing(newsockfd);
            exit(0);
        }
        else
        {
            // this is the parent process
            close(newsockfd);
            printf("child process pid=%d\n", pid);

            // this following handles the child process exit thus it leaves
            // no defunct child process in the system
            signal(SIGCHLD, handle_child_exit);
            printf("just handled a child exit\n");
        }
    }
    return 0;
}
