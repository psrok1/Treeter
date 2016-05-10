#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include "messagebase.h"
#include <list>
#include <condition_variable>
#include <mutex>
#include <iostream>

class MessageQueue
{
    // Queue of messages to send
    std::list<MessageBase::Reference> messageQueue;
    // Synchronization objects
    std::condition_variable cond;
    std::mutex mu;
public:
    MessageBase::Reference get();
    void put(MessageBase::Reference msg, bool oob=false);
};

#endif // MESSAGEQUEUE_H
