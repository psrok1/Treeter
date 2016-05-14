#include <iostream>

#include "crypto.h"
#include "base64.h"
#include "pthread.h"
#include <thread>
#include <mutex>
#include <cassert>
#include <stdexcept>
#include <system_error>

#include "openssl/crypto.h"
#include "openssl/evp.h"
#include "openssl/err.h"
#include "openssl/conf.h"
#include "openssl/pem.h"

namespace Crypto
{
    namespace ErrorHandling
    {
        struct ssl_error_category: public std::error_category
        {
            ssl_error_category() {}
            virtual ~ssl_error_category() noexcept {}

            virtual const char* name() const noexcept
            {
                return "ssl_error";
            }

            virtual std::string message(int errval) const
            {
                char err_msg[4096];
                ERR_error_string( errval , err_msg );
                return std::string( err_msg );
            }
        };

        std::error_code ssl_error()
        {
            static ssl_error_category ssl_category;
            return std::error_code( ERR_get_error() , ssl_category );
        }
    }

    typedef std::shared_ptr<RSA> RSAPtr;

    RSAPtr createRSAKey()
    {
        BIGNUM* e = BN_new();
        RSAPtr key(RSA_new(), RSA_free);

        BN_set_word(e, 65537);
        if(!RSA_generate_key_ex(key.get(), 1024, e, nullptr))
        {
            BN_free(e);
            throw std::system_error(ErrorHandling::ssl_error());
        }

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
        EVP_CIPHER_CTX *ctx = nullptr;
        int len;
        const char *plaintext = msg.data();
        int plaintext_len = msg.size();

        unsigned char *ciphertext = new unsigned char[plaintext_len+256];
        int ciphertext_len;

        try
        {
            ctx = EVP_CIPHER_CTX_new();

            if(ctx == nullptr)
                throw std::system_error(ErrorHandling::ssl_error());

            if(!EVP_EncryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
                throw std::system_error(ErrorHandling::ssl_error());

            if(!EVP_EncryptUpdate(ctx, ciphertext, &len, reinterpret_cast<const unsigned char*>(plaintext), plaintext_len))
                throw std::system_error(ErrorHandling::ssl_error());

            ciphertext_len = len;

            if(!EVP_EncryptFinal_ex(ctx, ciphertext + len, &len))
                throw std::system_error(ErrorHandling::ssl_error());

            ciphertext_len += len;
        } catch(std::system_error)
        {
            if(ciphertext)
                delete [] ciphertext;
            if(ctx)
                EVP_CIPHER_CTX_free(ctx);
            throw;
        }

        EVP_CIPHER_CTX_free(ctx);

        std::string output(reinterpret_cast<char*>(ciphertext), ciphertext_len);

        delete [] ciphertext;
        return output;
    }

    std::string AESContext::decrypt(std::string msg)
    {
        EVP_CIPHER_CTX *ctx = nullptr;
        int len;
        const char *ciphertext = msg.data();
        int ciphertext_len = msg.size();

        unsigned char *plaintext = new unsigned char[ciphertext_len];
        int plaintext_len;

        try
        {
            ctx = EVP_CIPHER_CTX_new();
            if(ctx == nullptr)
                throw std::system_error(ErrorHandling::ssl_error());

            if(!EVP_DecryptInit_ex(ctx, EVP_aes_256_cbc(), NULL, key, iv))
                throw std::system_error(ErrorHandling::ssl_error());
            if(!EVP_DecryptUpdate(ctx, plaintext, &len, reinterpret_cast<const unsigned char*>(ciphertext), ciphertext_len))
                throw std::system_error(ErrorHandling::ssl_error());
            plaintext_len = len;
            if(!EVP_DecryptFinal_ex(ctx, plaintext + len, &len))
                throw std::system_error(ErrorHandling::ssl_error());
            plaintext_len += len;
        } catch(std::system_error)
        {
            if(plaintext)
                delete [] plaintext;
            if(ctx)
                EVP_CIPHER_CTX_free(ctx);
            throw;
        }

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

        if(RSA_private_decrypt(raw_encoded_aes.size(),
                            reinterpret_cast<const unsigned char*>(raw_encoded_aes.data()),
                            raw_aes, key.get(), RSA_PKCS1_PADDING) < 0)
        {
            delete [] raw_aes;
            throw std::system_error(ErrorHandling::ssl_error());
        }

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
}
