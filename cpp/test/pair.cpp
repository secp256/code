// make_pair example
#include <utility>      // std::pair
#include <iostream>     // std::cout

int main () {
    std::pair <int,int> foo;
    std::pair <int,int> bar;

    foo = std::make_pair (10,20);
    bar = std::make_pair (10.5,'A'); // ok: implicit conversion from pair<double,char>


    std::pair <int, std::string> xxx, yyy;
    xxx = std::make_pair(100, "hello worldx");
    

    int i = 100;
    std::string str = "hello world";
    yyy = std::make_pair(i, str);

    if (xxx == yyy) {
        std::cout << "==" << "\n";
    }
    else {
        std::cout << "<>" << '\n';
    }

    std::cout << "foo: " << foo.first << ", " << foo.second << '\n';
    std::cout << "bar: " << bar.first << ", " << bar.second << '\n';

    return 0;
}
