#include "json.hpp"
#include "messageoutgoing.h"

void MessageOutgoing::send()
{
    this->connection->sendString(this->toString());
}

void MessageOutgoing::setConnection(Connection::Reference connection)
{
    this->connection = connection;
}

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
