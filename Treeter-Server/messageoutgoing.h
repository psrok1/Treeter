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
    std::string content;

    virtual std::string toString();
public:
    EchoResponse(Connection::Reference conn, std::string message):
        MessageOutgoing(conn), content(message) { }
};

class HelloResponse: public MessageOutgoing
{
public:
    HelloResponse(Connection::Reference conn, std::string pubKey): MessageOutgoing(conn), publicKey(pubKey) { }
    virtual std::string toString();

private:
    std::string publicKey;
};

class StartEncryptionResponse: public MessageOutgoing
{
public:
    StartEncryptionResponse(Connection::Reference conn): MessageOutgoing(conn) { }
    virtual std::string toString();
};

#endif // MESSAGEOUTGOING_H
