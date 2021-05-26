#ifndef _INCLUDED_TCPSERVER_H_
#define _INCLUDED_TCPSERVER_H_

#include <sys/select.h>
#include "packing.h"
#include "buffer.h"

class TCPServer
{
public:
    virtual void run() = 0;

protected:
    Packet<1024, LinearBufferIdx> buf_;
};

struct addrinfo;

class TCPSelectServer : public TCPServer
{
private:
    struct addrinfo* ai_;
    fd_set master_;
    int listener_;
    int fdmax_;

    void closeFd(int i);

public:
    TCPSelectServer(const char* port);
    virtual ~TCPSelectServer();
    virtual void run() override;
};

#endif
