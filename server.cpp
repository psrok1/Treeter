#include "server.h"
#include <stdexcept>
#include <algorithm>

void Server::operator()()
{
    // Launch message sender instance
    instSender = new MessageSender();
    instSender->start();

    /**
      TODO:
      Glowna petla serwera
      Tutaj mozna wykonywac potrzebne inicjalizacje np. socketu nasluchujacego
     **/

    // ---- SERVER LOOP
    for(;;)
    {
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
        // Dzieki kopii, destruktory wykonaja sie poza sekcja krytyczna.
        // Destruktory sa blokujace!
        ConnectionList connections_copy;
        {
            std::unique_lock<std::mutex> lck(this->lckConnList);

            connections_copy = this->connectionList;

            this->connectionList.empty();
        }
        // Stop niech bedzie wykonywane poza sekcja krytyczna
        for(auto& c: connections_copy)
            c->stop();
    } // Tutaj powinny wykonac sie destruktory - w tym miejscu ostatnim smart-pointerem powinien byc ten w kopii
    // Close sender instance
    delete instSender;
}

void Server::stop() {
    // Listening socket shutdown
    /**
     * Tutaj wyzwalamy zamkniecie socketa nasluchujacego.
     * Musimy sprawic, by watek wyskoczyl z petli nasluchujacej.
     * Stop moze byc wyzwalany z poziomu roznych watkow: pamietac o atomowosci.
     * Zadnych mutexow, ma byc nieblokujacy.
     */
    Threadloop::stop();
}

void Server::createConnection(/*Some args..*/)
{
    /**
     * Prawdopodobnie bedziesz chcial rozszerzyc Connection o dodatkowe argumenty (np. socket)
     * Mozesz ten argument przekazac w linijce nizej:
     *  (wymaga oczywiscie zmian w konstruktorze)
     */
    PConnection connection(new Connection(this));
    {
        std::unique_lock<std::mutex> lck(this->lckConnList);
        this->connectionList.push_back(connection);
    }
    connection->start();
}

void Server::deleteConnection(Connection& conn)
{
    PConnection connection;

    {
        std::unique_lock<std::mutex> lck(this->lckConnList);
        Server::ConnectionList::iterator it;

        for(auto it = this->connectionList.begin();
            it != this->connectionList.end();
            ++it)
        {
            if(**it == conn)
                break;
        }

        if(it == this->connectionList.end())
        {
            // Connection has been deleted by another thread! (Probably server thread during close)
            // No problem!
            return;
        }

        // Copy smart pointer to preserve connection existence
        connection = *it;

        // Delete it from list
        this->connectionList.erase(it);
    }
    // Now, just stop it!
    connection->stop();
    // Connection should be deleted during returning from this function
    // Deletion will be nonblocking in self-deletion case and blocking for caller otherwise
}

MessageSender& Server::getSender() const
{
    return *(this->instSender);
}
