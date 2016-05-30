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
    j["paths"] = paths;
    if (error != ResponseErrorCode::OK)
        j["error"] = static_cast<unsigned int>(error);
    return j.dump();
}

/** CreateAccountResponse **/

std::string CreateAccountResponse::toString()
{
    nlohmann::json j;
    j["response"] = "createAccount";
    if (error != ResponseErrorCode::OK)
        j["error"] = static_cast<unsigned int>(error);
    return j.dump();
}

/** createSubgroupResponse **/

std::string CreateSubgroupResponse::toString()
{
    nlohmann::json j;
    j["response"] = "createGroup";
    if (error != ResponseErrorCode::OK)
        j["error"] = static_cast<unsigned int>(error);
    return j.dump();
}

/** removeSubgroupResponse **/

std::string RemoveSubgroupResponse::toString()
{
    nlohmann::json j;
    j["response"] = "removeGroup";
    if (error != ResponseErrorCode::OK)
        j["error"] = static_cast<unsigned int>(error);
    return j.dump();
}

/** GetSubgroupsResponse **/

std::string GetSubgroupsResponse::toString()
{
    nlohmann::json j;
    j["response"] = "getSubgroups";
    j["path"] = path;
    j["subgroups"] = subgroups;
    if (error != ResponseErrorCode::OK)
        j["error"] = static_cast<unsigned int>(error);
    return j.dump();
}

/** AddUserToGroupResponse **/

std::string AddUserToGroupResponse::toString()
{
    nlohmann::json j;
    j["response"] = "addUserToGroup";
    if (error != ResponseErrorCode::OK)
        j["error"] = static_cast<unsigned int>(error);
    return j.dump();
}

/** RemoveUserFromGroupResponse **/

std::string RemoveUserFromGroupResponse::toString()
{
    nlohmann::json j;
    j["response"] = "removeUserFromGroup";
    if (error != ResponseErrorCode::OK)
        j["error"] = static_cast<unsigned int>(error);
    return j.dump();
}

/** GetGroupUsersResponse **/

std::string GetGroupUsersResponse::toString()
{
    nlohmann::json j;
    j["response"] = "getGroupUsers";
    j["path"] = path;
    nlohmann::json usersArray = nlohmann::json::array({});

    for (auto user : users)
    {
        nlohmann::json tmp;
        std::string role_str;
        tmp["login"] = user.first;
        if(user.second == MemberRole::Member)
            role_str = "standard";
        else if (user.second == MemberRole::Moderator)
            role_str = "moderator";
        else if (user.second == MemberRole::PendingApproval)
            role_str = "pending";

        tmp["role"] = role_str;
        usersArray.push_back(tmp);
    }
    j["users"] = usersArray;

    if (error != ResponseErrorCode::OK)
        j["error"] = static_cast<unsigned int>(error);
    return j.dump();
}

/** AddMeToGroupResponse **/

std::string AddMeToGroupResponse::toString()
{
    nlohmann::json j;
    j["response"] = "addMeToGroup";
    if (error != ResponseErrorCode::OK)
        j["error"] = static_cast<unsigned int>(error);
    return j.dump();
}

/** SendMessageResponse **/

std::string SendMessageResponse::toString()
{
    nlohmann::json j;
    j["response"] = "sendMessage";
    if (error != ResponseErrorCode::OK)
        j["error"] = static_cast<unsigned int>(error);
    return j.dump();
}

/** GetMessagesResponse **/

std::string GetMessagesResponse::toString()
{
    nlohmann::json j;
    j["response"] = "getMessages";
    j["path"] = path;

    std::vector<nlohmann::json> msgVect;
    for (auto msg : messages)
    {
        msgVect.push_back(msg.toJSON());
    }
    j["messages"] = msgVect;

    if (error != ResponseErrorCode::OK)
        j["error"] = static_cast<unsigned int>(error);
    return j.dump();
}

/** SetMemberPermissionResponse **/

std::string SetMemberPermissionResponse::toString()
{
    nlohmann::json j;
    j["response"] = "setMemberPermission";
    if (error != ResponseErrorCode::OK)
        j["error"] = static_cast<unsigned int>(error);
    return j.dump();
}

/** AddedToGroupNotification **/

std::string AddedToGroupNotification::toString()
{
    nlohmann::json j;
    j["notification"] = "addedToGroup";
    j["username"] = username;
    std::string role_str;
    if(role == MemberRole::Member)
        role_str = "standard";
    else if (role == MemberRole::Moderator)
        role_str = "moderator";
    else if (role == MemberRole::PendingApproval)
        role_str = "pending";
    j["role"] = role_str;
    j["path"] = path;
    return j.dump();
}

/** RemovedFromGroupNotification **/

std::string RemovedFromGroupNotification::toString()
{
    nlohmann::json j;
    j["notification"] = "removedFromGroup";
    j["username"] = username;
    j["path"] = path;
    return j.dump();
}

/** NewMessageNotification **/

std::string NewMessageNotification::toString()
{
    nlohmann::json j;
    j["notification"] = "newMessage";
    j["path"] = path;
    j["message"] = message.toJSON();
    return j.dump();
}

/** ModifiedMemberPermissionNotification **/
std::string ModifiedMemberPermissionNotification::toString()
{
    nlohmann::json j;
    j["notification"] = "modifiedMemberPermission";
    j["username"] = username;
    j["path"] = path;

    std::string role_str;
    if(role == MemberRole::Member)
        role_str = "standard";
    else if (role == MemberRole::Moderator)
        role_str = "moderator";
    else if (role == MemberRole::PendingApproval)
        role_str = "pending";
    j["role"] = role_str;

    return j.dump();
}

/** AddedSubgroupNotification **/
std::string AddedSubgroupNotification::toString()
{
    nlohmann::json j;
    j["notification"] = "addedSubgroup";
    j["path"] = path;
    j["subgroup"] = subgroup;
    return j.dump();
}

/** RemovedSubgroupNotification **/
std::string RemovedSubgroupNotification::toString()
{
    nlohmann::json j;
    j["notification"] = "removedSubgroup";
    j["path"] = path;
    j["subgroup"] = subgroup;
    return j.dump();
}

/** ResetNotification **/
std::string ResetNotification::toString()
{
    nlohmann::json j;
    j["notification"] = "reset";
    j["listeningOnPort"] = "port";
    return j.dump();
}
