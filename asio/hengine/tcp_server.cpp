#include "tcp_server.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

#include <stdexcept>
#include <string>
#include <iostream>


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

void TCPSelectServer::run()
{
    for (;;) 
    {
        fd_set readFds = master_;
        if (select(fdmax_+1, &readFds, NULL, NULL, NULL) == -1)
        {
            throw std::runtime_error("select error");
        }

        // run through all connections for data
        for (int i = 0; i <= fdmax_; ++i)
        {
            if (!FD_ISSET(i, &readFds)) { continue; }
            if (i == listener_)
            {
                std::cout << "new connection..." << std::endl;
                printf("new connection\n");
                struct sockaddr_storage remoteaddr;
                socklen_t addrlen = sizeof remoteaddr;
                int newfd = accept(
                    listener_, 
                    reinterpret_cast<struct sockaddr*>(&remoteaddr),
                    &addrlen);

                if (newfd == -1)
                {
                    std::cerr << "accept error" << std::endl;
                }
                else
                {
                    FD_SET(newfd, &master_); // add to master set
                    fdmax_ = std::max(fdmax_, newfd);

                    std::cout 
                        << "new connection from :" 
                        << getRemoteIP(remoteaddr)
                        << " on socket " << newfd
                        << std::endl;
                }
            }
            else // received data from a client
            {
                char buf[256];
                int nbytes = recv(i, buf, sizeof buf, 0);
                if (nbytes <= 0)
                {
                    if (nbytes == 0) 
                    {
                        std::cout << "socket " << i << " hung up" << std::endl;
                    }
                    else
                    {
                        std::cerr << "recv error" << std::endl;
                    }
                    this->closeFd(i);
                }
                else
                {
                    printf("recv nbytes:%d\n", nbytes);
                    std::string data(buf, buf+(std::min(256, nbytes)));
                    std::cout << "recv:" << data << std::endl;
                }
            }
        }
    }
}

