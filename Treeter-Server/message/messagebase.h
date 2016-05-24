#ifndef MESSAGE_H
#define MESSAGE_H

#include <memory>
#include <string>

class MessageBase
{
    friend class MessageSender;
    virtual bool isEOF() { return false; }
public:
    typedef std::shared_ptr<MessageBase> Reference;

    virtual std::string serialize() { return std::string(); }
};

class EOFMessage: public MessageBase
{
    virtual bool isEOF() { return true; }
};

#endif // MESSAGE_H
