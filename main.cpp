#include <iostream>
#include "server.h"
#include <thread>

#include <signal.h>
#include <unistd.h>

using namespace std;

Server serverInstance;

void handle_ctrlc(int) {
    serverInstance.stop();
}

int main(int argc, char *argv[])
{
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = handle_ctrlc;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);

    serverInstance.start();
    serverInstance.join();
    return 0;
}
