#include "messagesender.h"

void MessageSender::operator()(Reference)
{
    for(;;)
    {
        MessageBase::Reference msg = messageQueue.get();
        if(msg->isEOF())
        {
            break;
        }
        msg->send();
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
