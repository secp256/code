#include<iostream>
#include<string>
#include<vector>
#include <boost/function.hpp>
#include<algorithm>

using std::string;
using std::vector;
using std::cout;
using std::endl;

struct object_id_type {
    object_id_type(int num, string desc) {
        number = num;
        description = desc;
    }
    object_id_type (const object_id_type &id_type) {
        number = id_type.number;
        description = id_type.description;
    }
    object_id_type() { number = 0; }

    friend bool  operator < ( const object_id_type& a, const object_id_type& b ) { return a.number < b.number; }
    friend bool  operator > ( const object_id_type& a, const object_id_type& b ) { return a.number > b.number; }
    friend std::ostream& operator<<(std::ostream& os, const object_id_type& b) {
        os << "{" <<b.number << "-" << b.description << "}";
        return os;
    }

    int number;
    string description;
};

struct Object {
    Object() { id = 0;};
    Object(int id, string name, object_id_type type) {
        this->id = id;
        this->name = name;
        this->type = type;
    };
    ~Object() {};

    friend std::ostream& operator<<(std::ostream& os, const Object &obj);

    int id;
    string name;
    object_id_type type;
};

struct CmpByName {
    bool operator() (const Object &l, const Object &r) {
        return l.name.compare(r.name) < 0 ? true : false;

    }
};

struct CmpById {
    bool operator() (const Object &l, const Object &r) {
        return l.id < r.id;
    }
};

struct CmpByType {
    bool operator() (const Object &l, const Object &r) {
        return l.type < r.type;
    }
};

std::ostream &operator<<(std::ostream &os, const Object &obj)
{
    os << "{" << obj.id << "." + obj.name + ".";
    os << obj.type << "}";
    return os;
}

typedef boost::function<bool(Object, Object)> Func;
bool cmp_by_id(const Object &l, const Object &r) { return l.id < r.id; }

bool cmp_by_name(const Object &l, const Object &r)
{
    return l.name.compare(r.name) < 0 ? true : false;
}

bool cmp_by_type(const Object &l, const Object &r) { return l.type < r.type; }

int main()
{
    vector<Object> obj_vec;
    obj_vec.push_back({3, "ob3", {13, "type_3"}});
    obj_vec.push_back({2, "ob2", {12, "type_2"}});
    obj_vec.push_back({1, "ob1", {11, "type_1"}});
    obj_vec.push_back({0, "ob0", {10, "type_0"}});

    cout << "before sort..." << endl;
    for (auto ob : obj_vec) {
        cout << ob << endl;
    }

    string order_by = "id";
    /*
    // get compare function
    Func compare_func;
    if ("name" == order_by) {
        compare_func = cmp_by_name;
    }
    else if ("id" == order_by) {
        compare_func = cmp_by_id;
    }
    else {
        compare_func = cmp_by_type;
    }

    // sort
    std::sort(obj_vec.begin(), obj_vec.end(), compare_func);
    */

    if ("name" == order_by) {
        std::sort(obj_vec.begin(), obj_vec.end(), CmpByName());
    }
    else if ("id" == order_by) {
        std::sort(obj_vec.begin(), obj_vec.end(), CmpById());
    }
    else {
        std::sort(obj_vec.begin(), obj_vec.end(), CmpByType());
    }

    cout << "after sort..." << endl;
    for (auto ob : obj_vec) {
        cout << ob << endl;
    }

    return 0;
}
