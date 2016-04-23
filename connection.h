#ifndef CONNECTION_H
#define CONNECTION_H

#include "server.h"
#include "threadloop.h"

class Connection : public Threadloop
{
    Server* server;
public:
    Connection(Server*);

    virtual void operator()();
    virtual void stop();
};

#endif // CONNECTION_H
