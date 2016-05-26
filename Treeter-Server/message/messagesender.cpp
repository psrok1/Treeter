#include "messagesender.h"
#include "connection.h"

void MessageSender::operator()(Reference)
{
    for(;;)
    {
        MessageOutgoing::Reference msg = messageQueue.get();
        if(msg->isEOF())
        {
            // If stop signal: break sender loop
            break;
        }
        this->connection->sendString(msg->toString());
    }
}

void MessageSender::stopThread()
{
    MessageOutgoing::Reference eof(new EOFMessage());
    messageQueue.put(eof, true);
}

void MessageSender::send(MessageOutgoing::Reference msg)
{
    messageQueue.put(msg);
}
