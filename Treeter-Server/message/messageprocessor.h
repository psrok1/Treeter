#ifndef MESSAGEPROCESSOR_H
#define MESSAGEPROCESSOR_H

#include "../connection.h"
#include "messagesender.h"

class EchoRequest;
class HelloRequest;
class StartEncryptionRequest;
class AuthUserRequest;
class CreateAccountRequest;
class CreateSubgroupRequest;
class RemoveSubgroupRequest;
class GetSubgroupsRequest;
class AddUserToGroupRequest;
class RemoveUserFromGroupRequest;
class GetGroupUsersRequest;
class AddMeToGroupRequest;
class SetMemberPermissionRequest;

class SendMessageRequest;
class GetMessagesRequest;

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
    bool processRequest(const CreateSubgroupRequest &req);
    bool processRequest(const RemoveSubgroupRequest &req);
    bool processRequest(const GetSubgroupsRequest &req);
    bool processRequest(const AddUserToGroupRequest &req);
    bool processRequest(const RemoveUserFromGroupRequest &req);
    bool processRequest(const GetGroupUsersRequest &req);
    bool processRequest(const AddMeToGroupRequest &req);
    bool processRequest(const SendMessageRequest &req);
    bool processRequest(const GetMessagesRequest &req);
    bool processRequest(const SetMemberPermissionRequest &req);
};

#endif // MESSAGEPROCESSOR_H
