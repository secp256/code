#include <iostream>
#include <sstream>
#include <string>
#include <sstream>
#include "boost/variant.hpp"

class stream_output_visitor :
    public boost::static_visitor<void> {
        std::ostream& os_;
    public:
        stream_output_visitor(std::ostream& os) : os_(os) {}

        template <typename T> void operator()(T& t) const {
            os_ << t << '\n';
        }
    };

int main() {
    boost::variant<int,std::string> var;
    var=100;
    boost::apply_visitor(stream_output_visitor(std::cout),var);
    var="One hundred";
    boost::apply_visitor(stream_output_visitor(std::cout),var);
}
