htons() => host to network short

htonl() => host to network long

ntohs() => network to host short

ntohl() => network to host long


```
struct addinfo {
    int ai_flags; // AI_PASSIVE, AI_CANONNAME, etc.
    int ai_family; // AF_INET, AF_INET6, AF_UNSPEC
    int ai_socktype; // SOCK_STREAM, SOCK_DGRAM
    int ai_protocol; // use 0 for "any"
    size_t ai_addrlen; // size of ai_addr in bytes
    struct sockaddr* ai_addr; // struct sockaddr_in or _in6
    char* ai_canonname; // full canonical hostname

    struct addrinfo* ai_next; // linked list, next node
}
```

You load this struct up a bit, and then call `getaddrinfo()`. 

```
struct sockaddr {
    unsigned short sa_family; // address family, AF_xxx
    char sa_data[14]; // 14 bytes of protocol address
}
```

```
struct sockaddr_in {
    short int sin_family; // Address family, AF_INET
    unsigned short int sin_port; // Port number
    struct in_addr sin_addr; // Internet address
    unsigned char sin_zero[8]; // Same size as struct sockaddr
}
```

more structures
```
// (IPv4 only -- see struct in6_addr for IPv6)

// Internet address (a structure for historical reasons)
struct in_addr {
    uint32_t s_addr; // that's a  32-bit int (4 bytes)
}

// (IPv6 only -- see struct sockaddr_in and struct in_addr for IPv4)

struct sockaddr_in6 {
    uint16_t sin6_family; // address famil, AF_INET6
    uint16_t sin6_port; // port number, Network Byte Order
    uint32_t sin6_flowinfo; // IPv6 flow information
    struct in6_addr sin6_addr; // IPv6 address
    uint32_t sin6_scope_id; // Scope ID
}

struct in6_addr {
    unsigned char s6_addr[16]; // IPv6 address
}

```

`poll()` usage. The signature is:
```
#include <poll.h>
int poll(struct pollfd fds[], nfds_t nfds, int timeout);
```
timeout is a timeout in milliseconds. It returns of elements in the array
that have had an event occur. You can specify a negative timeout to wait 
forever.

```
// let's have a look at that struct:
struct pollfd {
    int fd; // the socket descriptor
    short events; //bitmap of events we're interested in
    short revents; // when poll() returns, bitmap of events that occurred
};
```

The `events` field is the bitwise-OR of the following:

POLLIN - Alert me when data is ready to recv() on this socket
POLLOUT - Alert me when I can send() data to this socket without blocking

On


