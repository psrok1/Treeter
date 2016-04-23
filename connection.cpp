#include "connection.h"
#include "server.h"

void Connection::operator()()
{
    // TODO: Inicjalizacja....

    for(;;)
    {
    /**
      TODO

      Ta funkcja realizuje glowna petle watku polaczenia.
      Powinna wykonywac:
        1. Odczyt wiadomosci
        2. Jesli koniec komunikacji: wyskoczyc z petli
        3. Deszyfrowanie
        4. Deserializacja wiadomosci (mozna powolac jakas fabryke)
        5. Wykonanie akcji zwiazanej z wiadomoscia
           Akcja powinna dokonac odpowiedzi przy pomocy MessageSender'a
        6. Koniec
     **/
    }

    // TODO: Finalizacja.

    // Na koncu zawsze wyrejestrowanie polaczenia
    server->deleteConnection(*this);
}

void Connection::stop()
{
    /**
     * Zadanie zatrzymania polaczenia: np. shutdown na sockecie
     * Ta metoda moze byc wykonywana z roznych watkow, pamietac o atomowosci
     * Bez synchronizacji mutexami, stop ma byc nieblokujace.
     * Jak sie nie da: zglosic na Issues
     */

    // Na koncu zawsze zmiana stanu Threadloop na "zatrzymany"
    Threadloop::stop();
}

void Connection::sendMessage(std::string msg)
{
    /**
     * Wyslanie wiadomosci do socketa
     * Tutaj mozna umiescic rowniez szyfrowanie
     * Jesli robienie send na ubitych socketach przeszkadza, zrobic jakas kontrole.
     * Czasem watek sendera moze chciec wysylac jakies wiadomosci, gdy polaczenie jest juz zamkniete
     **/
}

bool Connection::operator==(const Connection& comp_to) {
    return (this == &comp_to);
}
