#include "messageincoming.h"
#include "messageincomingctor.h"

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
