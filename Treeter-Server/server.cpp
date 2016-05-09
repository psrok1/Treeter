#include "server.h"
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <cstdlib>
#include <cstring>
#include <unistd.h>
#include <system_error>


Server::Server(unsigned short portNr): stopped(false), usedPort(portNr)
{
    if(pipe(this->shutdownPipe)==-1)
        throw std::system_error(std::error_code(errno, std::system_category()),"server pipe() failed");

    //Tworzenie socketa
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
    try
    {
        while(!stopped)
        {
            //Re-set descriptors
            FD_ZERO(&descriptors);
            FD_SET(socketDescriptor,&descriptors);
            FD_SET(shutdownPipe[0], &descriptors);

            if(select(maxdesc, &descriptors, nullptr, nullptr,nullptr)==-1)
            {
                throw std::system_error(std::error_code(errno, std::system_category()),"server select() failed");
            }

            if(FD_ISSET(shutdownPipe[0],&descriptors))
            {
                break;
            }
            if(FD_ISSET(socketDescriptor,&descriptors))
            {
                createConnection();
            }

            /**
              Tutaj nasluchujemy nowych polaczen
              Nowe obiekty polaczen rejestrujemy przy pomocy metody createConnection.
              Powoluje ona automatycznie obiekt sesji i watek obslugujacy ta aktywna sesje
              Gdy socket nasluchujacy okaze sie zerwany: wyskok z petli przez break
             **/
        }
    }
    catch(std::system_error e)
    {
        std::cout<<"Server thread exception, terminating: "<<e.what()<<" errno="<<e.code();
    }

    // .... END OF SERVER LOOP

    shutdown(socketDescriptor, SHUT_RD);
    /**
       Tutaj mozna przeprowadzic potrzebne finalizacje
     **/

    // Close all connections
    {
        ConnectionList connections_copy;
        {
            std::unique_lock<std::mutex> lck(this->connectionListLock);

            connections_copy = this->connectionList;

            this->connectionList.clear();
        }

        for(auto& c: connections_copy)
        {
            c->stopThread();
            c->joinThread();
        }
    }

    // Close sender instance
    messageSender->stopThread();
    messageSender->joinThread();
    std::cout << "Server stopped...\n";
}

void Server::stopThread() {
    /**
     * Tutaj wyzwalamy zamkniecie socketa nasluchujacego.
     * Musimy sprawic, by watek wyskoczyl z petli nasluchujacej.
     * Stop moze byc wyzwalany z poziomu roznych watkow: pamietac o atomowosci.
     * Zadnych mutexow, ma byc nieblokujacy.
     */
    stopped = true;
    write(this->shutdownPipe[1], "goodbye", sizeof("goodbye"));
}

void Server::createConnection(/*Some args..*/)
{
    /**
     * Prawdopodobnie bedziesz chcial rozszerzyc Connection o dodatkowe argumenty (np. socket)
     * Mozesz ten argument przekazac w linijce nizej:
     *  (wymaga oczywiscie zmian w konstruktorze)
     */
    try
    {
        Connection::Reference connection(new Connection(this, socketDescriptor));
        {
            std::unique_lock<std::mutex> lck(this->connectionListLock);
            this->connectionList.push_back(connection);
        }
        connection->createThread(connection);
    }
    catch(std::system_error e)
    {
        std::cout<<"Failed to create new connection: "<<e.what()<<" errno="<<e.code();
    }
}

void Server::deleteConnection(Connection::Reference connection)
{
    {
        std::unique_lock<std::mutex> lck(this->connectionListLock);
        Server::ConnectionList::iterator it;

        for(it = this->connectionList.begin();
            it != this->connectionList.end();
            ++it)
        {
            if(**it == *connection)
                break;
        }

        if(it == this->connectionList.end())
        {
            // Connection has been deleted by another thread! (Probably server thread during close)
            // No problem!
            return;
        }

        // Delete it from list
        (*it)->detachThread();
        this->connectionList.erase(it);
    }
    // Now, just stop it!
    connection->stopThread();
}

MessageSender::Reference Server::getSender() const {
    return this->messageSender;
}
