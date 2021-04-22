#include <sys/select.h>


class TCPServer
{
public:
    virtual void run() = 0;
};


struct addrinfo;

class TCPSelectServer
{
private:
    struct addrinfo* ai_;
    fd_set master_;
    int listener_;
    int fdmax_;

    void closeFd(int i);

public:
    TCPSelectServer(const char* port);
    virtual ~TCPServer();
    virtual void run() override;
};
