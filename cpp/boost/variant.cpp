#include <iostream>
#include <string>
#include <boost/variant.hpp>

using std::cout;
using std::endl;
using std::string;

class print_visitor : public boost::static_visitor<void> {
    public:
        void operator() (int i) const {
            cout << "It's an int: " << i << endl;
        }

        void operator() (string s) const {
            cout << "It's a string: " << s << endl;
        }

        void operator() (double d) const {
            cout << "It's a double: " << d << endl;
        }
};

int main()
{
    boost::variant<int, string, double, char> my_first_variant("hello world");
    cout << my_first_variant << endl;
    
    /*
    my_first_variant=24;
    my_first_variant=2.52;
    my_first_variant="Fabulous!";
    my_first_variant=0;

    assert(boost::get<int>(my_first_variant) == 0);

    int* val = boost::get<int>(&my_first_variant);
    assert(val && (*val) == 0);
    */
    
    print_visitor v;
    boost::apply_visitor(v, my_first_variant);

    my_first_variant = 12;
    boost::apply_visitor(v, my_first_variant);

    my_first_variant = 1.1;
    boost::apply_visitor(v, my_first_variant);

    my_first_variant = 'a';
    boost::apply_visitor(v, my_first_variant);

    return 0;
}
