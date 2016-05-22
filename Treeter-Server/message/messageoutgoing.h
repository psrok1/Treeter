#ifndef MESSAGEOUTGOING_H
#define MESSAGEOUTGOING_H

#include "messagebase.h"
#include <string>
#include <memory>
#include "../connection.h"

class MessageOutgoing : public MessageBase
{
    Connection::Reference connection;

    virtual std::string toString() = 0;
    virtual void send();
public:
    typedef std::shared_ptr<MessageOutgoing> Reference;

    virtual void setConnection(Connection::Reference connection);

    virtual Reference clone() = 0;
};

/** EchoResponse **/

class EchoResponse : public MessageOutgoing
{
    std::string content;

    virtual std::string toString();
public:
    EchoResponse(std::string message): content(message) { }
    virtual MessageOutgoing::Reference clone();
};

/** HelloResponse **/

class HelloResponse: public MessageOutgoing
{
    std::string publicKey;
    virtual std::string toString();
public:
    HelloResponse(std::string pubKey): publicKey(pubKey) { }
    virtual MessageOutgoing::Reference clone();
};

/** StartEncryptionResponse **/

class StartEncryptionResponse: public MessageOutgoing
{
    virtual std::string toString();
public:
    virtual MessageOutgoing::Reference clone();
};

#endif // MESSAGEOUTGOING_H
