#include "aes_base64.h"
#include <stdio.h>

#define STRING_ENCODE "encode"
#define STRING_DECODE "decode"
#define PROG_NAME "./aes_base64"

// g++ test.cpp aes_base64.cpp -lcrypto
void print_help() {
    printf("tools for string encode/decode, using aes and base64 \n"
            "Usage :\n"
            "%s encode/decode str\n", PROG_NAME);
}

int main(int argc,char * argv[])
{
    if (argc < 3) {
        print_help();
        return -1;
    }

    std::string raw_text = argv[2];
    std::string type = argv[1];

    std::string result;
    if (type == STRING_ENCODE) {
        // encode
        if (get_db_encrypt_code(result, raw_text) != 0) {
            printf("get_db_encrypt_code failed\n");
            return -1;
        }
    }
    else if (type == STRING_DECODE) {
        // decode
        if (get_db_decrypt_code(result, raw_text) != 0) {
            printf("get_db_decrypt_code failed\n");
            return -1;
        }
    }
    else {
        // error
        print_help();
        return -1;
    }
    printf("%s\n", result.c_str());

    return 0;
}

