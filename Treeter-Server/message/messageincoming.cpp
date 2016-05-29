#include "messageincoming.h"
#include "messageincomingctor.h"
#include <chrono>
#include <stdexcept>

using json = nlohmann::json;

MessageIncomingCtorMap ctorMap(
{
    {"echo", messageIncomingCtor<EchoRequest>()},
    {"hello", messageIncomingCtor<HelloRequest>()},
    {"startEncryption", messageIncomingCtor<StartEncryptionRequest>()},
    {"createAccount", messageIncomingCtor<CreateAccountRequest>()},
    {"authUser", messageIncomingCtor<AuthUserRequest>()},
    {"getGroupUsers", messageIncomingCtor<GetGroupUsersRequest>()},
    {"getMessages", messageIncomingCtor<GetMessagesRequest>()},
    {"sendMessage", messageIncomingCtor<SendMessageRequest>()},
    {"addMeToGroup", messageIncomingCtor<AddMeToGroupRequest>()},
    {"createSubgroup", messageIncomingCtor<CreateSubgroupRequest>()},
    {"removeSubgroup", messageIncomingCtor<RemoveSubgroupRequest>()},
    {"getSubgroups", messageIncomingCtor<GetSubgroupsRequest>()},
    {"addUserToGroup", messageIncomingCtor<AddUserToGroupRequest>()},
    {"removeUserFromGroup", messageIncomingCtor<RemoveUserFromGroupRequest>()},
    {"setMemberPermission", messageIncomingCtor<SetMemberPermissionRequest>()}
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

/** createSubgroupRequest **/

std::string CreateSubgroupRequest::getParentPath() const
{
    return json_object["path"];
}

std::string CreateSubgroupRequest::getSubgroupName() const
{
    return json_object["subgroup"];
}

/** removeSubgroupRequest **/

std::string RemoveSubgroupRequest::getParentPath() const
{
    return json_object["path"];
}

std::string RemoveSubgroupRequest::getSubgroupName() const
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

/** SendMessageRequest **/

std::string SendMessageRequest::getPath() const
{
    return json_object["path"];
}

std::string SendMessageRequest::getMessage() const
{
    return json_object["content"];
}

/** GetMessagesRequest **/

std::string GetMessagesRequest::getPath() const
{
    return json_object["path"];
}


/** SetMemberPermissionRequest **/

std::string SetMemberPermissionRequest::getUsername() const
{
    return json_object["username"];
}

MemberRole SetMemberPermissionRequest::getRole() const
{
    std::string role_string = json_object["role"];
    if (role_string == "standard")
        return MemberRole::Member;
    else if (role_string == "moderator")
        return MemberRole::Moderator;
    else if (role_string == "pending")
        return MemberRole::PendingApproval;
    // maybe throw error here
    throw new std::runtime_error("Unknown SetMemberPermission role value");
}
