#ifndef MESSAGEPROCESSOR_H
#define MESSAGEPROCESSOR_H

#include "connection.h"
#include "messagesender.h"

class EchoRequest;

class MessageProcessor {
    Connection::Reference connection;
    MessageSender::Reference sender;
public:
    MessageProcessor(Connection::Reference conn);
    // Copying is not allowed
    MessageProcessor(const MessageProcessor&) = delete;
    MessageProcessor& operator=(const MessageProcessor&) = delete;

    bool processRequest(const EchoRequest&);
};

#endif // MESSAGEPROCESSOR_H
