#include "messagesender.h"

void MessageSender::operator()(Reference)
{
    std::cout << "Sender started\n";
    for(;;)
    {
        MessageBase::Reference msg = messageQueue.get();
        if(msg->isEOF())
        {
            break;
        }
        msg->send();
    }
    std::cout << "Sender stopped\n";
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
