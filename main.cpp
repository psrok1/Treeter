#include <iostream>
#include "server.h"
#include <thread>
#include <cstdlib>
#include <ctime>
#include <signal.h>
#include <unistd.h>

using namespace std;

Server::Reference serverInstance;

void handle_ctrlc(int) {
    serverInstance->stopThread();
}

int main(int argc, char *argv[])
{
    srand(time(nullptr));
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = handle_ctrlc;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);

    serverInstance = Server::Reference(new Server());
    serverInstance->createThread(serverInstance);
    serverInstance->joinThread();
    return 0;
}
