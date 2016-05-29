#include "messageprocessor.h"
#include "messageincoming.h"
#include "messageoutgoing.h"
#include "../server.h"

#include "model/datamodel.h"
#include "model/group.h"
#include "model/user.h"

MessageProcessor::MessageProcessor(Connection::Reference conn): connection(conn) { }

/** EchoRequest **/

bool MessageProcessor::processRequest(const EchoRequest& req)
{
    MessageOutgoing::Reference response(new EchoResponse(req.getMessage()));

    this->connection->sendMessage(response);
    return true;
}

/** HelloRequest **/

bool MessageProcessor::processRequest(const HelloRequest &)
{
    connection->rsaContext = Crypto::RSAProvider::getKey();

    MessageOutgoing::Reference response(new HelloResponse(connection->rsaContext.getEncodedPublicKey()));

    this->connection->sendMessage(response);
    return true;
}

/** StartEncryptionRequest **/

bool MessageProcessor::processRequest(const StartEncryptionRequest &req)
{
    connection->aesContext = connection->rsaContext.decodeAESKey(req.getEncryptedKey());

    MessageOutgoing::Reference response(new StartEncryptionResponse());

    this->connection->sendMessage(response);
    return true;
}

/** AuthUserRequest **/

bool MessageProcessor::processRequest(const AuthUserRequest &req)
{
    auto userRef = this->connection->model->getUserByLogin(req.getLogin());

    if(!userRef || !userRef->verifyPassword(req.getPassword()))
    {
        MessageOutgoing::Reference response(new AuthUserResponse(ResponseErrorCode::AccessDenied));
        this->connection->sendMessage(response);
        return false;
    }

    // Add relation to current connection
    this->connection->user = userRef;

    // Add group subscriptions

    return true;
}

/** CreateAccountRequest **/

bool MessageProcessor::processRequest(const CreateAccountRequest &req)
{
    if(!Model::User::validateLogin(req.getLogin()))
    {
        MessageOutgoing::Reference response(new CreateAccountResponse(ResponseErrorCode::BadName));
        this->connection->sendMessage(response);
        return false;
    }

    auto userRef = this->connection->model->addUser(req.getLogin(), req.getPassword());

    if(!userRef)
    {
        MessageOutgoing::Reference response(new CreateAccountResponse(ResponseErrorCode::ObjectExist));
        this->connection->sendMessage(response);
        return false;
    }

    MessageOutgoing::Reference response(new CreateAccountResponse());
    this->connection->sendMessage(response);
    return true;
}

/** createSubgroupRequest **/

bool MessageProcessor::processRequest(const CreateSubgroupRequest &req)
{
    // TODO
    (void)req;
    return true;
}

/** removeSubgroupRequest **/

bool MessageProcessor::processRequest(const RemoveSubgroupRequest &req)
{
    // TODO
    (void)req;
    return true;
}

/** GetSubgroupsRequest **/

bool MessageProcessor::processRequest(const GetSubgroupsRequest &req)
{
    // TODO
    (void)req;
    return true;
}

/** AddUserToGroupRequest **/

bool MessageProcessor::processRequest(const AddUserToGroupRequest &req)
{
    // TODO
    (void)req;
    return true;
}

/** RemoveUserFromGroupRequest **/

bool MessageProcessor::processRequest(const RemoveUserFromGroupRequest &req)
{
    // TODO
    (void)req;
    return true;
}

/** GetGroupUsersRequest **/

bool MessageProcessor::processRequest(const GetGroupUsersRequest &req)
{
    // TODO
    (void)req;
    return true;
}

/** AddMeToGroupRequest **/

bool MessageProcessor::processRequest(const AddMeToGroupRequest &req)
{
    // TODO
    (void)req;
    return true;
}

/** SendMessageRequest **/

bool MessageProcessor::processRequest(const SendMessageRequest &req)
{
    // TODO
    (void)req;
    return true;
}

/** GetMessagesRequest **/

bool MessageProcessor::processRequest(const GetMessagesRequest &req)
{
    // TODO
    (void)req;
    return true;
}

/** SetMemberPermissionRequest **/

bool MessageProcessor::processRequest(const SetMemberPermissionRequest &req)
{
    // TODO
    (void)req;
    return true;
}
