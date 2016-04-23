#include "messagequeue.h"

PMessageBase MessageQueue::get()
{
    std::unique_lock<std::mutex> lck(mu);
    cond.wait(lck, [this]{ return !messageQueue.empty(); });

    PMessageBase msg = messageQueue.front();
    messageQueue.pop();

    return msg;
}

void MessageQueue::put(PMessageBase msg)
{
    std::unique_lock<std::mutex> lck(mu);

    messageQueue.push(msg);
    cond.notify_all();
}
