#include "server.h"

#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <system_error>
#include <iostream>

#include <unistd.h>

Server::Server(unsigned short portNr): stopped(false), usedPort(portNr)
{
    // Creating shutdown pipe
    if(pipe(this->shutdownPipe)==-1)
        throw std::system_error(std::error_code(errno, std::system_category()),"server pipe() failed");

    // Creating socket
    if((socketDescriptor = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK,0))==-1)
        throw std::system_error(std::error_code(errno, std::system_category()),"socket() failed");
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(usedPort);

    if(bind(socketDescriptor,(struct sockaddr*) &addr,sizeof(addr))==-1)
    {
        close(socketDescriptor);
        throw std::system_error(std::error_code(errno, std::system_category()),"bind() failed");
    }

    if(listen(socketDescriptor,8)==-1)
    {
        close(socketDescriptor);
        throw std::system_error(std::error_code(errno, std::system_category()),"listen() failed");
    }
}

Server::~Server()
{
    close(this->shutdownPipe[0]);
    close(this->shutdownPipe[1]);
    close(socketDescriptor);
}


void Server::operator()(Reference)
{
    std::cout << "Hello world! o/ \n\n";

    //Set descriptors up
    fd_set descriptors;
    int maxdesc = socketDescriptor>shutdownPipe[0]? socketDescriptor+1: shutdownPipe[0]+1;

    std::cout << "Importing data from database...\n";
    this->model.importFromDatabase();

    std::cout << "Listening on port "<<usedPort<<"...\n";

    // ---- SERVER LOOP
    try
    {
        while(!stopped)
        {
            //Re-set descriptors
            FD_ZERO(&descriptors);
            FD_SET(socketDescriptor,&descriptors);
            FD_SET(shutdownPipe[0], &descriptors);

            // Wait for pending connection or shutdown signal
            if(select(maxdesc, &descriptors, nullptr, nullptr,nullptr)==-1)
            {
                throw std::system_error(std::error_code(errno, std::system_category()),"server select() failed");
            }

            if(FD_ISSET(shutdownPipe[0],&descriptors))
            {
                // If it was shutdown: break server loop
                break;
            }

            if(FD_ISSET(socketDescriptor,&descriptors))
            {
                // If it was socket: create new connection instance
                createConnection();
            }
        }
    }
    catch(std::system_error e)
    {
        std::cerr<<"Server thread exception, terminating: "<<e.what()<<" errno="<<e.code()<<"\n";
    }
    catch (std::exception e)
    {
        std::cerr<<"Connection thread critical exception, terminating: "<<e.what()<<"\n";
    }

    // .... END OF SERVER LOOP

    shutdown(socketDescriptor, SHUT_RD);

    // Close all connections
    connectionList.stopAll();
    connectionList.waitUntilEmpty();

    std::cout << "Server stopped...\n";
}

void Server::stopThread() {
    stopped = true;
    write(this->shutdownPipe[1], "goodbye", sizeof("goodbye"));
}

void Server::createConnection()
{
    try
    {
        Connection::Reference connection(new Connection(this, socketDescriptor, &this->model));
        connectionList.insert(connection);
        connection->createThread(connection);
        connection->detachThread();
    }
    catch(std::system_error e)
    {
        std::cout<<"Failed to create new connection: "<<e.what()<<" errno="<<e.code();
    }
}

void Server::deleteConnection(Connection::Reference connection)
{
    connectionList.remove(connection);
}
