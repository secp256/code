#include <iostream>
#include <cassert>
#include <string>
#include "boost/regex.hpp"

// g++ match.cpp -lboost_regex

int main() {
    std::string exp = "\\d+\\w+";
    boost::regex reg(exp);

    std::string correct="123hello";
    std::string incorrect="hello";

    assert(boost::regex_match(correct, reg) == true);
    assert(boost::regex_match(incorrect, reg) == false);
    return 0;
}
