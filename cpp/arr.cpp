#include <vector>
#include <string>
#include <iostream>
#include <algorithm>

int main()
{
    std::vector<std::string> vec;
    vec = {"abc", "123"};
    std::copy(vec.begin(), vec.end(), std::ostream_iterator<std::string>(std::cout, " "));
    std::cout << std::endl;

    return 0;
}
