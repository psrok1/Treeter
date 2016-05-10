#include "connectionlist.h"

void ConnectionList::insert(Connection::Reference conn)
{
    std::unique_lock<std::mutex> lck(mu);
    this->connections.push_back(conn);
}

void ConnectionList::remove(Connection::Reference conn)
{
    std::unique_lock<std::mutex> lck(mu);
    auto it = this->connections.begin();

    for(; it != this->connections.end() && **it != *conn;
        ++it);

    if(it == this->connections.end())
        return;

    // Delete it from list
    this->connections.erase(it);
    if(this->connections.empty())
        this->all_stopped.notify_all();
}

void ConnectionList::stopAll()
{
    std::unique_lock<std::mutex> lck(mu);
    for(auto& ptr_conn: this->connections)
        ptr_conn->stopThread();
}

void ConnectionList::waitUntilEmpty()
{
    std::unique_lock<std::mutex> lck(mu);
    this->all_stopped.wait(lck, [this]{ return connections.empty(); });
}

