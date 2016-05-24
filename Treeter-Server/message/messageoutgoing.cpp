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

MessageOutgoing::Reference EchoResponse::clone()
{
    return MessageOutgoing::Reference(new EchoResponse(content));
}

/** HelloResponse **/

std::string HelloResponse::toString()
{
    nlohmann::json j;
    j["response"] = "hello";
    j["publicKey"] = publicKey;
    return j.dump();
}

MessageOutgoing::Reference HelloResponse::clone()
{
    return MessageOutgoing::Reference(new HelloResponse(publicKey));
}

/** StartEncryptionResponse **/

std::string StartEncryptionResponse::toString()
{
    nlohmann::json j;
    j["response"] = "startEncryption";
    return j.dump();
}

MessageOutgoing::Reference StartEncryptionResponse::clone()
{
    return MessageOutgoing::Reference(new StartEncryptionResponse());
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

MessageOutgoing::Reference AuthUserResponse::clone()
{
    return MessageOutgoing::Reference(new AuthUserResponse());
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

MessageOutgoing::Reference CreateAccountResponse::clone()
{
    return MessageOutgoing::Reference(new CreateAccountResponse());
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

MessageOutgoing::Reference CreateGroupResponse::clone()
{
    return MessageOutgoing::Reference(new CreateGroupResponse());
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

MessageOutgoing::Reference RemoveGroupResponse::clone()
{
    return MessageOutgoing::Reference(new RemoveGroupResponse());
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

MessageOutgoing::Reference GetSubgroupsResponse::clone()
{
    return MessageOutgoing::Reference(new GetSubgroupsResponse(subgroups));
}
