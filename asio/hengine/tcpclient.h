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
    Oscillator(T tmin, T tstep, std::size_t range, bool dir=true, std::size_t pos=0):
        min_(tmin),
        step_(tstep),
        range_(range),
        direction_(dir),
        pos_(pos)
    {}

    T gen()
    {
        auto ret = pos_*step_ + min_;
        if (pos_ == range_) 
        {
            direction_ = false;
            pos_ -= step_;
        }
        else if (pos_ == 0)
        {
            direction_ = true;
            pos_ = 1;
        }
        else
        {
            pos_ = direction_ ? pos_+step_ : pos_-step_;
        }
        return ret;
    }
};

class QuoteUpdateGen
{
    ByteArray<8> ticker_;
    Oscillator<uint32_t> qgen_;
    Oscillator<uint64_t> pgen_;

public:

    QuoteUpdateGen():
        ticker_(),
        qgen_(1, 1, 20),
        pgen_(100LL, 50LL, 30)
    {
        ticker_.fromArray("AAPL");
    }

    QuoteUpdate gen()
    {
        return QuoteUpdate
        {
            Timestamp::now(),
            qgen_.gen(),
            pgen_.gen(),
            ticker_
        };
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
