#include "connection.h"
#include "message/messageprocessor.h"
#include "message/messageincoming.h"
#include "message/messageoutgoing.h"
#include "server.h"

#include <cstdlib>
#include <chrono>
#include <cstring>
#include <unistd.h>

unsigned Connection::NEXT_ID = 0;

Connection::Connection(Server* srv, int socket): id(Connection::NEXT_ID++), server(srv), stopped(false)
{
    if(pipe(this->shutdownPipe)==-1)
        throw std::system_error(std::error_code(errno, std::system_category()),"connection pipe() failed");
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
            return false;
        }

        if(FD_ISSET(socketDescriptor,&descriptors))
        {
            recv_bytes += recv(socketDescriptor,(buffer+recv_bytes),(length-recv_bytes),0);

            if(recv_bytes==length)
                return true;

            if(recv_bytes <= 0)
            {
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

                auto msgObject = MessageIncoming::fromString(msg);
                delete[] buffer;
                msgObject->process(processor);
        }
    }
    catch (std::system_error e)
    {
        std::cout<<"Connection thread exception, terminating: "<<e.what()<<" errno="<<e.code();
    }

    shutdown(socketDescriptor, SHUT_RD);

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

void Connection::sendMessage(std::string msg)
{
    std::cout<<this->id<<"> "<<msg<<"\n";

    if(aesContext.isValid())
        msg = aesContext.encrypt(msg);

    unsigned int bytesToSend = msg.size()+4;
    unsigned int payloadSize = bytesToSend-4;
    unsigned int invertedSize = htonl(payloadSize);
    char* bufferptr = new char[bytesToSend];
    memcpy(bufferptr,&invertedSize,4);
    memcpy(bufferptr+4,msg.data(),payloadSize);
    fd_set writeDescriptors;

    while(bytesToSend>0)
    {
        FD_ZERO(&writeDescriptors);
        FD_SET(socketDescriptor,&writeDescriptors);
        if(select(socketDescriptor+1,nullptr,&writeDescriptors,nullptr,nullptr)==-1)
        {
            this->stopThread();
            break;
        }

        int bytesSent = send(socketDescriptor,bufferptr,msg.size(),0);
        if(bytesSent<0)
        {
            this->stopThread();
            break;
        }
        bufferptr+=bytesSent;
        bytesToSend -= bytesSent;
    }
    delete[] bufferptr;
}

bool Connection::operator==(const Connection& comp_to)
{
    return this->id == comp_to.id;
}
