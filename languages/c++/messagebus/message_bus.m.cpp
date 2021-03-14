// this is basically the example code I found on the internet
// except I change a few variable names

#include <string>
#include <iostream>
#include <functional>
#include <queue>
#include <vector>

class Message
{
public:
    Message(const std::string& event):event_(event) {}
    std::string event() const { return event_; }

private:
    std::string event_;
};

class MessageBus
{
public:
    void addReceiver(std::function<void(Message)> messageActor)
    {
        subscribers_.push_back(messageActor);
    }

    void sendMessage(Message message)
    {
        messageQueue_.push(message);
    }
    
    void notify()
    {
        while (!messageQueue_.empty())
        {
            for (auto& actor : subscribers_)
            {
                actor(messageQueue_.front());
            }
            messageQueue_.pop();
        }
    }

private:
    std::vector<std::function<void(Message)>> subscribers_;
    std::queue<Message> messageQueue_;
};

class BusNode
{
public:
    BusNode(MessageBus* pMessageBus): pMessageBus_(pMessageBus)
    {
        pMessageBus_->addReceiver(this->messageActor());
    }

    void send(Message message)
    {
        pMessageBus_->sendMessage(message);
    }

    virtual void update() {}

private:

    MessageBus* pMessageBus_;

    std::function<void(Message)> messageActor()
    {
        auto inner = [=](Message message) -> void
        {
            this->onNotify(message);
        };
        return inner;
    }

    virtual void onNotify(Message message)
    {
        std::cout << "Hello, in onNotify!" << std::endl;
    }
};

class ComponentA : public BusNode
{
public:
    ComponentA(MessageBus* pMessageBus): BusNode(pMessageBus) {}
    
private:
    void onNotify(Message message)
    {
        std::cout << "CompA received: " << message.event() << std::endl;
    }
};

class ComponentB : public BusNode
{
public:
    ComponentB(MessageBus* pMessageBus): BusNode(pMessageBus) {}

    void update()
    {
        send(Message("B Logon"));
    }

private:
    void onNotify(Message message)
    {
        std::cout << "CompB received: " << message.event() << std::endl;
    }
};

int main(int argc, char* argv[])
{
    MessageBus mb;

    ComponentA a(&mb);
    ComponentB b(&mb);

    a.update(); // nothing happens
    b.update(); // send a Logon message to the bus
    mb.notify();
    return 0;
}
