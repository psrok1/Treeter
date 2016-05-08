#ifndef CRYPTO_H
#define CRYPTO_H

#include <string>
#include "openssl/rsa.h"
#include "openssl/aes.h"

namespace Crypto
{
    extern void initialize();
    extern void free();

    extern std::string messageEncrypt(std::string msg, AES_KEY key);
    extern std::string messageDecrypt(std::string msg, AES_KEY key);
    extern AES_KEY keyDecrypt(std::string key, RSA* keypair);
}

#endif // CRYPTO_H
