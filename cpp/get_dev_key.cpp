#include <iostream>
#include <string>

#include <fc/crypto/elliptic.hpp>

#include <graphene/chain/protocol/types.hpp>
#include <graphene/utilities/key_conversion.hpp>

using namespace std;

string gen_wif_key(string raw_wif_key, int index, string c)
{
    string result = raw_wif_key;
    result.insert(index, c);
    return result;
}

int main(int argc, char **argv)
{
    // real public key
    // graphene::chain::public_key_type pub_key {"GXC4z4QJhh5Pia6DPgLwB8vTKhcCUagNesT3cV6N8pvXXfBR86NBH"};
    graphene::chain::public_key_type pub_key{"GXC8GE4vTLWj76HFtkD6gU23TzRUXKZC4F6QqLcw9krnr9mfZTYPr"};

    // raw input wif_key, 50 byte
    std::string raw_wif_key;

    bool need_help = false;
    if (argc < 2)
        need_help = true;
    else {
        raw_wif_key = argv[1];
        if ((raw_wif_key == "-h") || (raw_wif_key == "--help"))
            need_help = true;
    }

    if (need_help) {
        std::cerr << argv[0] << " " << argv[1] << "\n";
        std::cerr << "example:\n get-dev-key raw_wif_key\n";
        return 1;
    }

    string candidates = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
    for (int i = 1; i <= 50; ++i) {
        for (int j = 0; j < candidates.size(); ++j) {
            string wif_key = gen_wif_key(raw_wif_key, i, candidates.substr(j, 1));
            // std::cerr << wif_key << "\n";

            fc::optional<fc::ecc::private_key> optional_private_key = graphene::utilities::wif_to_key(wif_key);
            if (!optional_private_key) {
                // std::cerr << "Invalid private key" << "\n";
                continue;
            }
            graphene::chain::public_key_type wif_pub_key = optional_private_key->get_public_key();
            if (pub_key == wif_pub_key) {
                std::cerr << "you wif_key: " << wif_key << "\n";
                return 0;
            }
        }
    }

    return 0;
}
