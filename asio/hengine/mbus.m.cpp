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

const char* PORT = "12345";

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

class Server
{
public:
    Server()
    {
        FD_ZERO(&master_);

        struct addrinfo hints;
        memset(&hints, 0, sizeof hints);

        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_flags = AI_PASSIVE;

        struct addrinfo* ai;
        int rv = getaddrinfo(NULL, PORT, &hints, &ai);
        if (rv != 0)
        {
            throw std::runtime_error(
                "server cannot start" + std::string(gai_strerror(rv))
                );
        }

        struct addrinfo* p = NULL;
        for (p = ai; p != NULL; p = p->ai_next) 
        {
            listener_ = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
            if (listener_ < 0) { continue; }

            if (bind(listener_, p->ai_addr, p->ai_addrlen) < 0) {
                close(listener_);
                continue;
            }
            break;
        }

        if (p == NULL)
        {
            throw std::runtime_error("server cannot bind");
        }

        freeaddrinfo(ai);

        if (listen(listener_, 10) == -1) 
        {
            throw std::runtime_error("server cannot listen");
        }
        std::cout << "listener fd " << listener_ << std::endl;
        FD_SET(listener_, &master_); 
        fdmax_ = listener_;
        std::cout << "listener fd again " << listener_ << std::endl;
    }

    void run()
    {
        for (;;) 
        {
            std::cout << "listener fd " << listener_ << std::endl;
            break;
            fd_set readFds;
            if (select(fdmax_+1, &readFds, NULL, NULL, NULL) == -1)
            {
                throw std::runtime_error("select error");
            }

            // run through all connections for data
            for (int i = 0; i <= fdmax_; ++i)
            {
                if (!FD_ISSET(i, &readFds)) 
                {
                    std::cout << "passing fd " << i << std::endl;
                    continue; 
                }
                std::cout << "processing fd " << i << " listener " << listener_ << std::endl;
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
                else
                {
                    // pass
                }
            }
        }
    }
// private:
    fd_set master_;
    int listener_;
    int fdmax_;
};

int main(int argc, char* argv[])
{
    Server s;
    std::cout << "main:listener fd " << s.listener_ << std::endl;
    std::cout << "main:max fd " << s.fdmax_ << std::endl;
    // s.run();
    return 0;
}
