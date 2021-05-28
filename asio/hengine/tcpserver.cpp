#include "tcpserver.h"

#include <stdexcept>
#include <string>


void TCPSelectServer::closeFd(int i)
{
    close(i);
    FD_CLR(i, &master_);
}

TCPSelectServer::TCPSelectServer(const char* port)
{
    FD_ZERO(&master_);

    struct addrinfo hints;
    memset(&hints, 0, sizeof hints);

    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_flags = AI_PASSIVE;

    int rv = getaddrinfo(NULL, port, &hints, &ai_);
    if (rv != 0)
    {
        throw std::runtime_error(
            "server cannot start" + std::string(gai_strerror(rv))
            );
    }

    struct addrinfo* p = NULL;
    for (p = ai_; p != NULL; p = p->ai_next) 
    {
        listener_ = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (listener_ < 0) { continue; }

        rv = bind(listener_, p->ai_addr, p->ai_addrlen) < 0;
        if (rv < 0)
        {
            std::cerr << "binding error:" << std::string(gai_strerror(rv)) << std::endl;
            close(listener_);
            continue;
        }
        break;
    }

    if (p == NULL)
    {
        throw std::runtime_error("server cannot bind");
    }


    if (listen(listener_, 10) == -1) 
    {
        throw std::runtime_error("server cannot listen");
    }
    FD_SET(listener_, &master_); 
    fdmax_ = listener_;
}

TCPSelectServer::~TCPSelectServer()
{
    freeaddrinfo(ai_);
}
