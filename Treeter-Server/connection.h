#ifndef CONNECTION_H
#define CONNECTION_H

#include <memory>
#include <string>
#include "threadloop.h"
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

class Server;

class Connection : public Threadloop<Connection>
{
    static unsigned NEXT_ID;
    unsigned id;

    Server* server;
    int socketDescriptor;
    unsigned int ipAddress;
    //select-related stuff
    int maxdesc;
    fd_set descriptors;
    // TODO: cipher key
    std::atomic<bool> stopped;
    int shutdownPipe[2];

    bool readMsgLength(int length);
    bool readFromSocket(char* buffer, int length);

public:
    Connection(Server* srv, int socket);
    ~Connection();

    virtual void stopThread();

    void sendMessage(std::string msg);

    // Is "comp_to" the same as "this"?
    bool operator==(const Connection& comp_to);
    virtual void operator()(Reference refConnection);
};

#endif // CONNECTION_H
