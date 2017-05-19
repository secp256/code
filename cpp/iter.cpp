#include <iostream>
#include <vector>

using std::cout;
using std::endl;

template<typename T>
void print(std::vector<T > &v)
{
    // std::vector<T>::iterator iter;
    typename std::vector<T>::iterator iter;
    for (iter = v.begin(); iter != v.end(); ++iter) {
        cout << *iter << endl;
    }
}

int main()
{
    std::vector<int> vec;
    vec.push_back(0);
    vec.push_back(1);
    vec.push_back(2);
    vec.push_back(3);
    print(vec);
    return 0;
}
