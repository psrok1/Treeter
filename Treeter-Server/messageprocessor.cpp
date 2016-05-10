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
                new EchoResponse(connection, req.getId(), req.getMessage()));
    this->sender->send(response);
    return true;
}
