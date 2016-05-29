#include "messageprocessor.h"
#include "messageincoming.h"
#include "messageoutgoing.h"
#include "../server.h"

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
    // TODO
    (void)req;
    return true;
}

/** CreateAccountRequest **/

bool MessageProcessor::processRequest(const CreateAccountRequest &req)
{
    // TODO
    (void)req;
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
