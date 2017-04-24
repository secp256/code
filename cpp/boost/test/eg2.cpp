#define BOOST_TEST_MODULE example
#include <boost/test/included/unit_test.hpp>
#include <iostream>
#include <vector>
#include <boost/assign.hpp>

using namespace std;

// 全局测试夹具类
struct global_fixture {
    global_fixture() {cout << "global setup" << endl;}
    ~global_fixture() {cout << "global teardown" << endl;}
};

// 定义全局夹具
BOOST_GLOBAL_FIXTURE(global_fixture);

// 测试套件夹具类
struct assign_fixture {
    assign_fixture() {cout << "suit setup" << endl;}
    ~assign_fixture() {cout << "suit teardown" << endl;}

    std::vector<int> v;
};

// 定义测试套件级别的夹具
BOOST_FIXTURE_TEST_SUITE(s_assign, assign_fixture)


BOOST_AUTO_TEST_CASE(t_assign1) {
    using namespace boost::assign;
    v += 1, 2, 3, 4;
    BOOST_CHECK_EQUAL(v.size(), 4);
    BOOST_CHECK_EQUAL(v[2], 3);
}

BOOST_AUTO_TEST_CASE(t_assign2) {
    using namespace boost::assign;

    push_back(v)(11)(22)(33);
    BOOST_CHECK_EQUAL(v.empty(), false);
    BOOST_CHECK_LT(v[0], v[1]);
}

BOOST_AUTO_TEST_SUITE_END()
