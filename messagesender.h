#ifndef MESSAGESENDER_H
#define MESSAGESENDER_H

#include "threadloop.h"
#include "messagequeue.h"

class MessageSender : public Threadloop
{
    MessageQueue messageQueue;
public:
    virtual void operator()();
    virtual void stop();
    void send(PMessageBase msg);
};

#endif // MESSAGESENDER_H
