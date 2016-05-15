#ifndef CRYPTO_H
#define CRYPTO_H

#include <string>
#include <memory>
#include "openssl/rsa.h"
#include "openssl/aes.h"

namespace Crypto
{
    typedef std::shared_ptr<RSA> RSAPtr;
    typedef std::shared_ptr<unsigned char> AESKey;

    class AESContext
    {
        AESKey key;
        static unsigned char iv[16];
        bool valid;
    public:
        AESContext();
        AESContext(AESKey k);

        std::string encrypt(std::string msg);
        std::string decrypt(std::string msg);
        bool isValid() const;
    };

    class RSAContext
    {
        RSAPtr key;
        bool valid;
    public:
        RSAContext();
        RSAContext(RSAPtr k);        
        std::string getEncodedPublicKey();
        AESContext decodeAESKey(std::string aes_key);
        bool isValid() const;
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
