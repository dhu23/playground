#include "tcp_server.h"
#include <iostream>


int main(int argc, char* argv[])
{
    const char* PORT = "12345";
    TCPSelectServer s(PORT);
    s.run();
    return 0;
}
