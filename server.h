#ifndef SERVER_H
#define SERVER_H

#include <thread>
#include <vector>
#include <utility>
#include "messagesender.h"
#include "connection.h"
#include "threadloop.h"

class Server : public Threadloop
{
    MessageSender* instSender;
    std::thread *thSender;

    std::vector<std::pair<Connection*, std::thread*>> connections;
public:
    void createConnection(/*some args...*/);
    void deleteConnection(Connection* conn);

    MessageSender& getSender() const;

    virtual void operator()();
    virtual void stop();
};

#endif // SERVER_H
