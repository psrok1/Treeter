#include "messagequeue.h"

MessageBase::Reference MessageQueue::get()
{
    std::unique_lock<std::mutex> lck(mu);
    cond.wait(lck, [this]{ return !messageQueue.empty(); });

    MessageBase::Reference msg = messageQueue.front();
    messageQueue.pop_front();

    return msg;
}

void MessageQueue::put(MessageBase::Reference msg, bool oob)
{
    std::unique_lock<std::mutex> lck(mu);

    if(!oob)
        messageQueue.push_back(msg);
    else
        messageQueue.push_front(msg);

    cond.notify_all();
}
