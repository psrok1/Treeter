#include <iostream>
#include "server.h"
#include <thread>
#include <cstdlib>
#include <ctime>
#include <signal.h>
#include <unistd.h>
#include "database/database.h"

#include "config.h"
#include "crypto/crypto.h"

using namespace std;

Server::Reference serverInstance;
Database DB;

void handle_ctrlc(int) {
    serverInstance->stopThread();
}

void printLogo();

int main()
{
    printLogo();
    srand(time(nullptr));
    struct sigaction sigIntHandler;

    try
    {
        //Initialize the database connection
        DB.init();

        // Loading configuration file
        if(!Configuration::load())
            return 1;

        // Initializing Crypto module
        Crypto::initialize();
        Crypto::RSAProvider::generate();

        // Adding SIGINT handler
        sigIntHandler.sa_handler = handle_ctrlc;
        sigemptyset(&sigIntHandler.sa_mask);
        sigIntHandler.sa_flags = 0;
        sigaction(SIGINT, &sigIntHandler, NULL);


        // Invoking server and waiting until shutdown
        serverInstance = Server::Reference(new Server(Configuration::getServerPort()));
        serverInstance->createThread(serverInstance);
        serverInstance->joinThread();

        // Freeing Crypto
        Crypto::free();
    } catch(std::system_error e)
    {
        std::cerr << "Critical error, terminating: "<<e.what()<<"\n";
    }

    return 0;
}

void printLogo()
{
    std::cout<<"\n";
    std::cout<<"      .+-                                         \n";
    std::cout<<"   yosmMmh+y                                      \n";
    std::cout<<"   -odMMMNs         _____            _            \n";
    std::cout<<"mdsNydMMMmsNyhN`   |_   _| _ ___ ___| |_ ___ _ _  \n";
    std::cout<<":dNMMMMMMMMMMm:      | || '_/ -_) -_)  _/ -_) '_| \n";
    std::cout<<" `/ssshMdsss+`       |_||_| \\___\\___|\\__\\___|_|   \n";
    std::cout<<"      .o:                                         \n";
    std::cout<<"\n";
}
