#include <string>
#include <iostream>
#include <functional>
#include <queue>
#include <vector>
#include <variant>
#include <map>

struct Message
{
    enum class Type : char
    {
        Text,
        LogOn,
        LogOnResp,
        LogOff,
        LogOffResp
    };

    struct TextMsg { 
        std::string text; 
    };

    struct LogOnMsg { 
        std::string guestName; 
    };

    struct LogOnRespMsg { 
        std::string guestName; 
        std::string hostName; 
        bool accepted;
    };

    struct LogOffMsg { 
    };

    struct LogOffRespMsg {
    };

    using Body = std::variant
        < TextMsg
        , LogOnMsg
        , LogOnRespMsg
        , LogOffMsg
        , LogOffRespMsg
        >;

    Body body;
};

std::ostream& operator<<(std::ostream& os, Message::Type t)
{
    switch(t)
    {
    case Message::Type::Text:
        os << "MType[Text]";
        break;
    case Message::Type::LogOn:
        os << "MType[LogOn]";
        break;
    case Message::Type::LogOnResp:
        os << "MType[LogOnResp]";
        break;
    case Message::Type::LogOff:
        os << "MType[LogOff]";
        break;
    case Message::Type::LogOffResp:
        os << "MType[LogOffResp]";
        break;
    default:
        os << "MType[Unknown]";
        break;
    }
    return os;
}

std::ostream& operator<<(std::ostream& os, const Message::Body& b)
{
    // IOFunc is a ostream -> ostream function
    using IOFunc = std::function<std::ostream&(std::ostream&)>;
    struct BodyPrinter
    {
        IOFunc operator()(const Message::TextMsg& m) const
        {
            auto inner = [&m=m](std::ostream& os) -> std::ostream&
            {
                os << "TextMsg[text=" << m.text << "]";
                return os;
            };
            return inner;
        }
        IOFunc operator()(const Message::LogOnMsg& m) const
        {
            auto inner = [&m=m](std::ostream& os) -> std::ostream&
            {
                os << "LogOnMsg[guestName=" << m.guestName << "]";
                return os;
            };
            return inner;
        }
        IOFunc operator()(const Message::LogOnRespMsg& m) const
        {
            auto inner = [&m=m](std::ostream& os) -> std::ostream&
            {
                os 
                    << "LogOnRespMsg[guestName=" << m.guestName 
                    << ",hostName=" << m.hostName
                    << ",accepted=" << m.accepted
                    << "]";
                return os;
            };
            return inner;
        }
        IOFunc operator()(const Message::LogOffMsg& m) const
        {
            auto inner = [&m=m](std::ostream& os) -> std::ostream&
            {
                os << "LogOffMsg[]";
                return os;
            };
            return inner;
        }
        IOFunc operator()(const Message::LogOffRespMsg& m) const
        {
            auto inner = [&m=m](std::ostream& os) -> std::ostream&
            {
                os << "LogOffRespMsg[]";
                return os;
            };
            return inner;
        }
    };

    return std::visit(BodyPrinter{}, b)(os);
}

Message::Type messageType(const Message::Body& m)
{
    struct MsgTypeGetter
    {
        Message::Type operator()(const Message::TextMsg& m) const
        {
            return Message::Type::Text;
        }
        Message::Type operator()(const Message::LogOnMsg& m) const
        {
            return Message::Type::LogOn;
        }
        Message::Type operator()(const Message::LogOnRespMsg& m) const
        {
            return Message::Type::LogOnResp;
        }
        Message::Type operator()(const Message::LogOffMsg& m) const
        {
            return Message::Type::LogOff;
        }
        Message::Type operator()(const Message::LogOffRespMsg& m) const
        {
            return Message::Type::LogOffResp;
        }
    };

    return std::visit(MsgTypeGetter{}, m);
}

std::ostream& operator<<(std::ostream& os, const Message& m)
{
    os << m.body;
    return os;
}

