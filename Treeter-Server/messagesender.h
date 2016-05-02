#ifndef MESSAGESENDER_H
#define MESSAGESENDER_H

#include "threadloop.h"
#include "messagequeue.h"
#include <iostream>

class MessageSender : public Threadloop<MessageSender>
{
    MessageQueue messageQueue;
public:
    virtual void stopThread();
    void send(MessageBase::Reference msg);

    virtual void operator()(Reference refMessageSender);
};

#endif // MESSAGESENDER_H
