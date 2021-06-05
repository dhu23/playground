#ifndef _INCLUDED_TCPCLIENT_H_
#define _INCLUDED_TCPCLIENT_H_

#include <sys/select.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdexcept>

#include "messages.h"

template<typename T>
class Oscillator
{
    T min_;
    T step_;
    std::size_t range_;

    bool direction_;
    std::size_t pos_;

public:
    Oscillator(T tmin, T tstep, std::size_t range, bool dir, std::size_t pos=0):
        min_(tmin),
        step_(tstep),
        range_(range),
        direction_(dir),
        pos_(pos)
    {}

    T gen()
    {
        auto ret = pos_*step_ + min_;

        if (direction_)
        {
                    
        }
        else
        {
        }
    }
};

class QuoteUpdateGen
{
    static constexpr uint32_t qmax_ = 100;
    static constexpr uint32_t qmin_ = 1;

    static constexpr uint64_t pmax_ = 600LL;
    static constexpr uint64_t pmin_ = 100LL;

    uint32_t q_ = qmax_;
    uint32_t qstep_ = 1;
    bool qdirect_ = false;

    uint64_t p_ = pmin_;
    uint64_t pstep_ = 50LL;
    bool pdirect_ = true;

public:
    QuoteUpdate gen()
    {
        if (qdirect_)
        {

        }
    }
};

class TCPClient
{
protected:
    struct addrinfo *servinfo_;
    Packet<1024, RingBufferIdx> inbuf_;
    Packet<1024, RingBufferIdx> outbuf_;
    int serverfd_;
public:
    TCPClient(const char* host, const char* part);
    ~TCPClient();

    template<typename MProc>
    void run(MProc& mp)
    {
        for (;;)
        {
            fd_set readFds;
            FD_SET(serverfd_, &readFds);
            if (select(serverfd_+1, &readFds, NULL, NULL, NULL) == -1)
            {
                throw std::runtime_error("select error");
            }
            
            if (!FD_ISSET(serverfd_, &readFds)) { continue; }
            char buf[256];
            int nbytes = recv(serverfd_, buf, sizeof buf, 0);
            if (nbytes <= 0)
            {
                if (nbytes == 0)
                {
                    std::cout << "socket " << serverfd_ << " hung up" << std::endl;
                }
                else
                {
                    std::cerr << "recv error" << std::endl;
                }
                close(serverfd_);
                break;
            }
            else
            {
                std::cout << "recv nbytes:" << nbytes << std::endl;
                std::string data(buf, buf+(std::min(256, nbytes)));
                std::cout << "recv:" << data << std::endl;

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
            putM(outbuf_, QuoteUpdate
        }
    }

};

#endif
