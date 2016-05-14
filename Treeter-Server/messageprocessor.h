#ifndef MESSAGEPROCESSOR_H
#define MESSAGEPROCESSOR_H

#include "connection.h"
#include "messagesender.h"

class EchoRequest;
class HelloRequest;
class StartEncryptionRequest;

class MessageProcessor {
    Connection::Reference connection;
    MessageSender::Reference sender;

public:
    MessageProcessor(Connection::Reference conn);
    // Copying is not allowed
    MessageProcessor(const MessageProcessor&) = delete;
    MessageProcessor& operator=(const MessageProcessor&) = delete;

    bool processRequest(const EchoRequest&);
    bool processRequest(const HelloRequest &);
    bool processRequest(const StartEncryptionRequest &req);
};

#endif // MESSAGEPROCESSOR_H
