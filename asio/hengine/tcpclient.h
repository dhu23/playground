#ifndef _INCLUDED_TCPCLIENT_H_
#define _INCLUDED_TCPCLIENT_H_

#include "packing.h"
#include "buffer.h"

class TCPClient
{
public:
    TCPClient(const char* host, const char* part);
    ~TCPClient();
    void run();

protected:
    struct addrinfo *servinfo_;
    Packet<1024, LinearBufferIdx> buf_;
    int serverfd_;
};

#endif
