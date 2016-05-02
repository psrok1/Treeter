#ifndef SERVER_H
#define SERVER_H

#include <mutex>
#include <thread>
#include <list>
#include "messagesender.h"
#include "connection.h"
#include "threadloop.h"
#include <iostream>

class Server : public Threadloop<Server>
{
    MessageSender::Reference messageSender;

    typedef std::list<Connection::Reference> ConnectionList;

    ConnectionList connectionList;
    std::mutex connectionListLock;
    std::atomic<bool> stopped;
    int shutdownPipe[2];
public:
    Server();
    ~Server();

    void createConnection();
    void deleteConnection(Connection::Reference connection);

    MessageSender::Reference getSender() const;
    virtual void stopThread();

    virtual void operator()(Reference refServer);
};

#endif // SERVER_H
