#include "server.h"
#include <stdexcept>

void Server::operator()()
{
    // Launch message sender instance
    instSender = new MessageSender();
    thSender   = new std::thread(std::ref(*instSender));

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
    for(auto& c: this->connections) {
        PConnection connection = c.first;
        std::thread* worker = c.second;

        connection->stop();
        worker->join();
        delete worker;
    }

    // Close sender instance
    instSender->stop();
    thSender->join();
    delete thSender;
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
    std::thread* worker = new std::thread(std::ref(*connection));

    this->connections.push_back(std::make_pair(connection, worker));
}

void Server::deleteConnection(Connection& conn)
{
    auto conn_it = this->connections.begin();
    for(; conn_it != this->connections.end(); ++conn_it)
    {
        if(*(*conn_it).first == conn)
            break;
    }

    if(conn_it == this->connections.end())
        throw std::runtime_error("Attempt to delete connection which doesn't exist");

    auto* worker = (*conn_it).second;

    conn.stop();
    worker->join();
    delete worker;

    this->connections.erase(conn_it);
}

MessageSender& Server::getSender() const
{
    return *(this->instSender);
}
