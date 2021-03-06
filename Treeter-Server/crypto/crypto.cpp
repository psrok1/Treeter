#include "crypto.h"
#include "base64.h"

#include <thread>
#include <mutex>
#include <cassert>
#include <stdexcept>
#include <system_error>

#include <iostream>
#include <sstream>
#include <iomanip>

#include "pthread.h"

#include "openssl/crypto.h"
#include "openssl/evp.h"
#include "openssl/err.h"
#include "openssl/conf.h"
#include "openssl/pem.h"
#include "openssl/sha.h"

namespace Crypto
{
    // Custom std::system_error handler for OpenSSL
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

    // RSA-1024 key generation
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
        std::cout << "Crypto: RSA-1024 generated\n";
        return key;
    }

    AESContext::AESContext(): valid(false) {}
    AESContext::AESContext(AESKey k): key(k), valid(true) { }

    // Check, whether AESContext contains valid AES key
    bool AESContext::isValid() const
    {
        return this->valid;
    }

    std::string AESContext::encrypt(std::string msg)
    {
        if(!this->valid)
            throw std::logic_error("AESContext isn't valid object");

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

            if(!EVP_EncryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key.get(), iv))
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
        if(!this->valid)
            throw std::logic_error("AESContext isn't valid object");

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

            if(!EVP_DecryptInit_ex(ctx, EVP_aes_128_cbc(), NULL, key.get(), iv))
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

    RSAContext::RSAContext(): valid(false) { }
    RSAContext::RSAContext(RSAPtr k): key(k), valid(true) { }

    // Check whether RSAContext contains valid RSA keypair
    bool RSAContext::isValid() const
    {
        return this->valid;
    }

    // Returns Base64-encoded PEM with RSA public key
    std::string RSAContext::getEncodedPublicKey()
    {
        if(!this->valid)
            throw std::logic_error("RSAContext isn't valid object");

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

    // Decrypts Base64-encoded and encrypted AES key using RSA private key
    AESContext RSAContext::decodeAESKey(std::string aes_key)
    {
        if(!this->valid)
            throw std::logic_error("RSAContext isn't valid object");

        std::string raw_encoded_aes = Base64::decode(aes_key);
        AESKey raw_aes = AESKey(new unsigned char[raw_encoded_aes.size()]);

        if(RSA_private_decrypt(raw_encoded_aes.size(),
                            reinterpret_cast<const unsigned char*>(raw_encoded_aes.data()),
                            raw_aes.get(), key.get(), RSA_PKCS1_PADDING) < 0)
        {
            throw std::system_error(ErrorHandling::ssl_error());
        }

        return AESContext(raw_aes);
    }

    // RSA keypair provider
    namespace RSAProvider
    {
        RSAPtr currentKey;
        std::mutex mu;

        // Threadsafe keypair OpenSSL object setter
        void _set(RSAPtr key)
        {
            std::unique_lock<std::mutex> lck(mu);
            currentKey = key;
        }

        // Threadsafe keypair OpenSSL object getter
        RSAPtr _get()
        {
            std::unique_lock<std::mutex> lck(mu);
            assert(currentKey!=nullptr);
            return currentKey;
        }

        // Request key generation
        void generate()
        {
            RSAPtr newKey = createRSAKey();
            _set(newKey);
        }

        // Keypair getter (wrapped in RSAContext object)
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
        std::cout << "Crypto: initialize()\n";
    }

    void free()
    {
        CRYPTO_set_locking_callback(NULL);
        delete [] locks;
        std::cout << "Crypto: free()\n";
    }

    std::string sha256(std::string message)
    {
        unsigned char hash[SHA256_DIGEST_LENGTH];
        SHA256_CTX ctx;

        SHA256_Init(&ctx);
        SHA256_Update(&ctx, message.data(), message.size());
        SHA256_Final(hash, &ctx);

        std::stringstream ss_hash;
        for(size_t i = 0; i < SHA256_DIGEST_LENGTH; i++)
        {
            ss_hash << std::hex << std::setfill('0')
                    << std::uppercase << std::setw(2)
                    << static_cast<int>(hash[i]);
        }
        return ss_hash.str();
    }
}
