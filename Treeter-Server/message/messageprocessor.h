#ifndef MESSAGEPROCESSOR_H
#define MESSAGEPROCESSOR_H

#include "../connection.h"
#include "messagesender.h"

class EchoRequest;
class HelloRequest;
class StartEncryptionRequest;
class AuthUserRequest;
class CreateAccountRequest;

class MessageProcessor {
    Connection::Reference connection;
public:
    MessageProcessor(Connection::Reference conn);
    // Copying is not allowed
    MessageProcessor(const MessageProcessor&) = delete;
    MessageProcessor& operator=(const MessageProcessor&) = delete;

    bool processRequest(const EchoRequest&);
    bool processRequest(const HelloRequest &);
    bool processRequest(const StartEncryptionRequest &req);
    bool processRequest(const AuthUserRequest &req);
    bool processRequest(const CreateAccountRequest &req);
};

#endif // MESSAGEPROCESSOR_H
