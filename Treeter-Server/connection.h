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
#include "message/messagesender.h"
#include "message/messageoutgoing.h"
 
class Server;
class MessageProcessor;
 
class Connection : public Threadloop<Connection>
{
    // Unique ID generation
    static unsigned NEXT_ID;
    unsigned id;
 
    // Pointer to server instance
    Server* server;

    // Message sender instance
    MessageSender::Reference messageSender;
 
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

    void sendString(std::string msg);
public:
    Connection(Server* srv, int socket);
    ~Connection();
 
    virtual void stopThread();

    void sendMessage(MessageOutgoing::Reference message);

    // Is "comp_to" the same as "this"?
    bool operator==(const Connection& comp_to);
    virtual void operator()(Reference refConnection);
};
 
#endif // CONNECTION_H
