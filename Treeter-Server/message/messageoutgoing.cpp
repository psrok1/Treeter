#include "json.hpp"
#include "messageoutgoing.h"

/** EchoResponse **/

std::string EchoResponse::toString()
{
    nlohmann::json j;
    j["response"] = "echo";
    j["message"] = content;
    return j.dump();
}

/** HelloResponse **/

std::string HelloResponse::toString()
{
    nlohmann::json j;
    j["response"] = "hello";
    j["publicKey"] = publicKey;
    return j.dump();
}

/** StartEncryptionResponse **/

std::string StartEncryptionResponse::toString()
{
    nlohmann::json j;
    j["response"] = "startEncryption";
    return j.dump();
}

/** AuthUserResponse **/

std::string AuthUserResponse::toString()
{
    nlohmann::json j;
    j["response"] = "authUser";
    if (error != ResponseErrorCode::OK)
        ;   // TODO
    return j.dump();
}

/** CreateAccountResponse **/

std::string CreateAccountResponse::toString()
{
    nlohmann::json j;
    j["response"] = "createAccount";
    if (error != ResponseErrorCode::OK)
        ;   // TODO
    return j.dump();
}

/** CreateGroupResponse **/

std::string CreateGroupResponse::toString()
{
    nlohmann::json j;
    j["response"] = "createGroup";
    if (error != ResponseErrorCode::OK)
        ;   // TODO
    return j.dump();
}

/** RemoveGroupResponse **/

std::string RemoveGroupResponse::toString()
{
    nlohmann::json j;
    j["response"] = "removeGroup";
    if (error != ResponseErrorCode::OK)
        ;   // TODO
    return j.dump();
}

/** GetSubgroupsResponse **/

std::string GetSubgroupsResponse::toString()
{
    nlohmann::json j;
    j["response"] = "getSubgroups";
    j["subgroups"] = subgroups;
    if (error != ResponseErrorCode::OK)
        ;   // TODO
    return j.dump();
}

/** AddUserToGroupResponse **/

std::string AddUserToGroupResponse::toString()
{
    nlohmann::json j;
    j["response"] = "addUserToGroup";
    if (error != ResponseErrorCode::OK)
        ;   // TODO
    return j.dump();
}

/** RemoveUserFromGroupResponse **/

std::string RemoveUserFromGroupResponse::toString()
{
    nlohmann::json j;
    j["response"] = "removeUserFromGroup";
    if (error != ResponseErrorCode::OK)
        ;   // TODO
    return j.dump();
}

/** GetGroupPathsResponse **/

std::string GetGroupPathsResponse::toString()
{
    nlohmann::json j;
    j["response"] = "getGroupPaths";
    j["paths"] = groupPaths;
    if (error != ResponseErrorCode::OK)
        ;   // TODO
    return j.dump();
}

/** GetGroupUsersResponse **/

std::string GetGroupUsersResponse::toString()
{
    nlohmann::json j;
    j["response"] = "getGroupUsers";
    j["moderators"] = moderators;
    j["users"] = users;
    if (error != ResponseErrorCode::OK)
        ;   // TODO
    return j.dump();
}

/** AddMeToGroupResponse **/

std::string AddMeToGroupResponse::toString()
{
    nlohmann::json j;
    j["response"] = "addMeToGroup";
    if (error != ResponseErrorCode::OK)
        ;   // TODO
    return j.dump();
}

/** GetGroupPendingUsersResponse **/

std::string GetGroupPendingUsersResponse::toString()
{
    nlohmann::json j;
    j["response"] = "getGroupPendingUsers";
    if (error != ResponseErrorCode::OK)
        ;   // TODO
    return j.dump();
}

/** SendMessageResponse **/

std::string SendMessageResponse::toString()
{
    nlohmann::json j;
    j["response"] = "sendMessage";
    if (error != ResponseErrorCode::OK)
        ;   // TODO
    return j.dump();
}

/** GetMessagesResponse **/

std::string GetMessagesResponse::toString()
{
    nlohmann::json j;
    j["response"] = "getMessages";

    std::vector<std::string> msgVect;
    for (auto msg : messages)
    {
        msgVect.push_back(msg.toString());
    }
    j["messages"] = msgVect;

    if (error != ResponseErrorCode::OK)
        ;   // TODO
    return j.dump();
}

/** AddUserToGroupNotification **/

std::string AddUserToGroupNotification::toString()
{
    nlohmann::json j;
    j["notification"] = "addUserToGroup";
    j["path"] = path;
    j["username"] = username;
    if (error != ResponseErrorCode::OK)
        ;   // TODO
    return j.dump();
}

/** AddedToGroupNotification **/

std::string AddedToGroupNotification::toString()
{
    nlohmann::json j;
    j["notification"] = "addedToGroup";
    j["path"] = path;
    j["moderator"] = moderator;
    if (error != ResponseErrorCode::OK)
        ;   // TODO
    return j.dump();
}

/** RemovedFromGroupNotification **/

std::string RemovedFromGroupNotification::toString()
{
    nlohmann::json j;
    j["notification"] = "removedFromGroup";
    j["path"] = path;
    if (error != ResponseErrorCode::OK)
        ;   // TODO
    return j.dump();
}

/** NewMessageNotification **/

std::string NewMessageNotification::toString()
{
    nlohmann::json j;
    j["notification"] = "newMessage";
    j["path"] = path;
    j["message"] = message.toString();
    if (error != ResponseErrorCode::OK)
        ;   // TODO
    return j.dump();
}
