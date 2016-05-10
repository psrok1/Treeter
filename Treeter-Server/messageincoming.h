#ifndef MESSAGEINCOMING_H
#define MESSAGEINCOMING_H

#include "messagebase.h"
#include "messageprocessor.h"
#include "json.hpp"
#include <string>
#include <memory>

class MessageIncoming: public MessageBase
{
    virtual void send() { }
protected:
    nlohmann::json json_object;
public:
    typedef std::shared_ptr<MessageIncoming> Reference;

    static MessageIncoming::Reference fromString(std::string message);
    unsigned int getId() const;
    virtual bool process(MessageProcessor&) const = 0;

    MessageIncoming(nlohmann::json obj): json_object(obj) { }
};

class EchoRequest: public MessageIncoming
{
public:
    std::string getMessage() const;
    virtual bool process(MessageProcessor& processor) const
    {
        return processor.processRequest(*this);
    }

    EchoRequest(nlohmann::json obj): MessageIncoming(obj) { }
};

#endif // MESSAGEINCOMING_H
