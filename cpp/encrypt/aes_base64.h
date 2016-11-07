#ifndef INCLUDED_AES_BASE64_H
#define INCLUDED_AES_BASE64_H

#include <string>

int aes_ecb_encrypt(std::string &output, const std::string &input, bool decrypt);
int base64_encode_for_aes(std::string &dest, const std::string &src);
int base64_decode_for_aes(std::string &dest, const std::string &src);

int get_db_encrypt_code(std::string &output, const std::string &input);
int get_db_decrypt_code(std::string &output, const std::string &input);

#endif // INCLUDED_AES_BASE64_H
