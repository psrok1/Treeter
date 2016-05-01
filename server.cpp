#include "server.h"
#include <stdexcept>
#include <algorithm>
#include <chrono>
#include <cstdlib>

#include <unistd.h>


Server::Server(): stopped(false)
{
    pipe(this->shutdownPipe);
}

Server::~Server()
{
    close(this->shutdownPipe[0]);
    close(this->shutdownPipe[1]);
}


void Server::operator()(Reference)
{
    std::cout << "Server started...\n";
    // Launch message sender instance
    messageSender = MessageSender::Reference(new MessageSender());
    messageSender->createThread(messageSender);

    /**
      TODO:
      Glowna petla serwera
      Tutaj mozna wykonywac potrzebne inicjalizacje np. socketu nasluchujacego
     **/

    // ---- SERVER LOOP
    while(!stopped)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(((rand()%100)+1)*10));
        createConnection();
        /**
          Tutaj nasluchujemy nowych polaczen
          Nowe obiekty polaczen rejestrujemy przy pomocy metody createConnection.
          Powoluje ona automatycznie obiekt sesji i watek obslugujacy ta aktywna sesje
          Gdy socket nasluchujacy okaze sie zerwany: wyskok z petli przez break
         **/
    }
    // .... END OF SERVER LOOP

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
    Connection::Reference connection(new Connection(this));
    {
        std::unique_lock<std::mutex> lck(this->connectionListLock);
        this->connectionList.push_back(connection);
    }
    connection->createThread(connection);
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
