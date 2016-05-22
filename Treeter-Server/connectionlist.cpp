#include "connectionlist.h"

void ConnectionList::insert(Connection::Reference conn)
{
    std::unique_lock<std::mutex> lck(mu);
    this->connections.push_back(conn);
}

void ConnectionList::remove(Connection::Reference conn)
{
    std::unique_lock<std::mutex> lck(mu);

    // Find connection on list
    auto it = this->connections.begin();

    for(; it != this->connections.end() && !(**it == *conn);
        ++it);

    if(it == this->connections.end())
        return;

    // Erase it
    this->connections.erase(it);
    // No more active connections? Probably we should notify server in case of shutdown
    if(this->connections.empty())
        this->all_stopped.notify_all();
}

void ConnectionList::sendToAll(MessageOutgoing::Reference message)
{
    std::unique_lock<std::mutex> lck(mu);

    for(auto& ptr_conn: this->connections)
    {
        MessageOutgoing::Reference pointed_msg = message->clone();
        pointed_msg->setConnection(ptr_conn);
        ptr_conn->sendMessage(pointed_msg);
    }
}

void ConnectionList::stopAll()
{
    std::unique_lock<std::mutex> lck(mu);
    // Request to stop all active connections
    for(auto& ptr_conn: this->connections)
        ptr_conn->stopThread();
}

void ConnectionList::waitUntilEmpty()
{
    std::unique_lock<std::mutex> lck(mu);
    this->all_stopped.wait(lck, [this]{ return connections.empty(); });
}