using MessageActor = std::function<void(Message)>;

class PubSubHub
{
private:
    using Subscriptions = std::vector<MessageActor>;
    std::map<Message::Type, Subscriptions> subscribers_;
    std::queue<Message> messageQueue_;

public:
    void addSubscriber(Message::Type msgType, MessageActor msgActor)
    {
        subscribers_[msgType].push_back(msgActor);
    }

    void publish(const Message& msg)
    {
        messageQueue_.push(msg);
    }

    void distribute()
    {
        while (!messageQueue_.empty())
        {
            auto& msg = messageQueue_.front();
            auto found = subscribers_.find(messageType(msg.body));
            if (found != subscribers_.end())
            {
                for (auto& actor : found->second)
                {
                    actor(msg);
                }
            }
            else
            {
                std::cout << "No subscribers for " << msg << std::endl;
            }
            messageQueue_.pop();
        }
    }
};

class BusNode
{
protected:
    std::string name_;
    PubSubHub* pHub_;
public:
    BusNode(const std::string& n, PubSubHub* pHub): name_(n), pHub_(pHub) {}
    virtual ~BusNode() {}

    std::string name() const { return name_; }

    void publish(const Message& m)
    {
        pHub_->publish(m);
    }
};

class ServerA : public BusNode
{
private:
public:
    ServerA(const std::string& name, PubSubHub* pHub): BusNode(name, pHub)
    {
        pHub_->addSubscriber(
            Message::Type::LogOn, 
            [this](const Message& m) { this->publish(this->onLogOn(m)); }
        );
    }

    Message onLogOn(const Message& m)
    {
        std::cout 
            << "onLogOn() --> name[" << name_
            << "], receviedMessage=" << m
            << std::endl;
        auto p = std::get_if<Message::LogOnMsg>(&m.body);
        return p
            ? Message{Message::LogOnRespMsg{p->guestName, name_, true}}
            : Message{Message::LogOnRespMsg{"", name_, false}}
            ;
    }
};

class ClientA : public BusNode
{
private:
public:
    ClientA(const std::string& name, PubSubHub* pHub): BusNode(name, pHub)
    {
        pHub_->addSubscriber(
            Message::Type::LogOnResp,
            [this](const Message& m) { this->onLogOnResp(m); });
    }

    void onLogOnResp(const Message& m)
    {
        std::cout 
            << "onLogOnResp() --> name[" << name_
            << "], receiveMessage=" << m
            << std::endl;
    }
};


// Just a non-OOP snippet using "mutual recursion" to demonstrat the same idea
// clientA : send LogOn to ServerA
// ServerA : receive LogOn and send LogOnResp to clientA
// clientA : print LogOnResp out

void clientAProcessServerLogOnAck()
{
    std::cout << "clientA : receive LogOnResp from ServerA\n";
}

void serverAProcessClientALogOn()
{
    std::cout << "ServerA : receive LogOn and sendLogOnResp to clientA\n";
    clientAProcessServerLogOnAck();
}

void clientLogOn()
{
    std::cout << "clientA : send LogOn to ServerA\n";
    serverAProcessClientALogOn();
}


int main(int argc, char* argv[])
{
    std::vector<Message> ms
    {
        {Message::TextMsg{"Hello"}}
        ,{Message::LogOnMsg{"client1"}}
        ,{Message::LogOnRespMsg{"client1", "server1", true}}
        ,{Message::LogOffMsg{}}
        ,{Message::LogOffRespMsg{}}
    };
    for (auto& m : ms)
    {
        std::cout << m << std::endl;
    }

    PubSubHub hub;
    ClientA c("clientA1", &hub);
    ServerA s("serverA1", &hub);

    c.publish(Message{Message::LogOnMsg{"clientA1"}});
    hub.distribute();

    // non-OOP Proof of concept
    clientLogOn();

    return 0;
}
