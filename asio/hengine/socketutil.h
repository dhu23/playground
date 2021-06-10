#ifndef _INCLUDED_SOCKETUTIL_H_
#define _INCLUDED_SOCKETUTIL_H_

#include <string>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <netdb.h>

#include "packing.h"
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
template<std::size_t K, typename BufIdx>
inline int receive(Packet<K, BufIdx>& packet, int fd)
{
    auto tailBuff = packet.tailBuffer();

    if (tailBuff.size == 0) { return 0; } // no room to read out anything
    auto nbytes = recv(fd, tailBuff.data, tailBuff.size, 0);
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
        packet.write(static_cast<std::size_t>(nbytes));
    }
    return nbytes;
}


template<std::size_t K, typename BufIdx>
inline int sendall(Packet<K, BufIdx>& packet, int fd)
{
    auto headBuff = packet.headBuffer();
    std::cout 
        << StringUtils::concat(
            "sendall: head buffer size:", headBuff.size)
        << std::endl;
    std::size_t total = 0;
    std::size_t left = headBuff.size;
    
    int rc;

    while (total < headBuff.size)
    {
        rc = send(fd, headBuff.data+total, left, 0);
        if (rc == -1) 
        { 
            std::cerr << "failed at sending" << std::endl; 
            break;
        }
        auto delta = static_cast<std::size_t>(rc);
        std::cout << StringUtils::concat("sent bytes:", rc) << std::endl;
        total += delta;
        left -= delta;
        packet.read(delta);
    }

    return rc == -1 ? -1 : 0; // -1 on failure
}

#endif
