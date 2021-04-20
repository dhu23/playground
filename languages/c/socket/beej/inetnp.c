#include <arpa/inet.h>

#include <stdio.h>

// const char *inet_ntop(
//   int af, const void *src,
//   char *dst, socklen_t size);
//
// int inet_pton(int af, const char *src, void *dst);
//
// n stands for network and p for presentation, or text presentation or printable
//
// inet_ntop takes the address family in the af parameter (either AF_INET or 
// AF_INET6). The src parameter should be a pointer to either a struct in_addr 
// or struct in6_addr containing the address you wish to convert to a string
//
// Finally dst and size are the pointer to the destination string and the 
// maximum length of that string
//
// Max lengths are INET_ADDRSTRLEN and INET6_ADDRSTRLEN
//
//
// You might have a string containing an IP address in readable form, and you 
// want to pack it into a struct sockaddr_in or a struct sockaddr_in6. In that 
// case, the opposite function inet_pton() is what you're after. It takes an
// address family (either AF_INET or AF_INET6) in the af parameter. The src
// points to a string containing the IP address in printable form. Lastly the 
// dst parameter points to where the result should be stored, which is probably 
// a struct in_addr or struct in6_addr


// IPv4 demo
void test_conversion4(char *ip_addr)
{
    struct sockaddr_in sa;
    char str[INET_ADDRSTRLEN];

    // store IP address in sa
    inet_pton(AF_INET, ip_addr, &(sa.sin_addr)); // of in_addr type

    // now get ti back and print it
    inet_ntop(AF_INET, &(sa.sin_addr), str, INET_ADDRSTRLEN);

    printf("input:%s, output:%s\n", ip_addr, str);
}

// IPv6 demo
void test_conversion6(char *ip_addr)
{
    struct sockaddr_in6 sa;
    char str[INET6_ADDRSTRLEN];

    // store IP in sa
    inet_pton(AF_INET6, ip_addr, &(sa.sin6_addr)); // of in6_addr type

    // now get back 
    inet_ntop(AF_INET6, &(sa.sin6_addr), str, INET6_ADDRSTRLEN); 

    printf("input:%s, output:%s\n", ip_addr, str);
}


int main(void)
{
    test_conversion4("10.0.0.24");
    test_conversion6("fe80::272d:2081:4e1a:dd2e");
    return 0;
}
