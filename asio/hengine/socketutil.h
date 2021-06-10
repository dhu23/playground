#ifndef _INCLUDED_SOCKETUTIL_H_
#define _INCLUDED_SOCKETUTIL_H_

#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "generalutil.h"

inline std::string getRemoteIP(struct sockaddr_storage& sa)
{
    char remoteIP[INET6_ADDRSTRLEN];

    struct sockaddr* psa = reinterpret_cast<struct sockaddr*>(&sa);
    inet_ntop(
        psa->sa_family, 

        psa->sa_family == AF_INET
        ? (void*)&(reinterpret_cast<struct sockaddr_in*>(psa)->sin_addr)
        : (void*)&(reinterpret_cast<struct sockaddr_in6*>(psa)->sin6_addr)
        ,

        remoteIP, INET6_ADDRSTRLEN);

    return std::string(remoteIP);
}

// use receive function only if fd is not a listener port
// pass along c recv function return value.
// fd needs to be ready to read, and the caller is responsible for this
// for caller: on negative return, please close fd
inline int receive(char* buff, std::size_t n, int fd)
{
    if (n == 0) { return 0; } // no room to read out anything
    auto nbytes = recv(fd, buff, n, 0);
    if (nbytes <= 0)
    {
        if (nbytes == 0)
        {
            std::cout 
                << StringUtils::concat("socket ", fd, " hung up")
                << std::endl;
        }
        else
        {
            std::cerr << StringUtils::concat("recv error") << std::endl;
        }
        return -1;
    }
    else
    {
        std::cout << StringUtils::concat("recv nbytes:", nbytes) << std::endl;
    }
    return nbytes;
}

inline int sendall(char* buff, std::size_t n, int fd)
{
    decltype(n) total = 0;
    decltype(n) left = n;
    
    int rc;

    while (total < n)
    {
        rc = send(fd, buff+total, left, 0);
        if (rc == -1) 
        { 
            std::cerr << "failed at sending" << std::endl; 
            break;
        }
        auto delta = static_cast<std::size_t>(rc);
        total += delta;
        left -= delta;
    }

    return rc == -1 ? -1 : 0; // -1 on failure
}

#endif
