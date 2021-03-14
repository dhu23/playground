// explain how you would design a chat server. In particular provide details
// about the various backend components, classes and methods. What would be 
// the hardest problems to solve?

#include <string>

using namespace std;

struct UserId;
// besides UserId, User contains the way to connect to the client program
struct User;
// participants, conversation input history and time stamps

struct ChatRoomId;
struct ChatRoom;

// when users type in something and hit send, it sends a message to server
// message:
// user|timestamp|chatroomId|content
struct Message;

// whenever the chat room receives a new input, it broadcast the context to 
// all participants

// invite participant(s) to chat
// remove participant(s)
// leave chat

struct ChatServer;

struct Message
{
    Message(const string& text, const User& sender);
};

// maintain chat room information, participants, chat message
struct ChatRoom
{
    ChatRoom(const vector<User*>& users);
    
    // update history
    void receive(const Message& incomingMessage); 

    // send the message to every other participants in the chat
    void broadcast(const Message& outgoingMessage);
    void remove(const User* user);
    void add(const User* user);

    vector<Message> history;
    vector<User*> participants;
};


// backend comprises of distributed chatsvc as the front door in front
// of ChatServer class
struct ChatServer
{
    void receiveMessage(const Message& msg, ChatRoom& chat);

    // this would create a chat room for the two users, if not exists yet
    void receiveMessage(const Message& msg, const User& receipant);
    void sendMessage(const Message& msg, const User& receipant);

    bool isOnline(const User& user) const;
    bool signIn(const User& user);

    ChatRoom& getChatRoom(const vector<User>& users);

    vector<User> users;
    vector<ChatRoom> chatRooms;
};

