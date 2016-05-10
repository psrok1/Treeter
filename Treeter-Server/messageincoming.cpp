#include "messageincoming.h"
#include "messageincomingctor.h"

using json = nlohmann::json;

MessageIncomingCtorMap ctorMap(
{
    {"echo", messageIncomingCtor<EchoRequest>()}
});

MessageIncoming::Reference MessageIncoming::fromString(std::string message)
{
    json obj = json::parse(message);
    return ctorMap[obj["request"]](obj);
}

unsigned int MessageIncoming::getId() const
{
    return json_object["requestId"];
}

std::string EchoRequest::getMessage() const
{
    return json_object["message"];
}
