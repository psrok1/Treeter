#ifndef MESSAGEOUTGOING_H
#define MESSAGEOUTGOING_H

#include <string>
#include <vector>
#include <list>
#include <memory>
#include <model/groupmessage.h>
#include <model/memberrole.h>

enum class ResponseErrorCode
{
    OK = 0,
    BadName = 1,
    NotAMember = 2,
    AccessDenied = 3,
    MultipleSessionsNotAllowed = 4,
    RootNotAllowed = 5,
    UserExist = 6,
    UserNotExist = 7,
    GroupExist = 8,
    GroupNotExist = 9,
    MemberExist = 10,
    MemberNotExist = 11,
    NotLoggedIn = 12
};

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
    std::list<std::string> paths;
    ResponseErrorCode error;
public:
    AuthUserResponse(std::list<std::string> paths, ResponseErrorCode error = ResponseErrorCode::OK): paths(paths), error(error) { }
    AuthUserResponse(ResponseErrorCode error): AuthUserResponse(std::list<std::string>(), error) { }
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

/** CreateSubgroupResponse **/

class CreateSubgroupResponse: public MessageOutgoing
{
    ResponseErrorCode error;
public:
    CreateSubgroupResponse(ResponseErrorCode error = ResponseErrorCode::OK): error(error) { }
    virtual std::string toString();
};

/** RemoveSubgroupResponse **/

class RemoveSubgroupResponse: public MessageOutgoing
{
    ResponseErrorCode error;
public:
    RemoveSubgroupResponse(ResponseErrorCode error = ResponseErrorCode::OK): error(error) { }
    virtual std::string toString();
};

/** GetSubgroupsResponse **/

class GetSubgroupsResponse: public MessageOutgoing
{
    std::string path;
    std::list<std::string> subgroups;
    ResponseErrorCode error;
public:
    GetSubgroupsResponse(std::string path, std::list<std::string> subgroups, ResponseErrorCode error = ResponseErrorCode::OK): path(path), subgroups(subgroups), error(error) { }
    GetSubgroupsResponse(ResponseErrorCode error): GetSubgroupsResponse("", std::list<std::string>(), error) { }
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

/** GetGroupUsersResponse **/

class GetGroupUsersResponse: public MessageOutgoing
{
    std::string path;
    std::list<std::pair<std::string, MemberRole>> users;
    ResponseErrorCode error;
public:
    GetGroupUsersResponse(std::string path, std::list<std::pair<std::string, MemberRole>> users, ResponseErrorCode error = ResponseErrorCode::OK) : path(path), users(users), error(error) { }
    GetGroupUsersResponse(ResponseErrorCode error): GetGroupUsersResponse("", std::list<std::pair<std::string, MemberRole>>(), error) { }
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
    std::string path;
    std::list<Model::GroupMessage> messages;
    ResponseErrorCode error;
public:
    GetMessagesResponse(std::string path, std::list<Model::GroupMessage> messages, ResponseErrorCode error = ResponseErrorCode::OK): path(path), messages(messages), error(error) { }
    GetMessagesResponse(ResponseErrorCode error): GetMessagesResponse("", std::list<Model::GroupMessage>(), error) { }
    virtual std::string toString();
};

/** SetMemberPermissionResponse **/

class SetMemberPermissionResponse: public MessageOutgoing
{
    ResponseErrorCode error;
public:
    SetMemberPermissionResponse(ResponseErrorCode error = ResponseErrorCode::OK) : error(error) { }
    virtual std::string toString();
};

/** NOTIFICATIONS **/

/** AddedToGroupNotification **/

class AddedToGroupNotification: public MessageOutgoing
{
    std::string path, username;
    MemberRole role;
public:
    AddedToGroupNotification(std::string path, std::string username, MemberRole role) : path(path), username(username), role(role) { }
    virtual std::string toString();
};

/** RemovedFromGroupNotification **/

class RemovedFromGroupNotification: public MessageOutgoing
{
    std::string path, username;
public:
    RemovedFromGroupNotification(std::string path, std::string username) : path(path), username(username) { }
    virtual std::string toString();
};

/** NewMessageNotification **/

class NewMessageNotification: public MessageOutgoing
{
    std::string path;
    Model::GroupMessage message;
public:
    NewMessageNotification(std::string path, Model::GroupMessage message) : path(path), message(message) { }
    virtual std::string toString();
};

/** ModifiedMemberPermissionNotification **/

class ModifiedMemberPermissionNotification: public MessageOutgoing
{
    std::string username, path;
    MemberRole role;
public:
    ModifiedMemberPermissionNotification(std::string username, std::string path, MemberRole role) : username(username), path(path), role(role) { }
    virtual std::string toString();
};

/** AddedSubgroupNotification **/

class AddedSubgroupNotification: public MessageOutgoing
{
    std::string path, subgroup;
public:
    AddedSubgroupNotification(std::string path, std::string subgroup): path(path), subgroup(subgroup) { }
    virtual std::string toString();
};

/** RemovedSubgroupNotification **/

class RemovedSubgroupNotification: public MessageOutgoing
{
    std::string path, subgroup;
public:
    RemovedSubgroupNotification(std::string path, std::string subgroup): path(path), subgroup(subgroup) { }
    virtual std::string toString();
};

/** ResetNotification **/

class ResetNotification: public MessageOutgoing
{
    unsigned int port;
public:
    ResetNotification(unsigned int port) : port(port) { }
    virtual std::string toString();
};

#endif // MESSAGEOUTGOING_H
