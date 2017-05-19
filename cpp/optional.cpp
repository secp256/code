#include <string>
#include <iostream>
#include <boost/optional.hpp>

using namespace std;
struct T {
    string str;
    bool flag;

    T(string s, bool f) {
        str = s;
        flag = f;
    }
};

boost::optional<T> test(bool flag) const {
    boost::optional<T> ret;

    if (flag) {
        T t("hello", false);
        ret = t;
    } else {
        cout << "return nothing" << endl;
    }
    return ret;
}

int main() {
    boost::optional<T> obj;
    if (obj) {
        cout << "obj not null" << endl;
    } else {
        cout << "obj is null" << endl;
    }

    boost::optional<T> obj1 = test(true);
    if (obj1) {
        cout << obj1.get().str << endl;
    }
    else {
        cout << "obj2 is null" << endl;
    }

    boost::optional<T> obj2 = test(false);
    if (obj2) {
        cout << obj2.get().str << endl;
    }
    else {
        cout << "obj2 is null" << endl;
    }
}
