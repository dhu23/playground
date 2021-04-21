#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// int getaddrinfo(
//     const char *nodename, const char* servname,
//     const struct addrinfo *hints, struct addrinfo **res);

// the first parameter for getaddrinfo can be either a host name, like
// "wwww.example.com", or an IPv4/IPv6 address, passed as a string. 
// The can also be NULL if you are using the AI_PASSIVE flag, which is 
// the case in this code snippet.
//
// the 2nd parameter, servname parameter is basically the port number. 
// It can be a port number(passed as a string, like "80") or it can be
// service name, like "http", "tftp", "smtp", or "pop", etc.
// Well-known service name can be found in /etc/services file
//
//
// In hints:
//
// 1) ai_flag can be set using bitwise-or
//        AI_CANONNAME could cause ai_canonname of the result to be filled 
//        with the host's canonical(real) name. 
//        AI_PASSIVE causes the result's IP address to be filled out with 
//        INADDR_ANY(IPv4) or in6addr_any(IPv6)
// 2) ai_family
//        you'll likely want to set ai_family to AF_UNSPEC which tells 
//        getaddrinfo() to look for both IPv4 and IPv6 addresses. You can
//        restrict yourself to either AF_INET or AF_INET6.
// 3) the socktype field should be set to SOCK_STREAM or SOCK_DGRAM.
// 4) just leave ai_protocol at 0 to automatically choose your protocol type
//
// 
// void freeaddrinfo(struct addrinfo *ai);
// const char *gai_strerror(int ecode);
//
//
// struct addrinfo {
//   int ai_flags; // AI PASSIVE, AI_CANONNAME, ...
//   int ai_family; // AF_xxx
//   int ai_socktype; // SOCK_xxx
//   int ai_protocol; // 0 (auto) or IPPROTO_TCP, IPPROTO_UPD
//
//   socklen_t ai_addrlen; // length of ai_addr
//   char *ai_canonname;
//   struct sockaddr *ai_addr; // binary address
//   struct addrinfo *ai_next; // next structure in linked list
// };
//
// as a reference
// struct sockaddr {
//   unsigned short sa_family;
//   char sa_data[14];
// };
//
// we can cast it to 
// struct sockaddr_in {
//   short int sin_family;
//   unsigned short int sin_port; // a 16 bit port number in network byte order
//   struct in_addr sin_addr; // a 32 bit IP addr in network byte order
//   unsigned char sin_zero[8];
// }
//
// where 
// struct in_addr {
//   unsigned long s_addr;
// };
 


// getnameinfo(
//   const struct sockaddr* sa, socklen_t salen, 
//   char *host, size_t hostlen,
//   char *serv, size_t servlen, int flags);
//
// This function is the opposite of getaddrinfo(), that is this function takes an 
// already loaded struct sockaddr and does a name and service name lookup on it. 
// It replaces the old gethostbyaddr and getservbyport functions
//
//


void test_unspec_tcp(char* nodename, char* servname)
{
    struct addrinfo hints, *servinfo, *p;
    int rv;

    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    if ((rv = getaddrinfo(nodename, servname, &hints, &servinfo)) != 0) {
        fprintf(stderr, "getaddrinfo:%s\n", gai_strerror(rv));
        exit(1);
    }

    for (p = servinfo; p != NULL; p = p->ai_next) {
        printf("addrinfo ==================>\n");
        printf("socklen_t:%u\n", p->ai_addrlen);
        printf("ai_canonname:%s\n", p->ai_canonname);

        // unpack the message in sockaddr_in
        struct sockaddr_in *si = (struct sockaddr_in*)p->ai_addr;

        printf("ai_addr.sin_port:%u\n", si->sin_port);

        char host[1024];
        char serv[1024];

        rv = getnameinfo(
            p->ai_addr, p->ai_addrlen,
            host, sizeof host, serv, sizeof serv, 
            0);
        if (rv != 0) {
            fprintf(stderr, "getnameinfo:%s\n", gai_strerror(rv));
            continue;
        }

        printf("**host:%s\n", host);
        printf("**serv:%s\n", serv);
    }
}

int main(void)
{
    test_unspec_tcp("www.google.com", "http");
    test_unspec_tcp(NULL, "3490");
    return 0;
}
