#ifndef CONNECTION_H
#define CONNECTION_H

#include <memory>
#include <string>
#include "threadloop.h"
#include <iostream>

class Server;

class Connection : public Threadloop<Connection>
{
    static unsigned NEXT_ID;
    unsigned id;

    Server* server;
    // TODO: socket descriptor
    // TODO: cipher key
    std::atomic<bool> stopped;
    int shutdownPipe[2];
public:
    Connection(Server* srv);
    ~Connection();

    virtual void stopThread();

    void sendMessage(std::string msg);

    // Is "comp_to" the same as "this"?
    bool operator==(const Connection& comp_to);
    virtual void operator()(Reference refConnection);
};

#endif // CONNECTION_H
