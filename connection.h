#ifndef CONNECTION_H
#define CONNECTION_H

#include <memory>
#include <string>
#include "threadloop.h"

class Server;

class Connection : public Threadloop
{
    static unsigned NEXT_ID;
    unsigned id;
    Server* server;
    // TODO: socket descriptor
    // TODO: cipher key
public:
    Connection(Server* srv): id(Connection::NEXT_ID++), server(srv) {}

    virtual void operator()();
    virtual void stop();

    void sendMessage(std::string msg);

    // Is "comp_to" the same as "this"?
    bool operator==(const Connection& comp_to);
};

typedef std::shared_ptr<Connection> PConnection;

#endif // CONNECTION_H
