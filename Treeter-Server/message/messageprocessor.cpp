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
    return true;
}

/** CreateAccountRequest **/

bool MessageProcessor::processRequest(const CreateAccountRequest &req)
{
    // TODO
    return true;
}

/** CreateGroupRequest **/

bool MessageProcessor::processRequest(const CreateGroupRequest &req)
{
    // TODO
    return true;
}

/** RemoveGroupRequest **/

bool MessageProcessor::processRequest(const RemoveGroupRequest &req)
{
    // TODO
    return true;
}

/** GetSubgroupsRequest **/

bool MessageProcessor::processRequest(const GetSubgroupsRequest &req)
{
    // TODO
    return true;
}

/** AddUserToGroupRequest **/

bool MessageProcessor::processRequest(const AddUserToGroupRequest &req)
{
    // TODO
    return true;
}

/** RemoveUserFromGroupRequest **/

bool MessageProcessor::processRequest(const RemoveUserFromGroupRequest &req)
{
    // TODO
    return true;
}

/** GetGroupPathsRequest **/

bool MessageProcessor::processRequest(const GetGroupPathsRequest &req)
{
    // TODO
    return true;
}

/** GetGroupUsersRequest **/

bool MessageProcessor::processRequest(const GetGroupUsersRequest &req)
{
    // TODO
    return true;
}

/** AddMeToGroupRequest **/

bool MessageProcessor::processRequest(const AddMeToGroupRequest &req)
{
    // TODO
    return true;
}

/** GetGroupPendingUsers **/

bool MessageProcessor::processRequest(const GetGroupPendingUsersRequest &req)
{
    // TODO
    return true;
}
