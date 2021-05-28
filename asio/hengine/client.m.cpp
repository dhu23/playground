#include "tcpclient.h"
#include <iostream>


struct MAbsorber
{
    template<std::size_t K, typename BufIdx, typename T>
    void onMessage(const T& msg, Packet<K, BufIdx>& out)
    {
        std::cout << "absorbed:" << msg << std::endl;
    }
};

int main(int argc, char* argv[])
{
    const char* PORT = "12345";
    TCPClient c("localhost", PORT);

    MAbsorber ma;   
    c.run(ma);
    return 0;
}
