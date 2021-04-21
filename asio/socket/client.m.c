#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <netdb.h>
#include <netinet/in.h>

#include <string.h>


int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        fprintf(stderr, "usage %s hostname port\n", argv[0]);
        exit(0);
    }

    int portno = atoi(argv[2]);
    int sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0)
    {
        perror("ERROR opening socket");
        return 1;
    }

    struct hostent* server = gethostbyname(argv[1]);
    if (server == NULL)
    {
        fprintf(stderr, "EEROR, no such host\n");
        return 1;
    }

    // convert names from user input to server information structure
    struct sockaddr_in serv_addr;
    bzero((char*)&serv_addr, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    bcopy((char*)server->h_addr, 
          (char*)&serv_addr.sin_addr.s_addr, 
          server->h_length);
    serv_addr.sin_port = htons(portno);

    // connect to server
    if (connect(sockfd, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("ERROR connecting");
        return 1;
    }

    // ask for a message from the user, this message will be read by server
    printf("please enter the message:\n");

    char buffer[256];
    bzero(buffer, 256);
    fgets(buffer, 255, stdin);

    int n = write(sockfd, buffer, 255);
    if (n < 0)
    {
        perror("ERROR writing to socket\n");
        return 1;
    }

    bzero(buffer, 256);
    n = read(sockfd, buffer, 255);
    if (n < 0)
    {
        perror("ERROR reading from socket\n");
        return 1;
    }

    printf("%s\n", buffer);
    return 0;
}
