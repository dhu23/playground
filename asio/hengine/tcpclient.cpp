#include "tcpclient.h"
#include "socketutil.h"

#include <unistd.h>

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
