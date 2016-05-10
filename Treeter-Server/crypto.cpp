#include <iostream>

#include "crypto.h"
#include "base64.h"
#include "pthread.h"
#include <thread>
#include <mutex>
#include <cassert>

#include "openssl/crypto.h"
#include "openssl/evp.h"
#include "openssl/err.h"
#include "openssl/conf.h"
#include "openssl/pem.h"

namespace Crypto
{
    namespace RSAProvider
    {
        RSA* rsa_key = nullptr;
        std::mutex mu;

        void set(RSA* new_key)
        {
            std::unique_lock<std::mutex> lck(mu);
            if(rsa_key)
                RSA_free(rsa_key);
            rsa_key = new_key;
        }

        RSA* get()
        {
            std::unique_lock<std::mutex> lck(mu);
            assert(rsa_key!=nullptr);
            return rsa_key;
        }

        void generate()
        {
            BIGNUM* e = BN_new();
            RSA* key = RSA_new();

            BN_set_word(e, 65537);
            RSA_generate_key_ex(key, 1024, e, nullptr);

            BN_free(e);

            set(key);
        }

        std::string getPublicKey()
        {
            RSA* key = get();
            BIO* bio = BIO_new(BIO_s_mem());

            PEM_write_bio_RSAPublicKey(bio, key);

            size_t skey_len = BIO_ctrl_pending(bio);
            char* skey_buf = new char[skey_len];
            BIO_read(bio, (void*)skey_buf, skey_len);

            std::cout << skey_len << "\n";
            std::string serialized = Base64::encode(std::string(skey_buf, skey_len));

            BIO_free(bio);
            delete [] skey_buf;
            return serialized;
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
//        EVP_CIPHER_CTX *ctx;

//        int len;

//        int ciphertext_len;

//        ctx = EVP_CIPHER_CTX_new();
//        EVP_EncryptInit(&ctx, EVP_aes_256_cbc(), key, iv);
//        EVP_EncryptUpdate(&ctx, out, &outlen1, in, sizeof(in));
//        EVP_EncryptFinal(&ctx, out + outlen1, &outlen2);

//        /* Clean up */
//        EVP_CIPHER_CTX_free(ctx);

//        return ciphertext_len;
    }

    std::string messageDecrypt(std::string msg, AES_KEY key)
    {

    }

    AES_KEY keyDecrypt(std::string key, RSA* keypair)
    {

    }

    void test()
    {
        RSAProvider::generate();
        std::cout << RSAProvider::getPublicKey() << "\n";
    }
}
