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
#include "base64.h"

using namespace std;

Server::Reference serverInstance;

void handle_ctrlc(int) {
    serverInstance->stopThread();
}

void printLogo();

int main(int argc, char *argv[])
{
    printLogo();
    {
        using namespace Crypto;
        initialize();
        RSAProvider::generate();
        RSAContext rsa = RSAProvider::getKey();
        std::cout << "Public key:\n";
        std::cout << rsa.getEncodedPublicKey() << "\n";
        std::cout << "Give me AES key, RSA-encrypted, base64:\n";
        std::string aes_encrypted;
        std::cin >> aes_encrypted;
        AESContext aes = rsa.decodeAESKey(aes_encrypted);
        std::cout << "Encoded:\n";
        std::cout << Base64::encode(aes.encrypt("Kocham Pafcia! <3")) << "\n";
        free();
    }
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
