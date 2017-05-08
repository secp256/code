#include <iostream>
#include <memory>

//  g++ weak_ptr2.cpp -std=c++11

std::weak_ptr<int> gw;

void f()
{
    if (auto spt = gw.lock()) { // Has to be copied into a shared_ptr before usage
        std::cout << *spt << "\n";
    }
    else {
        std::cout << "gw is expired\n";
    }
}

int main()
{
    // test 1
    {
        auto sp = std::make_shared<int>(42);
        gw = sp;

        f();
    }

    f();

    // test 2
    std::cout << "-------" << std::endl;
    auto p = std::make_shared<int>(42);
    std::weak_ptr<int> wp = p;
    {
        auto sp = wp.lock();
        std::cout << *sp << std::endl;
    }
    p.reset();
    if (wp.expired()) {
        std::cout << "expired" << std::endl;
    }

    return 0;
}
