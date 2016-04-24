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
           Mozna stworzyc w przyszlosci odpowiedni wizytator.
     **/
    }

    // TODO: Finalizacja.

    // Wyrejestrowanie polaczenia zawsze na koncu
    // Jesli watek wyrejestrowuje sie sam: po tej instrukcji "this" bedzie uniewaznione
    // Jesli watek jest wyrejestrowywany przez serwer - serwer na poczatek stworzy kopie smart-pointerow, by wiedziec kiedy
    // machina sie zatrzyma. My jednak zakladamy pierwszy scenariusz
    server->deleteConnection(*this);
}

void Connection::stop()
{
    /**
     * Zadanie zatrzymania polaczenia: np. shutdown na sockecie
     * Ta metoda moze byc wykonywana z roznych watkow, pamietac o atomowosci
     */

    // Tutaj powinnismy moim zdaniem rowniez przeprowadzic odlaczanie referencji z modelu
    // Dzieki temu, ze przeprowadzi to watek odlaczajacy: mamy gwarancje, ze polaczenie zostanie oswobodzone
    // dokladnie w chwili, gdy zadajacy tego chce
    // Potem nasze polaczenie mozna juz zostawic same sobie.. niech sie na spokojnie zamknie, nikogo juz nie bedzie obchodzic

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
