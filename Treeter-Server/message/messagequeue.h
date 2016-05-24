#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include "messageoutgoing.h"
#include <list>
#include <condition_variable>
#include <mutex>

class MessageQueue
{
    // Queue of messages to send
    std::list<MessageOutgoing::Reference> messageQueue;
    // Synchronization objects
    std::condition_variable cond;
    std::mutex mu;
public:
    MessageOutgoing::Reference get();
    void put(MessageOutgoing::Reference msg, bool oob=false);
};

#endif // MESSAGEQUEUE_H
