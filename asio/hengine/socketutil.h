#ifndef _INCLUDED_SOCKETUTIL_H_
#define _INCLUDED_SOCKETUTIL_H_

#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>

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

#endif
