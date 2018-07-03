// g++ sha.cpp -I/usr/local/opt/openssl/include  -lcrypto

// gcc -c -o sha.o sha.cpp -I/usr/local/opt/openssl/include
// ar rcs libsha.a sha.o

//
#include <iostream>
#include <string>

#include "sha.hpp"

string sha256(const string str)
{
    char buf[2];
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, str.c_str(), str.size());
    SHA256_Final(hash, &sha256);
    std::string NewString = "";
    for(int i = 0; i < SHA256_DIGEST_LENGTH; i++)
    {
        sprintf(buf,"%02x",hash[i]);
        NewString = NewString + buf;
    }
    return NewString;
}
