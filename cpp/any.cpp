#include <iostream>
#include <list>
#include <boost/any.hpp>

typedef std::list<boost::any> list_any;

void fill_list(list_any &la)
{
    la.push_back(10);
    la.push_back(std::string("hello world"));
}

void show_list(list_any &la)
{
    list_any::iterator it;
    boost::any anyone;

    for (it = la.begin(); it != la.end(); ++it) {
        anyone = *it;

        if (anyone.type() == typeid(int)) {
            std::cout << boost::any_cast<int>(*it) << std::endl;
        }
        else if (anyone.type() == typeid(std::string)) {
            std::cout << boost::any_cast<std::string>(*it) << std::endl;
        }
    }
}

int main()
{
    list_any la;
    fill_list(la);
    show_list(la);
    return 0;
}
