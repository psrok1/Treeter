#include "server.h"
#include <stdexcept>

void Server::createConnection(/*some args...*/)
{
    Connection* connection = new Connection(this);
    std::thread* worker = new std::thread(*connection);

    this->connections.push_back(std::make_pair(connection, worker));
}

void Server::deleteConnection(Connection* conn)
{
    auto conn_it = this->connections.begin();
    for(; conn_it != this->connections.end(); ++conn_it)
    {
        if((*conn_it).first == conn)
            break;
    }

    if(conn_it == this->connections.end())
        throw std::runtime_error("Attempt to delete connection which doesn't exist");

    auto* worker = (*conn_it).second;

    conn->stop();
    worker->join();
    delete worker;
    delete conn;

    this->connections.erase(conn_it);
}

MessageSender& Server::getSender() const
{
    return *(this->instSender);
}

void Server::operator()()
{
    // Launch message sender instance
    instSender = new MessageSender();
    thSender   = new std::thread(*instServer);

    // ---- MAIN SERVER LOOP
    for(;;)
    {
    }
    // .... END OF SERVER LOOP

    // Close all connections
    for(auto& c: this->connections) {
        Connection* connection = c.first;
        std::thread* worker = c.second;

        connection->stop();
        worker->join();
        delete worker;
        delete connection;
    }

    // Close sender instance
    instSender->stop();
    thSender->join();
    delete thServer;
    delete instServer;
}

void Server::stop() {
    // Listening socket shutdown

    Threadloop::stop();
}
