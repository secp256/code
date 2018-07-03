#include <iostream>
#include <string>

#include "sha.hpp"

// g++ m.cpp  -I/usr/local/opt/openssl/include ./libsha.a -lcrypto
int main() {
    std::string x = "hello world";
    cout << sha256(x) << endl;
    return 0;
}

