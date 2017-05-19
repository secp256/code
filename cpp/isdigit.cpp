#include <iostream>

using std::cout;
int main()
{
    std::string s = "1a900abcd";
    if (std::isdigit(s.front())) {
       cout << "true" << std::endl;
    }
    return 0;
}
