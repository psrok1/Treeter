#include "json.hpp"
#include "messageoutgoing.h"

void MessageOutgoing::send()
{
    this->connection->sendMessage(this->toString());
}

std::string EchoResponse::toString()
{
    nlohmann::json j;
    j["type"] = "response";
    j["responseId"] = id;
    j["message"] = content;
    return j.dump();
}

std::string HelloResponse::toString()
{
    nlohmann::json j;
    j["type"] = "response";
    return j.dump();
}

std::string StartEncryptionResponse::toString()
{
    nlohman::json j;
    j["type"] = "response";
    return j.dump();
}
