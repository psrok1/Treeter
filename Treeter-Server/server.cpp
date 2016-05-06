#include "server.h"
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <unistd.h>


Server::Server(unsigned short portNr): stopped(false), usedPort(portNr)
{
    pipe(this->shutdownPipe);

    //Tworzenie socketa
    socketDescriptor = socket(AF_INET, SOCK_STREAM | SOCK_NONBLOCK,0);
    sockaddr_in addr;
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = htonl(INADDR_ANY);
    addr.sin_port = htons(usedPort);
    bind(socketDescriptor,(struct sockaddr*) &addr,sizeof(addr));
    listen(socketDescriptor,8);
}

Server::~Server()
{
    close(this->shutdownPipe[0]);
    close(this->shutdownPipe[1]);
    close(socketDescriptor);
}


void Server::operator()(Reference)
{
    std::cout << "Server started...\n";
    // Launch message sender instance
    messageSender = MessageSender::Reference(new MessageSender());
    messageSender->createThread(messageSender);

    //Set descriptors up
    fd_set descriptors;
    int maxdesc = socketDescriptor>shutdownPipe[0]? socketDescriptor+1: shutdownPipe[0]+1;

    /**
      TODO:
      Glowna petla serwera
      Tutaj mozna wykonywac potrzebne inicjalizacje np. socketu nasluchujacego
     **/

    // ---- SERVER LOOP
    while(!stopped)
    {
        //Re-set descriptors
        FD_ZERO(&descriptors);
        FD_SET(socketDescriptor,&descriptors);
        FD_SET(shutdownPipe[0], &descriptors);

        select(maxdesc, &descriptors, nullptr, nullptr,nullptr);

        if(FD_ISSET(shutdownPipe[0],&descriptors))
        {
            break;
        }
        if(FD_ISSET(socketDescriptor,&descriptors))
        {
            createConnection();
        }

        //std::this_thread::sleep_for(std::chrono::milliseconds(((rand()%100)+1)*10));
        //createConnection();
        /**
          Tutaj nasluchujemy nowych polaczen
          Nowe obiekty polaczen rejestrujemy przy pomocy metody createConnection.
          Powoluje ona automatycznie obiekt sesji i watek obslugujacy ta aktywna sesje
          Gdy socket nasluchujacy okaze sie zerwany: wyskok z petli przez break
         **/
    }
    // .... END OF SERVER LOOP

    shutdown(socketDescriptor, SHUT_RD);
    /**
       Tutaj mozna przeprowadzic potrzebne finalizacje
     **/

    // Close all connections
    connectionList.stopAll();
    connectionList.waitUntilEmpty();

    // Close sender instance
    messageSender->stopThread();
    messageSender->joinThread();
    std::cout << "Server stopped...\n";
}

void Server::stopThread() {
    stopped = true;
    write(this->shutdownPipe[1], "goodbye", sizeof("goodbye"));
}

void Server::createConnection()
{
    Connection::Reference connection(new Connection(this, socketDescriptor));
    connectionList.insert(connection);
    connection->createThread(connection);
    connection->detachThread();
}

void Server::deleteConnection(Connection::Reference connection)
{
    connectionList.remove(connection);
}

MessageSender::Reference Server::getSender() const {
    return this->messageSender;
}
