#include "messagesender.h"
#include "connection.h"

void MessageSender::operator()(Reference)
{
    for(;;)
    {
        MessageBase::Reference msg = messageQueue.get();
        if(msg->isEOF())
        {
            // If stop signal: break sender loop
            break;
        }
        this->connection->sendString(msg->serialize());
    }
}

void MessageSender::stopThread()
{
    MessageBase::Reference eof(new EOFMessage());
    messageQueue.put(eof, true);
}

void MessageSender::send(MessageBase::Reference msg)
{
    messageQueue.put(msg);
}
