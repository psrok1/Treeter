#include "connection.h"
#include "message/messageprocessor.h"
#include "message/messageincoming.h"
#include "message/messageoutgoing.h"
#include "message/messagesender.h"
#include "server.h"

#include <cstdlib>
#include <chrono>
#include <cstring>
#include <unistd.h>

unsigned Connection::NEXT_ID = 0;

Connection::Connection(Server* srv, int socket, Model::DataModel* model): id(Connection::NEXT_ID++), server(srv), stopped(false), model(model)
{
    // Creating shutdown pipe
    if(pipe(this->shutdownPipe)==-1)
        throw std::system_error(std::error_code(errno, std::system_category()),"connection pipe() failed");

    // Accepting connection on listening socket
    sockaddr_in addr;
    unsigned int length;
    if((socketDescriptor = accept(socket, (struct sockaddr*) &addr, &length))==-1)
            throw std::system_error(std::error_code(errno, std::system_category()),"accept() failed");
    ipAddress = ntohl(addr.sin_addr.s_addr);
}
Connection::~Connection()
{
    close(this->shutdownPipe[0]);
    close(this->shutdownPipe[1]);
    close(socketDescriptor);
}

bool Connection::readFromSocket(char* buffer, unsigned int length)
{
    unsigned int recv_bytes=0;
    while(true)
    {
        //Re-set descriptors
        FD_ZERO(&descriptors);
        FD_SET(socketDescriptor,&descriptors);
        FD_SET(shutdownPipe[0], &descriptors);

        if(select(maxdesc, &descriptors, nullptr, nullptr,nullptr)==-1)
        {
            throw std::system_error(std::error_code(errno, std::system_category()),"connection select() failed");
        }

        if(FD_ISSET(shutdownPipe[0],&descriptors))
        {
            // If shutdown: signal that message completing was interrupted
            return false;
        }

        if(FD_ISSET(socketDescriptor,&descriptors))
        {
            recv_bytes += recv(socketDescriptor,(buffer+recv_bytes),(length-recv_bytes),0);

            // Check whether whole message was received
            if(recv_bytes==length)
                return true;

            if(recv_bytes <= 0)
            {
                // If recv returns no data: connection is closed
                std::cout << "Peer closed connection.\n";
                return false;
            }
        }
    }
}

void Connection::operator()(Reference refConnection)
{
    MessageProcessor processor(refConnection);
    std::cout << "Connection " << this->id << " started...\n";

    // Launch message sender instance
    messageSender = MessageSender::Reference(new MessageSender(this));
    messageSender->createThread(messageSender);

    //Set max. descriptor value
    maxdesc = socketDescriptor>shutdownPipe[0]? socketDescriptor+1: shutdownPipe[0]+1;

    unsigned int length;
    char* buffer;

    try
    {
        while(!stopped)
        {
                //Read payload length - false forces us to terminate the connection
                if(!readFromSocket(reinterpret_cast<char*>(&length),4))
                    break;
                length = ntohl(length);

                buffer = new char[length];
                //Read payload
                if(!readFromSocket(buffer,length))
                {
                    delete[] buffer;
                    break;
                }

                std::string msg(buffer,length);

                if(aesContext.isValid())
                    msg = aesContext.decrypt(msg);
                std::cout << this->id << "> " << msg << "\n";

                auto msgObject = MessageIncoming::fromString(msg);
                delete[] buffer;
                msgObject->process(processor);
        }
    }
    catch (std::system_error e)
    {
        std::cerr<<"Connection thread exception, terminating: "<<e.what()<<" errno="<<e.code()<<"\n";
    }
    catch (std::exception e)
    {
        std::cerr<<"Connection thread critical exception, terminating: "<<e.what()<<"\n";
    }

    shutdown(socketDescriptor, SHUT_RD);

    // If associated user: make a logout
    if(this->user)
    {
        this->user->unregisterConnection();
        this->user.reset();
    }

    // Close sender instance
    messageSender->stopThread();
    messageSender->joinThread();

    std::cout << "Connection " << this->id << " finished...\n";
    this->server->deleteConnection(refConnection);
}

void Connection::stopThread()
{
    // Tutaj powinnismy moim zdaniem rowniez przeprowadzic odlaczanie referencji z modelu
    // Dzieki temu, ze przeprowadzi to watek odlaczajacy: mamy gwarancje, ze polaczenie zostanie oswobodzone
    // dokladnie w chwili, gdy zadajacy tego chce
    // Potem nasze polaczenie mozna juz zostawic same sobie.. niech sie na spokojnie zamknie, nikogo juz nie bedzie obchodzic

    stopped = true;
    write(this->shutdownPipe[1], "goodbye", sizeof("goodbye"));
}

void Connection::sendString(std::string msg)
{
    char* msgbuffer = nullptr;
    std::cout<<this->id<<"< "<<msg<<"\n";

    try
    {
        // Encrypt message, when there is valid AES key
        if(aesContext.isValid())
            msg = aesContext.encrypt(msg);

        unsigned int payloadSize = msg.size();
        unsigned int bytesToSend = payloadSize + 4;
        unsigned int invertedSize = htonl(payloadSize);

        msgbuffer = new char[bytesToSend];

        // Append message length to buffer
        memcpy(msgbuffer,&invertedSize,4);
        memcpy(msgbuffer+4,msg.data(),payloadSize);

        char* bufferptr = msgbuffer;
        fd_set writeDescriptors;

        // Write completion loop
        while(bytesToSend>0)
        {
            FD_ZERO(&writeDescriptors);
            FD_SET(socketDescriptor,&writeDescriptors);
            if(select(socketDescriptor+1,nullptr,&writeDescriptors,nullptr,nullptr)==-1)
            {
                throw std::system_error(std::error_code(errno, std::system_category()),"connection select() failed");
            }

            int bytesSent = ::send(socketDescriptor,bufferptr,bytesToSend,0);
            if(bytesSent<0)
            {
                // Socket may be closed.
                this->stopThread();
                break;
            }
            bufferptr += bytesSent;
            bytesToSend -= bytesSent;
        }
    } catch(std::system_error e)
    {
        std::cout<<"Exception during send operation, terminating: "<<e.what()<<" errno="<<e.code();
        this->stopThread();
    }
    delete[] msgbuffer;
}

bool Connection::operator==(const Connection& comp_to)
{
    // Connection objects are the same, when both have the same ID
    return this->id == comp_to.id;
}

void Connection::sendMessage(MessageOutgoing::Reference message)
{
    this->messageSender->send(message);
}
