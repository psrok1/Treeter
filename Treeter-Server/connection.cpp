#include "connection.h"
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

bool Connection::readFromSocket(char* buffer, int length)
{
    int recv_bytes=0;
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
    MessageSender::Reference sender = this->server->getSender();
    std::cout << "Connection " << this->id << " started...\n";
    // TODO: Inicjalizacja....

    //Set max. descriptor value
    maxdesc = socketDescriptor>shutdownPipe[0]? socketDescriptor+1: shutdownPipe[0]+1;

    //
    int len;
    char* buffer;

    try
    {
        while(!stopped)
        {
                //Read payload length - false forces us to terminate the connection
                if(!readFromSocket(reinterpret_cast<char*>(&len),4))
                    break;
                len = ntohl(len);

                buffer = new char[len];
                //Read payload
                if(!readFromSocket(buffer,len))
                {
                    delete[] buffer;
                    break;
                }

                MessageBase::Reference msg(new TestMessage(refConnection, std::string(buffer,len)));
                delete[] buffer;
                sender->send(msg);
        }
    }
    catch (std::system_error e)
    {
        std::cout<<"Connection thread exception, terminating: "<<e.what()<<" errno="<<e.code();
    }

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
    shutdown(socketDescriptor, SHUT_RD);

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
    send(socketDescriptor,msg.data(),msg.size(),0);
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
