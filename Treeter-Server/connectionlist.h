#ifndef CONNECTIONLIST_H
#define CONNECTIONLIST_H

#include <mutex>
#include <condition_variable>
#include <list>
#include "connection.h"
#include "message/messageoutgoing.h"

class ConnectionList
{
    std::list<Connection::Reference> connections;
    std::mutex mu;
    std::condition_variable all_stopped;
public:
    void insert(Connection::Reference conn);
    void remove(Connection::Reference conn);
    void sendToAll(MessageOutgoing::Reference message);
    void stopAll();
    void waitUntilEmpty();
};

#endif // CONNECTIONLIST_H
