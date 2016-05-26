#include "messageincoming.h"
#include "messageincomingctor.h"
#include <chrono>

using json = nlohmann::json;

MessageIncomingCtorMap ctorMap(
{
    {"echo", messageIncomingCtor<EchoRequest>()},
    {"hello", messageIncomingCtor<HelloRequest>()},
    {"startEncryption", messageIncomingCtor<StartEncryptionRequest>()}
});

MessageIncoming::Reference MessageIncoming::fromString(std::string message)
{
    json obj = json::parse(message);
    return ctorMap[obj["request"]](obj);
}

/** EchoRequest **/

std::string EchoRequest::getMessage() const
{
    return json_object["message"];
}

/** StartEncryptionRequest **/

std::string StartEncryptionRequest::getEncryptedKey() const
{
    return json_object["encryptedKey"];
}

/** AuthUserRequest **/

std::string AuthUserRequest::getLogin() const
{
    return json_object["login"];
}

std::string AuthUserRequest::getPassword() const
{
    return json_object["password"];
}

/** CreateAccountRequest **/

std::string CreateAccountRequest::getLogin() const
{
    return json_object["login"];
}

std::string CreateAccountRequest::getPassword() const
{
    return json_object["password"];
}

/** CreateGroupRequest **/

std::string CreateGroupRequest::getParentPath() const
{
    return json_object["path"];
}

std::string CreateGroupRequest::getSubgroupName() const
{
    return json_object["subgroup"];
}

/** RemoveGroupRequest **/

std::string RemoveGroupRequest::getParentPath() const
{
    return json_object["path"];
}

std::string RemoveGroupRequest::getSubgroupName() const
{
    return json_object["subgroup"];
}


/** GetSubgroupsRequest **/

std::string GetSubgroupsRequest::getPath() const
{
    return json_object["path"];
}

/** AddUserToGroupRequest **/

std::string AddUserToGroupRequest::getUsername() const
{
    return json_object["username"];
}

std::string AddUserToGroupRequest::getPath() const
{
    return json_object["path"];
}

bool AddUserToGroupRequest::getModerator() const
{
    return json_object["moderator"];
}

/** RemoveUserFromGroupRequest **/

std::string RemoveUserFromGroupRequest::getUsername() const
{
    return json_object["username"];
}

std::string RemoveUserFromGroupRequest::getPath() const
{
    return json_object["path"];
}

/** GetGroupUsersRequest **/

std::string GetGroupUsersRequest::getPath() const
{
    return json_object["path"];
}

/** AddMeToGroupRequest **/

std::string AddMeToGroupRequest::getPath() const
{
    return json_object["path"];
}

/** GetGroupPendingUsersRequest **/

std::string GetGroupPendingUsersRequest::getPath() const
{
    return json_object["path"];
}

/** SendMessageRequest **/

std::string SendMessageRequest::getPath() const
{
    return json_object["path"];
}

Model::GroupMessage SendMessageRequest::getMessage() const
{
    std::string author = json_object["message"]["author"];
    std::string content = json_object["message"]["content"];
    return Model::GroupMessage(author, content);    // Ignore timestamp stored in incoming message
}

/** GetMessagesRequest **/

std::string GetMessagesRequest::getPath() const
{
    return json_object["path"];
}

std::chrono::system_clock::time_point GetMessagesRequest::getLastMsgTimestamp() const
{
    if (json_object.count("lastMsgTimestamp"))
    {
        return std::chrono::system_clock::from_time_t(json_object["lastMsgTimestamp"]);
    }
    return std::chrono::system_clock::from_time_t(0);
}
