#ifndef _INCLUDED_TCPCLIENT_H_
#define _INCLUDED_TCPCLIENT_H_

#include <sys/select.h>
#include <unistd.h>
#include <sys/socket.h>
#include <stdexcept>

#include "messages.h"
#include "socketutil.h"
#include "generalutil.h"

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
    QuoteUpdateGen qugen_;
public:
    TCPClient(const char* host, const char* part);
    ~TCPClient();

    template<typename MProc>
    void run(MProc& mp)
    {
        std::cout << "running client...\n" << std::endl;
        for (;;)
        {
            // always try to send everything out before any read
            putM(outbuf_, qugen_.gen());
            if (sendall(outbuf_, serverfd_) < 0)
            {
                std::cerr << "error: failed to send to server somehow" << std::endl;
            }

            fd_set readFds;
            FD_SET(serverfd_, &readFds);

            struct timeval tv;
            tv.tv_sec = 2;
            tv.tv_usec = 500000;
            if (select(serverfd_+1, &readFds, NULL, NULL, &tv) == -1)
            {
                throw std::runtime_error("select error");
            }

            for (int i = 0; i <= serverfd_; ++i)
            {
                if (FD_ISSET(i, &readFds)) 
                {
                    std::cout << StringUtils::concat(i, " set in select readFs\n");
                }
            }

            if (FD_ISSET(serverfd_, &readFds)) 
            { 
                std::cout << "running receive...\n";
                int rc = receive(inbuf_, serverfd_);
                if (rc < 0)
                {
                    std::cout << "closed serverfd" << std::endl;
                    close(serverfd_);
                }
                else // (rc >= 0) no room to read out or something was read out
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
            }
        } // END for infinite loop
    }

};

#endif
