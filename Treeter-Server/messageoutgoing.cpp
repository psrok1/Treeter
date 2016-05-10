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
