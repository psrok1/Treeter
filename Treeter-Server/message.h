#ifndef MESSAGE_H
#define MESSAGE_H

#include <string>
#include <memory>
#include <iostream>
#include "connection.h"

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

class Message: public MessageBase {
    Connection::Reference connection;

    virtual std::string toString() = 0;
    virtual void send();
public:
    Message(Connection::Reference conn): connection(conn) { }
};

class TestMessage: public Message {

    std::string Msgbuff;

    virtual std::string toString()
    {
        return Msgbuff;
    }
public:
    TestMessage(Connection::Reference conn, std::string msg): Message(conn), Msgbuff(msg) { }
};


#endif // MESSAGE_H
