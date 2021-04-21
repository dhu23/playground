#include <netdb.h>
#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

// some data structures.
//
// The first two are equivalent but sockaddr_in is much easier to 
// manage for programmers. It is later casted to sockaddr
// struct sockaddr {
//     unsigned short sa_family;
//     char sa_data[14]; 
// }
//
// struct sockaddr_in {
//     short sin_family;
//     unsigned short sin_port; // port number
//     struct in_addr sin_addr; // IP address
//     char sin_zero[8];
// }
//
// struct in_addr {
//     unsigned long s_addr; // 4 bytes long
// }
//
// All network data is sent in big endian format
// And this is called Network Byte Order. 
// Native representation on the host as Host Byte Order
// We have to convert all data into network byte order before transmission

// well known services
void run_servent()
{
    struct servent* p = getservbyname("domain", "tcp");
    if (p) 
    {
        printf("name=%s\n", p->s_name);
        char* pc = *(p->s_aliases);
        while (pc)
        {
            printf("alias=%s", pc);
            ++pc;
        }
        printf("port=%d\n", p->s_port);
        printf("proto=%s\n", p->s_proto);
    }
}

void run_inet_aton(char* inet_dot_notation)
{
    // host byte order to network byte order: htons(), htonl()
    // network byte order to host byte order: ntohs(), ntohl()
    //
    // IP address format:
    // ascii dotted to binary: inet_aton()
    // binary to ascii dotted: inet_ntoa()
    int retval;
    struct in_addr addrptr;
    memset(&addrptr, '\0', sizeof(addrptr));
    retval = inet_aton(inet_dot_notation, &addrptr);
    printf("converted %s to %u\n", inet_dot_notation, addrptr.s_addr);

    char* ip;
    ip = inet_ntoa(addrptr);
    printf("IP address is %s\n", ip);
}


// int socket(int domain, int type, int protocol)
// domain = PF_INET (IPv4 communication)
// type = SOCK_STREAM(TCP), SOCK_DGRAM(UDP)
// protocol = 0 (for now)
//
// int bind(int sockfd, struct sockaddr* my_addr, socklen_t addrlen)
// sockfd = socket descriptor returned by socket()
// my_addr = pointer to a valid sockaddr_in structure casted to sockaddr*
// addrlen = length of the sockaddr_in structure
//
// int listen(int sock, int backlog)
// sock = socket returned by socket()
// backlog = maximum length of the pending connections queue
//
//
// int accept(int socket, (struct sockaddr*)&client, socklen_t* client_len)
// socket = socket in listen state
// client = will hold the new client's information when accept returns
// client_len = pointer to size of the client structure
// 
// By client:
// int connect(int sock, (struct sockaddr*)&server_addr, socklen_t len)
// sock = socket returned by socket()
// server_addr = a sockaddr_in struct pointer filled with all the remote
//               server details and cast as a sockaddr struct pointer
// len = size of the server_addr struct
//
// send(), recv(), read(), write()
// int send(int sock, void* mesg, size_t len, int flags)
// int recv(int sock, void* mesg, size_t len, int flags)
// sock = A connected socket
// mesg = pointer to a buffer to send/receive data from/in
// len = size of the message buffer
// flags = 0 (for now)
//
// return value is the number of bytes actually sent/received
//
//
// in close(int sock)
// sock = socket to close


void create_socket()
{
    int fd = socket(
}

int main(int argc, char* argv[])
{
    run_servent();
    // this prints out 222492730, stored in in_addr.s_addr (4 byte)
    // it is translated to the following in binary format:
    // 10000100 10011101 10110010 01000100
    // 132 / 157 / 178 / 68
    run_inet_aton("68.178.157.132");
    return 0;
}
