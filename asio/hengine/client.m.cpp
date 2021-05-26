#include "tcpclient.h"
#include <iostream>

int main(int argc, char* argv[])
{
    const char* PORT = "12345";
    TCPClient c("localhost", PORT);
    c.run();
    return 0;
}
