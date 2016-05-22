#ifndef CONNECTION_H
#define CONNECTION_H
 
#include <memory>
#include <string>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include "threadloop.h"
#include "crypto/crypto.h"

#include "model/user.h"
#include "message/messagebase.h"
 
class Server;
class MessageProcessor;
class MessageSender;
 
class Connection : public Threadloop<Connection>
{
    // Unique ID generation
    static unsigned NEXT_ID;
    unsigned id;
 
    // Pointer to server instance
    Server* server;
 
    // MessageProcessor is an integral part of Connection object
    friend class MessageProcessor;
    // MessageSender will be an integral part...
    friend class MessageSender;
 
    // Connection socket and IP address
    int socketDescriptor;
    unsigned int ipAddress;

    // Select-related stuff
    int maxdesc;
    fd_set descriptors;

    std::atomic<bool> stopped;
    int shutdownPipe[2];
 
    bool readMsgLength(int length);
    bool readFromSocket(char* buffer, unsigned int length);
 
    // Crypto key wrappers
    Crypto::RSAContext rsaContext;
    Crypto::AESContext aesContext;

    // Logged user
    std::shared_ptr<Model::User> user;
public:
    Connection(Server* srv, int socket);
    ~Connection();
 
    virtual void stopThread();

    void sendMessage(MessageBase::Reference message);
    void sendString(std::string msg);
 
    // Is "comp_to" the same as "this"?
    bool operator==(const Connection& comp_to);
    virtual void operator()(Reference refConnection);
};
 
#endif // CONNECTION_H
