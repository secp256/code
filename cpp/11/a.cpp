#include <iostream>
#include <vector>
#include <list>

using namespace std;

class TestClass
{
public:
    TestClass() = default;
    TestClass(const int i, const char c): member_i(i), member_c(c) {}

    int member_i;
    char member_c;
};

int main()
{
    cout<<"test =default class construct:\n";
    TestClass tc; // may cause error if no default construct.
    cout<<tc.member_i<<'\t'<<(short)tc.member_c<<endl;
    cout<<"test =default done."<<endl;

    vector <string> v1 = {"ab", "cd", "ef"};
    list <string> l2{"gh", "ij", "kl"};
    cout << "test vector/list list initialization:\n" << v1[1] << "\t" << l2.front() << endl;
    return 0;
}
