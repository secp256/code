#include <iostream>
#include <string>
#include <vector>
#include <algorithm>
#include <sstream>
#include "boost/variant.hpp"

class stream_output_visitor : public boost::static_visitor<void> {
    std::ostream& os_;
public:
    stream_output_visitor(std::ostream& os) : os_(os) {}

    template <typename T> void operator()(T& t) const {
        os_ << t << '\n';
    }
};

class lexicographical_visitor : public boost::static_visitor<bool> {
public:
    template <typename LHS,typename RHS>
        bool operator()(const LHS& lhs,const RHS& rhs) const {
            return get_string(lhs)<get_string(rhs);
        }
private:
    template <typename T> static std::string
        get_string(const T& t) {
            std::ostringstream s;
            s << t;
            return s.str();
        }

    static const std::string& get_string(const std::string& s) {
        return s;
    }
};

int main() {
    boost::variant<int,std::string> var1="100";
    boost::variant<double> var2=99.99;

    std::cout << "var1<var2: " <<
        boost::apply_visitor(
                lexicographical_visitor(),var1,var2) << '\n';
    std::cout << std::endl;

    typedef std::vector<
        boost::variant<int,std::string,double> > vec_type;
    vec_type vec;
    vec.push_back("Hello");
    vec.push_back(12);
    vec.push_back(1.12);
    vec.push_back("0");

    stream_output_visitor sv(std::cout);
    std::for_each(vec.begin(),vec.end(),sv);
    std::cout << std::endl;

    lexicographical_visitor lv;
    std::sort(vec.begin(),vec.end(),boost::apply_visitor(lv));

    std::cout << '\n';
    std::for_each(vec.begin(),vec.end(),sv);
};
