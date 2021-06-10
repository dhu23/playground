#ifndef _INCLUDED_TCPSERVER_H_
#define _INCLUDED_TCPSERVER_H_

#include "messages.h"
#include "socketutil.h"

#include <sys/select.h>
#include <unistd.h>
#include <netinet/in.h>
#include <iostream>


class TCPSelectServer
{
private:
    struct addrinfo* ai_;
    fd_set master_;
    int listener_;
    int fdmax_;
    Packet<1024, RingBufferIdx> inbuf_;
    Packet<1024, RingBufferIdx> outbuf_;

    void closeFd(int i);

public:
    TCPSelectServer(const char* port);
    ~TCPSelectServer();

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
                    int rc = receive(inbuf_, i);
                    if (rc < 0)
                    {
                        std::cout << "closed fd:" << i << std::endl;
                        this->closeFd(i);
                    }
                    else // rc >= 0 no room or something was read out
                    {
                        std::cout 
                            << StringUtils::concat(
                                "processing inbuf. read size:", 
                                inbuf_.readableSize()) 
                            << std::endl;
                        if (procM(inbuf_, outbuf_, mp) == ProcMRes::Error)
                        {
                            std::cerr << "ran into procM error" << std::endl;
                        }
                    }

                    // send everything in outbuf_
                    if (sendall(outbuf_, i) < 0)
                    {
                        std::cerr << "error: failed to send somehow" << std::endl;

                    }

                }
            }
        }
    }
};

#endif
