#ifndef CONNECTION_H
#define CONNECTION_H
 
#include <memory>
#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "threadloop.h"
#include "crypto/crypto.h"
 
class Server;
class MessageProcessor;
 
class Connection : public Threadloop<Connection>
{
    static unsigned NEXT_ID;
    unsigned id;
 
    Server* server;
 
    friend class MessageProcessor;
 
    int socketDescriptor;
    unsigned int ipAddress;

    //select-related stuff
    int maxdesc;
    fd_set descriptors;

    std::atomic<bool> stopped;
    int shutdownPipe[2];
 
    bool readMsgLength(int length);
    bool readFromSocket(char* buffer, unsigned int length);
 
    Crypto::RSAContext rsaContext;
    Crypto::AESContext aesContext;
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
