#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include "message.h"
#include <queue>
#include <condition_variable>
#include <mutex>
#include <iostream>

class MessageQueue
{
    // Queue of messages to send
    std::queue<MessageBase::Reference> messageQueue;
    // Synchronization objects
    std::condition_variable cond;
    std::mutex mu;
public:
    MessageBase::Reference get();
    void put(MessageBase::Reference msg);
};

#endif // MESSAGEQUEUE_H
