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
    std::mutex* locks;

    void initialize()
    {
        // Initialize threading
        locks = new std::mutex[CRYPTO_num_locks()];

        CRYPTO_set_id_callback([]() -> unsigned long {
            return static_cast<unsigned long>(pthread_self());
        });

        CRYPTO_set_locking_callback([](int mode, int, const char*, int type) {
            if (mode & CRYPTO_LOCK)
                locks[type].lock();
            else
                locks[type].unlock();
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
