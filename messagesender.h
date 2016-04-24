#ifndef MESSAGESENDER_H
#define MESSAGESENDER_H

#include "threadloop.h"
#include "messagequeue.h"
#include <iostream>

class MessageSender : public Threadloop<MessageSender>
{
    MessageQueue messageQueue;
public:
    MessageSender() {
        std::cout<<"MessageSender()\n";
    }
    ~MessageSender() {
        std::cout<<"~MessageSender()\n";
    }
    virtual void stopThread();
    void send(MessageBase::Reference msg);

    virtual void operator()(Reference refMessageSender);
};

#endif // MESSAGESENDER_H
