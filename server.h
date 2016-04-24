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
    std::atomic<bool> stop;
public:
    Server(): stop(false) {
        std::cout<<"Server()\n";
    }
    ~Server() {
        std::cout<<"~Server()\n";
    }
    void createConnection();
    void deleteConnection(Connection::Reference connection);

    MessageSender::Reference getSender() const;
    virtual void stopThread();

    virtual void operator()(Reference refServer);
};

#endif // SERVER_H
