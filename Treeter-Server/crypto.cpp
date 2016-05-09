#include <iostream>

#include "crypto.h"
#include "pthread.h"
#include <thread>
#include <mutex>

#include "openssl/crypto.h"
#include "openssl/evp.h"
#include "openssl/err.h"
#include "openssl/conf.h"

namespace Crypto
{
    namespace RSAProvider
    {
        RSA* rsa_key = nullptr;

        void generate()
        {
            BIGNUM* e = BN_new();
            RSA* key = RSA_new();
            BN_set_word(e, 65537);
            RSA_generate_key_ex(key, 1024, e, NULL);
            BN_free(e);

            if(rsa_key)
                RSA_free(rsa_key);
            rsa_key = key;
        }

        RSA* get()
        {
            return rsa_key;
        }

        std::string serializePublicKey()
        {
            RSA* key = get();
            unsigned char* buf;
            int buf_len = i2d_RSAPublicKey(key, &buf);
            // TODO
            // convert to base64
            // check whether buf must be freed
            return "";
        }
    }

    std::mutex* locks;

    void initialize()
    {
        // Initialize threading
        locks = new std::mutex[CRYPTO_num_locks()];

        CRYPTO_set_id_callback([]() -> unsigned long {
            return static_cast<unsigned long>(pthread_self());
        });

        CRYPTO_set_locking_callback([](int mode, int n, const char*, int) {
            if (mode & CRYPTO_LOCK)
                locks[n].lock();
            else
                locks[n].unlock();
        });

        // Load OpenSSL dependencies
        ERR_load_crypto_strings ();
        OPENSSL_config (NULL);
        OpenSSL_add_all_algorithms ();
    }

    void free()
    {
        CRYPTO_set_locking_callback(NULL);
        delete [] locks;
    }

    std::string messageEncrypt(std::string msg, AES_KEY key)
    {

    }

    std::string messageDecrypt(std::string msg, AES_KEY key)
    {

    }

    AES_KEY keyDecrypt(std::string key, RSA* keypair)
    {

    }
}
