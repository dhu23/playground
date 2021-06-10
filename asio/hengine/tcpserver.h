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
                    auto tailBuff = inbuf_.tailBuffer();
                    int rc = receive(tailBuff.data, tailBuff.size, i);
                    if (rc < 0)
                    {
                        this->closeFd(i);
                    }
                    else // rc >= 0 no room or something was read out
                    {
                        if (rc > 0) { inbuf_.write(rc); }
                        procM(inbuf_, outbuf_, mp);
                    }

                    // char buf[256];
                    // int nbytes = recv(i, buf, sizeof buf, 0);
                    // if (nbytes <= 0)
                    // {
                    //     if (nbytes == 0) 
                    //     {
                    //         std::cout << "socket " << i << " hung up" << std::endl;
                    //     }
                    //     else
                    //     {
                    //         std::cerr << "recv error" << std::endl;
                    //     }
                    //     this->closeFd(i);
                    // }
                    // else
                    // {
                    //     std::cout << "recv nbytes:" << nbytes << std::endl;
                    //     // lets optimize this one away maybe?
                    //     std::string data(buf, buf+(std::min(256, nbytes)));
                    //     std::cout << "recv:" << data << std::endl;

                    //     // TODO there is some optimization, and maybe redesign that
                    //     // I can go without this unnecessary copying of data
                    //     if (inbuf_.put(buf, nbytes))
                    //     {
                    //         std::cout 
                    //             << "wrote " << nbytes << " bytes into buffer. left:" 
                    //             << inbuf_.writableSize() << std::endl;
                    //         procM(inbuf_, outbuf_, mp);
                    //     }
                    //     else
                    //     {
                    //         std::cerr << "failed to write to buffer" << std::endl;
                    //     }
                    // }
                }
            }
        }
    }
};

#endif
