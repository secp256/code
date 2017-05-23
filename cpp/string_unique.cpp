#include<string>
#include<vector>
#include<iostream>
#include<algorithm>

using std::string;
using std::vector;
int main()
{
    std::vector<string> vec;
    vec.push_back("abc");
    vec.push_back("abc");
    vec.push_back("abcd");
    vec.push_back("abcd");
    vec.push_back("dabc");
    vec.push_back("abc");

    std::sort(vec.begin(), vec.end());
    vector<string>::iterator iter_end = std::unique(vec.begin(), vec.end());
    vec.erase(iter_end, vec.end());

    for (string s : vec)
        std::cout << s << std::endl;
    return 0;
}
