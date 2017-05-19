#include <iostream>
#include <iomanip>
#include <sstream>
#include <string>

std::string ToHex(std::string s)
{
    std::ostringstream ret;
    for (std::string::size_type i = 0; i < s.length(); ++i) {
        std::cout << (int)s[i] << std::endl;
        ret << std::hex 
            << std::setfill('0') 
            << std::setw(8) 
            << std::uppercase
            << (int)s[i];
    }
    return ret.str();
}

int main()
{
    std::string s = "16";
    int num = std::stoi(s, 0, 16);
    std::cout << num << std::endl << std::endl;

    std::string hex_str = ToHex(s);
    std::cout << hex_str << std::endl;
    return 0;
}
