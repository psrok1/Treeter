#ifndef MESSAGEINCOMING_H
#define MESSAGEINCOMING_H

#include "messagebase.h"
#include "messageprocessor.h"
#include "json.hpp"
#include <string>
#include <memory>

class MessageIncoming: public MessageBase
{
protected:
    nlohmann::json json_object;
public:
    typedef std::shared_ptr<MessageIncoming> Reference;

    static MessageIncoming::Reference fromString(std::string message);
    virtual bool process(MessageProcessor&) const = 0;

    MessageIncoming(nlohmann::json obj): json_object(obj) { }
};

/** EchoRequest **/

class EchoRequest: public MessageIncoming
{
public:
    std::string getMessage() const;
    virtual bool process(MessageProcessor& processor) const { return processor.processRequest(*this); }

    EchoRequest(nlohmann::json obj): MessageIncoming(obj) { }
};

/** HelloRequest **/

class HelloRequest: public MessageIncoming
{
public:
    HelloRequest(nlohmann::json obj): MessageIncoming(obj) { }
    virtual bool process(MessageProcessor& processor) const { return processor.processRequest(*this); }
};

/** StartEncryptionRequest **/

class StartEncryptionRequest: public MessageIncoming
{
public:
    StartEncryptionRequest(nlohmann::json obj): MessageIncoming(obj) { }
    virtual bool process(MessageProcessor& processor) const { return processor.processRequest(*this); }
    std::string getEncryptedKey() const;
};

/** AuthUserRequest **/

class AuthUserRequest: public MessageIncoming
{
public:
    AuthUserRequest(nlohmann::json obj): MessageIncoming(obj) { }
    virtual bool process(MessageProcessor& processor) const { return processor.processRequest(*this); }
    std::string getLogin() const;
    std::string getPassword() const;
};

/** CreateAccountRequest **/

class CreateAccountRequest: public MessageIncoming
{
public:
    CreateAccountRequest(nlohmann::json obj): MessageIncoming(obj) { }
    virtual bool process(MessageProcessor &processor) const { return processor.processRequest(*this); }
    std::string getLogin() const;
    std::string getPassword() const;
};

/** CreateGroupRequest **/

class CreateGroupRequest: public MessageIncoming
{
public:
    CreateGroupRequest(nlohmann::json obj): MessageIncoming(obj) { }
    virtual bool process(MessageProcessor &processor) const { return processor.processRequest(*this); }
    std::string getPath() const;
};

/** RemoveGroupRequest **/

class RemoveGroupRequest: public MessageIncoming
{
public:
    RemoveGroupRequest(nlohmann::json obj): MessageIncoming(obj) { }
    virtual bool process(MessageProcessor &processor) const { return processor.processRequest(*this); }
    std::string getPath() const;
};

/** GetSubgroupsRequest **/

class GetSubgroupsRequest: public MessageIncoming
{
public:
    GetSubgroupsRequest(nlohmann::json obj): MessageIncoming(obj) { }
    virtual bool process(MessageProcessor &processor) const { return processor.processRequest(*this); }
    std::string getPath() const;
};

#endif // MESSAGEINCOMING_H
