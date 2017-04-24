#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>
#include <iostream>

struct MyConfig {
    MyConfig()   { std::cout << "global setup\n"; }
    ~MyConfig()  { std::cout << "global teardown\n"; }
};

BOOST_GLOBAL_FIXTURE( MyConfig );

BOOST_AUTO_TEST_CASE( test_case )
{
    BOOST_CHECK( true );
}
