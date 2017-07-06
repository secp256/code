#include <iostream>
#include <string>
#include <vector>
#include <exception>

using namespace std;

int main()
{
    vector<string> vec = {"abc", "123", "aaa"};
    for (auto &item : vec) {
        if ("123" == item) {
            throw std::exception();
        }
        cout << item << endl;
    }
    cout << "end" << endl;
    return 0;
}
