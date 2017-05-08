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
    return 0;
}
