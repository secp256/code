#include "aes_base64.h"

#include <stdio.h>
#include <openssl/bio.h>
#include <openssl/buffer.h>
#include <openssl/hmac.h>
#include <openssl/aes.h>

int aes_ecb_encrypt(std::string &output, const std::string &input, bool decrypt)
{
    // 64 bit key
    unsigned char aes_key[] = "7bc7d3c7a04c5721";

    output = "";
    // process 16 byte each time
    for (unsigned int i = 0; i < input.length(); i += 16) {
        unsigned char aes_in[32] = {0};
        size_t index = 0;
        for (unsigned int j = i; j < i + 16 && j < input.length(); j++) {
            aes_in[index++] = input[j];
        }

        unsigned char aes_out[32] = {0};
        if (!decrypt) {
            AES_KEY key;
            AES_set_encrypt_key(aes_key, 128, &key);
            AES_ecb_encrypt(aes_in, aes_out, &key, AES_ENCRYPT);
        }
        else {
            AES_KEY key;
            AES_set_decrypt_key(aes_key, 128, &key);
            AES_ecb_encrypt(aes_in, aes_out, &key, AES_DECRYPT);
        }
        output += reinterpret_cast<char*>(aes_out);
    }

    return 0;
}

int base64_encode_for_aes(std::string &dest, const std::string &src)
{
    BIO *bmem, *b64;
    BUF_MEM *bptr;

    b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    bmem = BIO_new(BIO_s_mem());

    b64 = BIO_push(b64, bmem);
    BIO_write(b64, src.c_str(), src.size());

    BIO_flush(b64);
    BIO_get_mem_ptr(b64, &bptr);

    dest.assign(bptr->data, bptr->length);
    BIO_free_all(b64);

    return 0;
}

int base64_decode_for_aes(std::string &dest, const std::string &src)
{
    if (src.empty()) {
        dest = "";
        return 0;
    }

    BIO * b64 = BIO_new(BIO_f_base64());
    BIO_set_flags(b64, BIO_FLAGS_BASE64_NO_NL);
    BIO * bmem = BIO_new_mem_buf(const_cast<char*>(src.c_str()), src.length());
    bmem = BIO_push(b64, bmem);

    int readlen;
    char readbuf[1024];
    while ((readlen = BIO_read(bmem, readbuf, 1024)) > 0) {
        dest.append(readbuf, readlen);
    }

    BIO_free_all(b64);
    return 0;
}

// encrypt
int get_db_encrypt_code(std::string &output, const std::string &input)
{
    std::string aes_str;
    if (aes_ecb_encrypt(aes_str, input.c_str(), true) != 0) {
        // printf("aes_ecb_encrypt encrypt failed\n");
        return -1;
    }

    if (base64_encode_for_aes(output, aes_str.c_str()) != 0) {
        // printf("base64 encode failed\n");
        return -1;
    }
    return 0;
}

// decrypt
int get_db_decrypt_code(std::string &output, const std::string &input)
{
    std::string base64_str;
    if (base64_decode_for_aes(base64_str, input) != 0) {
        // printf("base64 decode failed\n");
        return -1;
    }

    if (aes_ecb_encrypt(output, base64_str, false) != 0) {
        // printf("aes_ecb_encrypt decrypt failed\n");
        return -1;
    }

    return 0;
}
