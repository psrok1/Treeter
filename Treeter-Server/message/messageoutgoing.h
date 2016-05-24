#ifndef MESSAGEOUTGOING_H
#define MESSAGEOUTGOING_H

#include "messagebase.h"
#include <string>
#include <memory>
#include "../connection.h"

class MessageOutgoing : public MessageBase
{
    virtual std::string toString() = 0;
public:
    typedef std::shared_ptr<MessageOutgoing> Reference;

    // @TODO: Deprecated
    virtual Reference clone() = 0;
    // @TODO: Need to be refactored
    // serialize() method is only a bypass - toString must be public
    // Moreover, MessageBase hierarchy in this form, doesn't have sense with separate senders
    // @TODO: Missing const!
    virtual std::string serialize() { return this->toString(); }
    // @TODO: Delete line below and do refactoring in messageprocessor
    virtual void setConnection(Connection::Reference) { }
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
