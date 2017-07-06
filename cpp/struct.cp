#include <iostream>

int main()
{
    struct st
    {
        int i = 10;
    };

    st t;
    std::cout << t.i << std::endl;
    return 0;
}
