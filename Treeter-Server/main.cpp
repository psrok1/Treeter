#include <iostream>
#include "server.h"
#include <thread>
#include <cstdlib>
#include <ctime>
#include <signal.h>
#include <unistd.h>
#include <system_error>

#include "config.h"
#include "crypto.h"

using namespace std;

Server::Reference serverInstance;

void handle_ctrlc(int) {
    serverInstance->stopThread();
}

void printLogo();

int main(int argc, char *argv[])
{
    printLogo();
    Crypto::initialize();
    Crypto::test();
    Crypto::free();
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
