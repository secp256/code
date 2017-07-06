#include <iostream>

using namespace std;

class TestClass
{
public:
    int member_i;
    char member_c;
};

int main()
{
    cout << "test sizeof class member: \n";
    cout << sizeof(TestClass::member_i) << "\t" << sizeof(TestClass::member_c) << endl;

    int i = 0;
    int j = 0;
    ((i>0) ? i : j) = 1;
    cout << i << endl;
    cout << j << endl;

    if (1==1 or 2!=2) {
        cout << "hello" << endl;
    }
    return 0;
}
