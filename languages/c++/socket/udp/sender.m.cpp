#include <future>
#include <cinttypes>
#include <cstdint>
#include <memory>
#include <csignal>
#include <iostream>
#include <cstdlib>
#include <thread>

namespace {

    constexpr uint32_t address = 0x7f000001; // 127.0.0.1

    constexpr uint16_t port = 8000;

    void onReceiveMessage(const std::string& message) {
    
        printf("Received message: %s\n", message.c_str());
    }

}


int main(int argc, char* argv[]) {
    DatagramSocket sender;
    if (!sender.bind()) {
        std::cout << "Cannot bind sender to port" << std::endl;
        return EXIT_FAILURE;
    }

    // start event loop to process the sending and receiving of datagrams
    sender.start(onReceiveMessage);

    printf("Now sending message to port %" PRIu16 "...\n", port);
    auto sent = std::make_shared<std::promise<void>>();
    sender.sendMessage(
        "Hello, World!",
        address, 
        port, 
        [sent]{sent->set_value();});

    // wait for the datagram to be sent, and then clean up and terminate
    sent->get_future().wait();
    printf("Program exiting.\n");
    return EXIT_SUCCESS;
}
