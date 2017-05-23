#include <string>
#include <vector>
#include <iostream>

using std::vector;
using std::string;
using std::cout;
using std::endl;

int main()
{
    vector<string> v;
    v.push_back("abc");
    v.push_back("def");
    v.push_back("ghi");
    v.push_back("123");

    string s = v.front();
    cout << s << endl;
    return 0;
}
