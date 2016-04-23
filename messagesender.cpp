#include "messagesender.h"

void MessageSender::operator()()
{
    for(;;)
    {
        PMessageBase msg = messageQueue.get();
        if(msg->isEOF())
        {
            break;
        }
        msg->send();
    }
}

void MessageSender::stop()
{
    PMessageBase eof(new EOFMessage());
    messageQueue.put(eof);
    Threadloop::stop();
}

void MessageSender::send(PMessageBase msg)
{
    messageQueue.put(msg);
}
