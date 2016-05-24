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
