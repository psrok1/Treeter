#ifndef MESSAGE_H
#define MESSAGE_H

#include <memory>

class MessageBase
{
    friend class MessageSender;
    virtual bool isEOF() { return false; }
    virtual void send() = 0;
public:
    typedef std::shared_ptr<MessageBase> Reference;
};

class EOFMessage: public MessageBase
{
    virtual bool isEOF() { return true; }
    virtual void send() { }
};

#endif // MESSAGE_H
