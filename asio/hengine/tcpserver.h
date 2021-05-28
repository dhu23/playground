#ifndef _INCLUDED_TCPSERVER_H_
#define _INCLUDED_TCPSERVER_H_

#include <sys/select.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>

#include "messages.h"

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

struct addrinfo;

class TCPSelectServer
{
private:
    struct addrinfo* ai_;
    fd_set master_;
    int listener_;
    int fdmax_;
    Packet<1024, LinearBufferIdx> inbuf_;
    Packet<1024, LinearBufferIdx> outbuf_;

    void closeFd(int i);

public:
    TCPSelectServer(const char* port);
    virtual ~TCPSelectServer();

    template<typename MProc>
    void run(MProc& mp)
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
                        std::cout << "recv nbytes:" << nbytes << std::endl;
                        // lets optimize this one away maybe?
                        std::string data(buf, buf+(std::min(256, nbytes)));
                        std::cout << "recv:" << data << std::endl;

                        // TODO there is some optimization, and maybe redesign that
                        // I can go without this unnecessary copying of data
                        if (inbuf_.put(buf, nbytes))
                        {
                            std::cout 
                                << "wrote " << nbytes << " bytes into buffer. left:" 
                                << inbuf_.writableSize() << std::endl;
                            procM(inbuf_, outbuf_, mp);
                        }
                        else
                        {
                            std::cerr << "failed to write to buffer" << std::endl;
                        }
                    }
                }
            }
        }
    }
};

#endif
