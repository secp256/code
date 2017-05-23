#include <vector>
#include <iostream>
#include <algorithm>
#include <functional>

int main()
{
    std::vector<int> c { 1,2,3,4,5,6,7 };
    int x = 5;
    // remove_if()将所有应该移除的元素都移动到了容器尾部并返回一个分界的迭代器. 移除的所有元素仍然可以通过返回的迭代器访问到
    // 为了实际移除元素, 你必须对容器自行调用erase()以擦除需要移除的元素. 这也是erase-remove idiom名称的由来
    c.erase(std::remove_if(c.begin(), c.end(), [x](int n) { return n < x; } ), c.end());

    std::cout << "c: ";
    for (auto i: c) {
        std::cout << i << ' ';
    }
    std::cout << '\n';

    // the type of a closure cannot be named, but can be inferred with auto
    auto func1 = [](int i) { return i+4; };
    std::cout << "func1: " << func1(6) << '\n'; 

    // like all callable objects, closures can be captured in std::function
    // (this may incur unnecessary overhead)
    std::function<int(int)> func2 = [](int i) { return i+4; };
    std::cout << "func2: " << func2(6) << '\n'; 

    return 0;
}
