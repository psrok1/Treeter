#ifndef SERVER_H
#define SERVER_H

#include <mutex>
#include <thread>
#include <list>
#include "messagesender.h"
#include "connection.h"
#include "threadloop.h"

class Server : public Threadloop
{
    MessageSender* instSender;

    typedef std::list<PConnection> ConnectionList;

    ConnectionList connectionList;
    std::mutex lckConnList;
public:
    void createConnection(/*some args...*/);
    void deleteConnection(Connection& conn);

    MessageSender& getSender() const;

    virtual void operator()();
    virtual void stop();
};

#endif // SERVER_H
