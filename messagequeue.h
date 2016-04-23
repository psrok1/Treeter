#ifndef MESSAGEQUEUE_H
#define MESSAGEQUEUE_H

#include "message.h"
#include <queue>
#include <condition_variable>
#include <mutex>

class MessageQueue
{
    // Queue of messages to send
    std::queue<PMessageBase> messageQueue;
    // Synchronization objects
    std::condition_variable cond;
    std::mutex mu;
public:
    PMessageBase get();
    void put(PMessageBase msg);
};

#endif // MESSAGEQUEUE_H
