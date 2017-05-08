#include <iostream>
#include <string>
#include <vector>
#include <map>

using std::cout;
using std::endl;

// g++ for.cpp -std=c++11
int main()
{
    std::map<std::string, std::vector<int> > m;
    std::vector<int> v;

    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(4);
    m["one"] = v;
    m["two"] = v;

    for (const auto &kvp : m) {
        cout << kvp.first << endl;       
        for (auto v : kvp.second) {
            cout << v << endl;
        }
    }

    int arr[] = {1, 2, 3, 4, 5};
    for (int &e : arr) {
        e = e * e;
        cout << e << endl;
    }

    return 0;
}
