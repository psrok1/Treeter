#include "json.hpp"
#include "messageoutgoing.h"

void MessageOutgoing::send()
{
    this->connection->sendMessage(this->toString());
}

std::string EchoResponse::toString()
{
    nlohmann::json j;
    j["response"] = "echo";
    j["message"] = content;
    return j.dump();
}

std::string HelloResponse::toString()
{
    nlohmann::json j;
    j["response"] = "hello";
    j["publicKey"] = publicKey;
    return j.dump();
}

std::string StartEncryptionResponse::toString()
{
    nlohmann::json j;
    j["response"] = "startEncryption";
    return j.dump();
}
