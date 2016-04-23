#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <memory>

class Connection;
typedef std::shared_ptr<Connection> PConnection;

class MessageBase
{
    friend class MessageSender;
    virtual bool isEOF() { return false; }
    virtual void send() = 0;
};

typedef std::shared_ptr<MessageBase> PMessageBase;

class EOFMessage: public MessageBase
{
    virtual bool isEOF() { return true; }
    virtual void send() { }
};

class Message: public MessageBase {
    PConnection connection;

    virtual std::string toString() = 0;
    virtual void send();
public:
    Message(PConnection conn): connection(conn) { }
};


#endif // MESSAGE_H
