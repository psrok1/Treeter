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
    typedef std::shared_ptr<RSA> RSAPtr;

    RSAPtr createRSAKey()
    {
        BIGNUM* e = BN_new();
        RSAPtr key(RSA_new(), RSA_free);

        BN_set_word(e, 65537);
        RSA_generate_key_ex(key.get(), 1024, e, nullptr);

        BN_free(e);

        return key;
    }

    AESContext::AESContext(unsigned char* k): key(k) { }

    AESContext::~AESContext()
    {
        delete [] key;
    }

    std::string AESContext::encrypt(std::string msg)
    {
        EVP_CIPHER_CTX *ctx;
        int len;
        const char *plaintext = msg.data();
        int plaintext_len = msg.size();

        unsigned char *ciphertext = new unsigned char[plaintext_len+256];
        int ciphertext_len;

        ctx = EVP_CIPHER_CTX_new();

        EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
        EVP_EncryptUpdate(ctx, ciphertext, &len, reinterpret_cast<const unsigned char*>(plaintext), plaintext_len);
        ciphertext_len = len;
        EVP_EncryptFinal_ex(ctx, ciphertext + len, &len);
        ciphertext_len += len;

        EVP_CIPHER_CTX_free(ctx);

        std::string output(reinterpret_cast<char*>(ciphertext), ciphertext_len);

        delete [] ciphertext;
        return output;
    }

    std::string AESContext::decrypt(std::string msg)
    {
        EVP_CIPHER_CTX *ctx;
        int len;
        const char *ciphertext = msg.data();
        int ciphertext_len = msg.size();

        unsigned char *plaintext = new unsigned char[ciphertext_len];
        int plaintext_len;

        ctx = EVP_CIPHER_CTX_new();

        EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv);
        EVP_DecryptUpdate(ctx, plaintext, &len, reinterpret_cast<const unsigned char*>(ciphertext), ciphertext_len);
        plaintext_len = len;
        EVP_DecryptFinal_ex(ctx, plaintext + len, &len);
        plaintext_len += len;

        EVP_CIPHER_CTX_free(ctx);

        std::string output(reinterpret_cast<char*>(plaintext), plaintext_len);

        delete [] plaintext;
        return output;
    }

    unsigned char AESContext::iv[16] = "\0\0\0\0\0\0\0\0\0\0\0\0\0\0\0";


    RSAContext::RSAContext(RSAPtr k): key(k) { }

    std::string RSAContext::getEncodedPublicKey()
    {
        BIO* bio = BIO_new(BIO_s_mem());

        PEM_write_bio_RSAPublicKey(bio, key.get());

        size_t skey_len = BIO_ctrl_pending(bio);
        char* skey_buf = new char[skey_len];
        BIO_read(bio, (void*)skey_buf, skey_len);

        std::string serialized = Base64::encode(std::string(skey_buf, skey_len));

        BIO_free(bio);
        delete [] skey_buf;
        return serialized;
    }

    AESContext RSAContext::decodeAESKey(std::string aes_key)
    {
        std::string raw_encoded_aes = Base64::decode(aes_key);
        unsigned char* raw_aes = new unsigned char[raw_encoded_aes.length()];

        RSA_private_decrypt(raw_encoded_aes.size(),
                            reinterpret_cast<const unsigned char*>(raw_encoded_aes.data()),
                            raw_aes, key.get(), RSA_PKCS1_PADDING);

        return AESContext(raw_aes);
    }

    namespace RSAProvider
    {
        RSAPtr currentKey;
        std::mutex mu;

        void _set(RSAPtr key)
        {
            std::unique_lock<std::mutex> lck(mu);
            currentKey = key;
        }

        RSAPtr _get()
        {
            std::unique_lock<std::mutex> lck(mu);
            assert(currentKey!=nullptr);
            return currentKey;
        }

        void generate()
        {
            RSAPtr newKey = createRSAKey();
            _set(newKey);
        }

        RSAContext getKey()
        {
            return RSAContext(_get());
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

    void test()
    {
        RSAProvider::generate();
        std::cout << RSAProvider::getKey().getEncodedPublicKey() << "\n";
    }
}
