#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <memory>

class MessageBase
{
public:
    virtual bool isEOF() { return false; }
    virtual void send() = 0;
};

typedef std::shared_ptr<MessageBase> PMessageBase;

class EOFMessage: public MessageBase
{
public:
    virtual bool isEOF() { return true; }
    virtual void send() { }
};

class Message: public MessageBase {
    // <- connection
public:
    virtual std::string toString() = 0;
    Message();
};


#endif // MESSAGE_H
