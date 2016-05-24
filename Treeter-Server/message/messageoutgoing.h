#ifndef MESSAGEOUTGOING_H
#define MESSAGEOUTGOING_H

#include <string>
#include <vector>
#include <memory>

enum class ResponseErrorCode { OK };

class MessageOutgoing
{
public:
    typedef std::shared_ptr<MessageOutgoing> Reference;
    virtual std::string toString() = 0;
    virtual bool isEOF() { return false; }
};

/** EOFMessage **/

class EOFMessage : public MessageOutgoing
{
public:
    virtual bool isEOF() { return true; }
    virtual std::string toString() { return std::string(); }
};

/** EchoResponse **/

class EchoResponse : public MessageOutgoing
{
    std::string content;
public:
    EchoResponse(std::string message): content(message) { }
    virtual std::string toString();
};

/** HelloResponse **/

class HelloResponse: public MessageOutgoing
{
    std::string publicKey;
public:
    HelloResponse(std::string pubKey): publicKey(pubKey) { }
    virtual std::string toString();
};

/** StartEncryptionResponse **/

class StartEncryptionResponse: public MessageOutgoing
{
public:
    virtual std::string toString();
};

/** AuthUserResponse **/

class AuthUserResponse: public MessageOutgoing
{
    ResponseErrorCode error;
public:
    AuthUserResponse(ResponseErrorCode error = ResponseErrorCode::OK): error(error) { }
    virtual std::string toString();
};

/** CreateAccountResponse **/

class CreateAccountResponse: public MessageOutgoing
{
    ResponseErrorCode error;
public:
    CreateAccountResponse(ResponseErrorCode error = ResponseErrorCode::OK): error(error) { }
    virtual std::string toString();
};

/** CreateGroupResponse **/

class CreateGroupResponse: public MessageOutgoing
{
    ResponseErrorCode error;
public:
    CreateGroupResponse(ResponseErrorCode error = ResponseErrorCode::OK): error(error) { }
    virtual std::string toString();
};

/** RemoveGroupResponse **/

class RemoveGroupResponse: public MessageOutgoing
{
    ResponseErrorCode error;
public:
    RemoveGroupResponse(ResponseErrorCode error = ResponseErrorCode::OK): error(error) { }
    virtual std::string toString();
};

/** GetSubgroupsResponse **/

class GetSubgroupsResponse: public MessageOutgoing
{
    std::vector<std::string> subgroups;
    ResponseErrorCode error;
public:
    GetSubgroupsResponse(std::vector<std::string> subgroups, ResponseErrorCode error = ResponseErrorCode::OK): subgroups(subgroups), error(error) { }
    virtual std::string toString();
};
#endif // MESSAGEOUTGOING_H
