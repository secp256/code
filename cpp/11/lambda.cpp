#include <iostream>
#include <vector>

using std::cout;
using std::endl;

int main()
{
    std::vector<int> num(10);
    std::generate(num.begin(), num.end(), 
            []
            {
                return rand() % 10;
            });
    std::copy(num.begin(), num.end(), std::ostream_iterator<int>(std::cout, " "));
    cout << endl;

    int odd_count = 0;
    for_each(num.begin(), num.end(), 
            [&odd_count](int value) 
            {
                if (value % 2 == 1) odd_count++;
            });
    cout << odd_count << endl;
    return 0;
}
