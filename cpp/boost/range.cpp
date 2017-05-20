#include <string>
#include <iostream>

#include <boost/range/iterator_range.hpp>
#include <boost/range/as_literal.hpp>

void test_range_construct_string()  
{  
    typedef std::string::iterator                   iterator;  
    typedef std::string::const_iterator             const_iterator;  
    typedef boost::iterator_range<iterator>           irange;  
    typedef boost::iterator_range<const_iterator> cirange;  
    std::string       str  = "hello world";  
    const std::string cstr = "const world";  

    // 1. 基本构建方法  
    boost::iterator_range<std::string::iterator> ir(str);  
    boost::iterator_range<std::string::const_iterator> cir(str);  

    // 2. 利用make_iterator_range(几种重载函数)  
    irange r    = boost::make_iterator_range(str);  
    r           = boost::make_iterator_range(str.begin(), str.end());  
    cirange r2  = boost::make_iterator_range(cstr);  
    r2          = boost::make_iterator_range(cstr.begin(), cstr.end());  
    r2          = boost::make_iterator_range(str);  
    assert(r == str);  
    assert(r.size() == 11);  

    irange r3   = boost::make_iterator_range(str, 1, -1);  
    assert(boost::as_literal("ello worl") == r3);  
    irange r4   = boost::make_iterator_range(r3, -1, 1);    // 这个也可以理解成复制构造  
    assert(str == r4);  
    std::cout << r4 << std::endl;  

    irange r5 = boost::make_iterator_range(str.begin(), str.begin() + 5);  
    assert(r5 == boost::as_literal("hello"));  
}  

void test_range_type()  
{  
    using namespace boost;  

    // 数组  
    const int SIZE = 9;  
    typedef int array_t[SIZE];  
    const array_t ca = {1, 2, 3, 4, 5, 6, 7, 8, 10};  

    assert((is_same<range_iterator<array_t>::type,       int*          >::value));  
    assert((is_same<range_value<array_t>::type,             int           >::value));  
    assert((is_same<range_difference<array_t>::type,     std::ptrdiff_t>::value));  
    assert((is_same<range_size<array_t>::type,           std::size_t   >::value));  
    assert((is_same<range_const_iterator<array_t>::type, const int*    >::value));  

    assert(begin(ca) == ca);  
    assert(end(ca) == ca + size(ca));  
    assert(empty(ca) == false);  
    std::cout << size(ca) << std::endl;
}  

int main()
{
    test_range_construct_string();
    test_range_type();
    return 0;
}
