#include "tcpclient.h"

#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netdb.h>

namespace  // duplicate in tcpserver.cpp
{

std::string getRemoteIP(struct sockaddr_storage& sa)
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

} // anonymous namespace

TCPClient::TCPClient(const char* host, const char* port)
{
    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;

    int rv = getaddrinfo(host, port, &hints, &servinfo_);
    if (rv != 0)
    {
        throw std::runtime_error(
            "address error " + std::string(gai_strerror(rv))
            );
    }

    struct addrinfo* p = NULL;
    for (p = servinfo_; p != NULL; p = p->ai_next)
    {
        serverfd_ = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (serverfd_ < 0) { continue; }

        rv = connect(serverfd_, p->ai_addr, p->ai_addrlen);
        if (rv < 0)
        {
            std::cerr << "cannot connect to server" << std::endl;
            close(serverfd_);
            continue;
        }
        break;
    }

    if (p == NULL)
    {
        throw std::runtime_error("cannot connect to server");
    }
    struct sockaddr_storage* serveraddr = (struct sockaddr_storage*)p->ai_addr;
    std::cout << "client connected to:" << getRemoteIP(*serveraddr) << std::endl;
}

TCPClient::~TCPClient()
{
    freeaddrinfo(servinfo_);
}

void TCPClient::run()
{
    std::cout << "running..." << std::endl;
}
