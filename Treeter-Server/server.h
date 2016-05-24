#ifndef SERVER_H
#define SERVER_H

#include <mutex>
#include <thread>
#include <list>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "message/messagesender.h"
#include "connection.h"
#include "connectionlist.h"
#include "threadloop.h"

class Server : public Threadloop<Server>
{
    ConnectionList connectionList;
    std::atomic<bool> stopped;
    int shutdownPipe[2];

    unsigned short usedPort;
    int socketDescriptor;
public:
    Server(unsigned short portNr);
    ~Server();

    void createConnection();
    void deleteConnection(Connection::Reference connection);

    virtual void stopThread();

    virtual void operator()(Reference refServer);
};

#endif // SERVER_H
