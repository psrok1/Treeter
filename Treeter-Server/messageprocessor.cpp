#include "messageprocessor.h"
#include "messagebase.h"
#include "messageincoming.h"
#include "messageoutgoing.h"
#include "server.h"

MessageProcessor::MessageProcessor(Connection::Reference conn): connection(conn)
{
    this->sender = conn->server->getSender();
}

bool MessageProcessor::processRequest(const EchoRequest& req)
{
    MessageBase::Reference response(
                new EchoResponse(connection, req.getMessage()));
    this->sender->send(response);
    return true;
}

bool MessageProcessor::processRequest(const HelloRequest &)
{
    connection->rsaContext = Crypto::RSAProvider::getKey();
    MessageBase::Reference response(new HelloResponse(connection, connection->rsaContext.getEncodedPublicKey()));
    this->sender->send(response);
    return true;
}

bool MessageProcessor::processRequest(const StartEncryptionRequest &req)
{
    connection->aesContext = connection->rsaContext.decodeAESKey(req.getEncryptedKey());

    MessageBase::Reference response(new StartEncryptionResponse(connection));
    this->sender->send(response);
    return true;
}
