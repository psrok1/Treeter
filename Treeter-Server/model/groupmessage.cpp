#include "groupmessage.h"
#include "message/json.hpp"

const nlohmann::json Model::GroupMessage::toJSON() const
{
    nlohmann::json j;
    j["author"] = author;
    j["message"] = message;
    j["timestamp"] = std::chrono::system_clock::to_time_t(timestamp);
    return j;
}
