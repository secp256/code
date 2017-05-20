#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/composite_key.hpp>
#include <iostream>
#include <boost/range/algorithm.hpp>
#include <boost/range/adaptors.hpp>

using boost::adaptors::transformed;
using boost::adaptors::uniqued;


struct Person {
    Person(int id, std::string name):
        m_id(id),
        m_name(name)
    { }

    int m_id;
    std::string m_name;
};

namespace bmi = boost::multi_index;

typedef boost::multi_index_container<
    Person,
    bmi::indexed_by<
        bmi::ordered_unique<
            bmi::member<Person, int, &Person::m_id>
        >,
        bmi::ordered_unique<
            bmi::tag<struct by_name_id>,
            bmi::composite_key<
                Person,
                bmi::member<Person, std::string, &Person::m_name>,
                bmi::member<Person, int, &Person::m_id>
            >
        >
    >
> Roster;

int main()
{
    Roster r;
    r.insert(Person(1, "Tom"));
    r.insert(Person(2, "Jack"));
    r.insert(Person(3, "Tom"));
    r.insert(Person(4, "Leo"));

    size_t unique_names = boost::size(r.get<by_name_id>() 
            | transformed([](Person const& p) -> std::string const& { return p.m_name; })
            | uniqued
            );

    std::cout << unique_names << std::endl;
}

