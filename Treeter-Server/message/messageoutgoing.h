#ifndef MESSAGEOUTGOING_H
#define MESSAGEOUTGOING_H

#include <string>
#include <vector>
#include <memory>
#include <model/groupmessage.h>

enum class ResponseErrorCode { OK };

class MessageOutgoing
{
public:
    typedef std::shared_ptr<MessageOutgoing> Reference;
    virtual std::string toString() = 0;
    virtual bool isEOF() { return false; }
};

/** EOFMessage **/

class EOFMessage : public MessageOutgoing
{
public:
    virtual bool isEOF() { return true; }
    virtual std::string toString() { return std::string(); }
};

/** EchoResponse **/

class EchoResponse : public MessageOutgoing
{
    std::string content;
public:
    EchoResponse(std::string message): content(message) { }
    virtual std::string toString();
};

/** HelloResponse **/

class HelloResponse: public MessageOutgoing
{
    std::string publicKey;
public:
    HelloResponse(std::string pubKey): publicKey(pubKey) { }
    virtual std::string toString();
};

/** StartEncryptionResponse **/

class StartEncryptionResponse: public MessageOutgoing
{
public:
    virtual std::string toString();
};

/** AuthUserResponse **/

class AuthUserResponse: public MessageOutgoing
{
    ResponseErrorCode error;
public:
    AuthUserResponse(ResponseErrorCode error = ResponseErrorCode::OK): error(error) { }
    virtual std::string toString();
};

/** CreateAccountResponse **/

class CreateAccountResponse: public MessageOutgoing
{
    ResponseErrorCode error;
public:
    CreateAccountResponse(ResponseErrorCode error = ResponseErrorCode::OK): error(error) { }
    virtual std::string toString();
};

/** CreateGroupResponse **/

class CreateGroupResponse: public MessageOutgoing
{
    ResponseErrorCode error;
public:
    CreateGroupResponse(ResponseErrorCode error = ResponseErrorCode::OK): error(error) { }
    virtual std::string toString();
};

/** RemoveGroupResponse **/

class RemoveGroupResponse: public MessageOutgoing
{
    ResponseErrorCode error;
public:
    RemoveGroupResponse(ResponseErrorCode error = ResponseErrorCode::OK): error(error) { }
    virtual std::string toString();
};

/** GetSubgroupsResponse **/

class GetSubgroupsResponse: public MessageOutgoing
{
    std::vector<std::string> subgroups;
    ResponseErrorCode error;
public:
    GetSubgroupsResponse(std::vector<std::string> subgroups, ResponseErrorCode error = ResponseErrorCode::OK): subgroups(subgroups), error(error) { }
    virtual std::string toString();
};

/** AddUserToGroupResponse **/

class AddUserToGroupResponse: public MessageOutgoing
{
    ResponseErrorCode error;
public:
    AddUserToGroupResponse(ResponseErrorCode error = ResponseErrorCode::OK): error(error) { }
    virtual std::string toString();
};


/** RemoveUserFromGroupResponse **/

class RemoveUserFromGroupResponse: public MessageOutgoing
{
    ResponseErrorCode error;
public:
    RemoveUserFromGroupResponse(ResponseErrorCode error = ResponseErrorCode::OK) : error(error) { }
    virtual std::string toString();
};

/** GetGroupPathsResponse **/

class GetGroupPathsResponse: public MessageOutgoing
{
    std::vector<std::string> groupPaths;
    ResponseErrorCode error;
public:
    GetGroupPathsResponse(std::vector<std::string> groupPaths, ResponseErrorCode error = ResponseErrorCode::OK) : groupPaths(groupPaths), error(error) { }
    virtual std::string toString();
};


/** GetGroupUsersResponse **/

class GetGroupUsersResponse: public MessageOutgoing
{
    std::vector<std::string> moderators, users;
    ResponseErrorCode error;
public:
    GetGroupUsersResponse(std::vector<std::string> moderators, std::vector<std::string> users, ResponseErrorCode error = ResponseErrorCode::OK) : moderators(moderators), users(users), error(error) { }
    virtual std::string toString();
};

/** AddMeToGroupResponse **/

class AddMeToGroupResponse: public MessageOutgoing
{
    ResponseErrorCode error;
public:
    AddMeToGroupResponse(ResponseErrorCode error = ResponseErrorCode::OK) : error(error) { }
    virtual std::string toString();
};


/** GetGroupPendingUsersResponse **/

class GetGroupPendingUsersResponse: public MessageOutgoing
{
    std::vector<std::string> users;
    ResponseErrorCode error;
public:
    GetGroupPendingUsersResponse(std::vector<std::string> users, ResponseErrorCode error = ResponseErrorCode::OK) : users(users), error(error) { }
    virtual std::string toString();
};

/** SendMessageResponse **/

class SendMessageResponse: public MessageOutgoing
{
    ResponseErrorCode error;
public:
    SendMessageResponse(ResponseErrorCode error = ResponseErrorCode::OK) : error(error) { }
    virtual std::string toString();
};

/** GetMessagesResponse **/

class GetMessagesResponse: public MessageOutgoing
{
    std::vector<Model::GroupMessage> messages;
    ResponseErrorCode error;
public:
    GetMessagesResponse(std::vector<Model::GroupMessage> messages, ResponseErrorCode error): messages(messages), error(error) { }
    virtual std::string toString();
};

/** NOTIFICATIONS **/

/** AddUserToGroupNotification **/

class AddUserToGroupNotification: public MessageOutgoing
{
    std::string path, username;
    ResponseErrorCode error;
public:
    AddUserToGroupNotification(std::string path, std::string username, ResponseErrorCode error = ResponseErrorCode::OK) : path(path), username(username), error(error) { }
    virtual std::string toString();
};

/** AddedToGroupNotification **/

class AddedToGroupNotification: public MessageOutgoing
{
    std::string path;
    bool moderator;
    ResponseErrorCode error;
public:
    AddedToGroupNotification(std::string path, bool moderator, ResponseErrorCode error = ResponseErrorCode::OK) : path(path), moderator(moderator), error(error) { }
    virtual std::string toString();
};

/** RemovedFromGroupNotification **/

class RemovedFromGroupNotification: public MessageOutgoing
{
    std::string path;
    ResponseErrorCode error;
public:
    RemovedFromGroupNotification(std::string path, ResponseErrorCode error = ResponseErrorCode::OK) : path(path), error(error) { }
    virtual std::string toString();
};

/** NewMessageNotification **/

class NewMessageNotification: public MessageOutgoing
{
    std::string path;
    Model::GroupMessage message;
    ResponseErrorCode error;
public:
    NewMessageNotification(std::string path, Model::GroupMessage message, ResponseErrorCode error = ResponseErrorCode::OK) : path(path), message(message), error(error) { }
    virtual std::string toString();
};

#endif // MESSAGEOUTGOING_H
