#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>

#define PORT 8080

int main(int argc, char const* argv[])
{
    int sock = 0;
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0)
    {
        perror("\n socket creation error\n");
        return -1;
    }
    printf("socket created: %d\n", sock);

    struct sockaddr_in serv_addr;
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(PORT);
    if (inet_pton(AF_INET, "127.0.0.1", &serv_addr.sin_addr) <= 0)
    {
        perror("\nInvalid address/address not supported\n");
        return -1;
    }
    // serv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
    if (connect(sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) < 0)
    {
        perror("\nConnection failed\n");
        return -1;
    }

    char* hello = "Hello from client";
    send(sock, hello, strlen(hello), 0);
    printf("Hello message from client was sent\n");

    char buffer[1024] = {0};
    int valread = read(sock, buffer, 1024);
    printf("from server:%s\n", buffer);

    return 0;
}
