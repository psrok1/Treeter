#include <iostream>
#include "server.h"
#include <thread>

#include <signal.h>
#include <unistd.h>

using namespace std;

Server testServer;
std::thread thServer(std::ref(testServer));

void handle_ctrlc(int) {
    testServer.stop();
}

int main(int argc, char *argv[])
{
    struct sigaction sigIntHandler;

    sigIntHandler.sa_handler = handle_ctrlc;
    sigemptyset(&sigIntHandler.sa_mask);
    sigIntHandler.sa_flags = 0;

    sigaction(SIGINT, &sigIntHandler, NULL);

    thServer.join();
    return 0;
}
