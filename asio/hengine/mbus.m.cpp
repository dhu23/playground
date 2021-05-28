#include "tcpserver.h"
#include "messages.h"
#include <iostream>


struct MAbsorber
{
    template<std::size_t K, typename BufIdx>
    void onMessage(const ShutDown& msg, Packet<K, BufIdx>& out)
    {
        std::cout << "absorbed:" << msg << std::endl;
    }

    template<std::size_t K, typename BufIdx>
    void onMessage(const LogOn& msg, Packet<K, BufIdx>& out)
    {
        std::cout << "absorbed:" << msg << std::endl;
    }

    template<std::size_t K, typename BufIdx>
    void onMessage(const LogOut& msg, Packet<K, BufIdx>& out)
    {
        std::cout << "received:" << msg << std::endl;
    }

    template<std::size_t K, typename BufIdx>
    void onMessage(const QuoteUpdate& msg, Packet<K, BufIdx>& out)
    {
    }

    template<std::size_t K, typename BufIdx>
    void onMessage(const Acknowledgement& msg, Packet<K, BufIdx>& out)
    {
    }

    template<std::size_t K, typename BufIdx>
    void onMessage(const HeartBeat& msg, Packet<K, BufIdx>& out)
    {
    }
};


int main(int argc, char* argv[])
{
    const char* PORT = "12345";
    TCPSelectServer s(PORT);

    MAbsorber ma;
    s.run(ma);
    return 0;
}
