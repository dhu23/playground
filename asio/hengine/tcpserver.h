#ifndef _INCLUDED_TCPSERVER_H_
#define _INCLUDED_TCPSERVER_H_

#include <sys/select.h>


class TCPServer
{
public:
    virtual void run() = 0;
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
