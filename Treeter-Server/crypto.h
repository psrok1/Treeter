#ifndef CRYPTO_H
#define CRYPTO_H

#include <string>
#include <memory>
#include "openssl/rsa.h"
#include "openssl/aes.h"

namespace Crypto
{
    typedef std::shared_ptr<RSA> RSAPtr;

    class AESContext
    {
        unsigned char* key;
        static unsigned char iv[16];
    public:
        AESContext(unsigned char* k);
        ~AESContext();
        std::string encrypt(std::string msg);
        std::string decrypt(std::string msg);
    };

    class RSAContext
    {
        RSAPtr key;
    public:
        RSAContext(RSAPtr k);
        std::string getEncodedPublicKey();
        AESContext decodeAESKey(std::string aes_key);
    };

    namespace RSAProvider
    {
        extern void generate();
        extern RSAContext getKey();
    }

    extern void initialize();
    extern void free();
}

#endif // CRYPTO_H
