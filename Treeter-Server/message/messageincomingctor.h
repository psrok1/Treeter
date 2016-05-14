#ifndef MESSAGEINCOMINGCTOR_H
#define MESSAGEINCOMINGCTOR_H

#include "messageincoming.h"
#include "json.hpp"
#include <functional>
#include <unordered_map>

typedef std::function<MessageIncoming::Reference(nlohmann::json)> MessageIncomingCtor;
typedef std::unordered_map<std::string, MessageIncomingCtor> MessageIncomingCtorMap;

template<typename T>
MessageIncomingCtor messageIncomingCtor()
{
    return [](nlohmann::json obj) { return MessageIncoming::Reference(new T(obj)); };
}

#endif // MESSAGEINCOMINGCTOR_H
