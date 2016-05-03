#include "connection.h"
#include "server.h"
#include <cstdlib>
#include <chrono>
#include <cstring>
#include <unistd.h>

unsigned Connection::NEXT_ID = 0;

Connection::Connection(Server* srv, int socket): id(Connection::NEXT_ID++), server(srv), stopped(false)
{
    pipe(this->shutdownPipe);
    sockaddr_in addr;
    unsigned int length;
    socketDescriptor = accept(socket,(struct sockaddr*) &addr, &length);
    ipAddress = ntohl(addr.sin_addr.s_addr);
}

Connection::~Connection()
{
    close(this->shutdownPipe[0]);
    close(this->shutdownPipe[1]);
    close(socketDescriptor);
}

void Connection::operator()(Reference refConnection)
{
    MessageSender::Reference sender = this->server->getSender();
    std::cout << "Connection " << this->id << " started...\n";
    // TODO: Inicjalizacja....

    //Set descriptors up
    fd_set descriptors;
    int maxdesc = socketDescriptor>shutdownPipe[0]? socketDescriptor+1: shutdownPipe[0]+1;
    //Create buffers: byte buffer
    char msgbuff[1500];

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
            int bytes = recv(socketDescriptor,msgbuff,1500,0);
            MessageBase::Reference msg(new TestMessage(refConnection, std::string(msgbuff,bytes)));
            sender->send(msg);
        }

        /*std::this_thread::sleep_for(std::chrono::milliseconds(((rand()%100)+1)*10));
        MessageBase::Reference msg(new TestMessage(refConnection));
        if((rand() % 100) < 10)
            break;
        else
            sender->send(msg);*/
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

    std::cout << "Connection " << this->id << " finished...\n";
    this->server->deleteConnection(refConnection);
}

void Connection::stopThread()
{
    /**
     * Zadanie zatrzymania polaczenia: np. shutdown na sockecie
     * Ta metoda moze byc wykonywana z roznych watkow, pamietac o atomowosci
     */

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
    send(socketDescriptor,msg.data(),sizeof(msg.data()),0);
    /**
     * Wyslanie wiadomosci do socketa
     * Tutaj mozna umiescic rowniez szyfrowanie
     * Jesli robienie send na ubitych socketach przeszkadza, zrobic jakas kontrole.
     * Czasem watek sendera moze chciec wysylac jakies wiadomosci, gdy polaczenie jest juz zamkniete
     *
     **/
}

bool Connection::operator==(const Connection& comp_to) {
    return this->id == comp_to.id;
}
