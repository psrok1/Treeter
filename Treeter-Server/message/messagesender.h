#ifndef MESSAGESENDER_H
#define MESSAGESENDER_H

#include "../threadloop.h"
#include "messagequeue.h"

class Connection;

class MessageSender : public Threadloop<MessageSender>
{
    Connection* connection;
    MessageQueue messageQueue;
public:
    MessageSender(Connection* connection): connection(connection) { }

    virtual void stopThread();
    void send(MessageOutgoing::Reference msg);

    virtual void operator()(Reference refMessageSender);
};

#endif // MESSAGESENDER_H
