#ifndef MESSAGEOUTGOING_H
#define MESSAGEOUTGOING_H

#include "messagebase.h"
#include <string>
#include "connection.h"

class MessageOutgoing : public MessageBase
{
    Connection::Reference connection;

    virtual std::string toString() = 0;
    virtual void send();
public:
    MessageOutgoing(Connection::Reference conn): connection(conn) { }
};

class EchoResponse : public MessageOutgoing
{
    unsigned int id;
    std::string content;

    virtual std::string toString();
public:
    EchoResponse(Connection::Reference conn, unsigned int id, std::string message):
        MessageOutgoing(conn), id(id), content(message) { }
};

#endif // MESSAGEOUTGOING_H
