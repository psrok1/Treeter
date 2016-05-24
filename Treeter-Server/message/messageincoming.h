#ifndef MESSAGEINCOMING_H
#define MESSAGEINCOMING_H

#include "messageprocessor.h"
#include "json.hpp"
#include <string>
#include <memory>

class MessageIncoming
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

/** AddUserToGroupRequest **/

class AddUserToGroupRequest: public MessageIncoming
{
public:
    AddUserToGroupRequest(nlohmann::json obj): MessageIncoming(obj) { }
    virtual bool process(MessageProcessor &processor) const { return processor.processRequest(*this); }
    std::string getUsername() const;
    std::string getPath() const;
    bool getModerator() const;
};

/** RemoveUserFromGroupRequest **/

class RemoveUserFromGroupRequest: public MessageIncoming
{
public:
    RemoveUserFromGroupRequest(nlohmann::json obj): MessageIncoming(obj) { }
    virtual bool process(MessageProcessor &processor) const { return processor.processRequest(*this); }
    std::string getUsername() const;
    std::string getPath() const;
};

/** GetGroupPathsRequest **/

class GetGroupPathsRequest: public MessageIncoming
{
public:
    GetGroupPathsRequest(nlohmann::json obj): MessageIncoming(obj) { }
    virtual bool process(MessageProcessor &processor) const { return processor.processRequest(*this); }
};


/** GetGroupUsersRequest **/

class GetGroupUsersRequest: public MessageIncoming
{
public:
    GetGroupUsersRequest(nlohmann::json obj): MessageIncoming(obj) { }
    virtual bool process(MessageProcessor &processor) const { return processor.processRequest(*this); }
    std::string getPath() const;
};


/** AddMeToGroupRequest **/

class AddMeToGroupRequest: public MessageIncoming
{
public:
    AddMeToGroupRequest(nlohmann::json obj): MessageIncoming(obj) { }
    virtual bool process(MessageProcessor &processor) const { return processor.processRequest(*this); }
    std::string getPath() const;
};

/** GetGroupPendingUsers **/

class GetGroupPendingUsersRequest: public MessageIncoming
{
    GetGroupPendingUsersRequest(nlohmann::json obj): MessageIncoming(obj) { }
    virtual bool process(MessageProcessor &processor) const { return processor.processRequest(*this); }
    std::string getPath() const;
};


#endif // MESSAGEINCOMING_H
