#include "messagequeue.h"

MessageBase::Reference MessageQueue::get()
{
    std::unique_lock<std::mutex> lck(mu);
    cond.wait(lck, [this]{ return !messageQueue.empty(); });

    MessageBase::Reference msg = messageQueue.front();
    messageQueue.pop();

    return msg;
}

void MessageQueue::put(MessageBase::Reference msg)
{
    std::unique_lock<std::mutex> lck(mu);

    messageQueue.push(msg);
    cond.notify_all();
}
