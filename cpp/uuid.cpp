#include <vector>
#include <string>
#include <iostream>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include <boost/lexical_cast.hpp>

using namespace boost::uuids;
using namespace std;

int main()
{
    /*
    // 一些std函数的应用
    vector<unsigned int> v(16, 7);
    uuid u;
    std::copy(v.begin(), v.end(), u.begin());	// 将一个序列复制到另一个序列中(从begin到end)
    std::fill_n(u.data + 2, 6, 8);	// 将数组第二个序列号后的6个值赋为8
    cout <<"u first print:" <<u<<endl;
    std::memset(u.data, 0, u.size());	// 将数组中的所有元素都置零
    cout <<"UUID is empty:"<<u.is_nil()<<endl; // 是否全零
    cout <<"u second print:" <<u<<endl;	// 再次输输出

    // uuid的比较(字典序1~9,a~z排序)
    uuid u1, u2;
    std::fill_n(u1.begin(), u1.size(), 0xab);	// 从第位开始始，后面size()位都赋成0xab
    std::fill_n(u2.begin(), u2.size(), 0x10);
    cout <<"uuid1:"<<u1<<endl<<"uuid2:"<<u2<<endl;
    (u1 >= u2) ? cout<<"uuid1 is Max equal to uuid2\n" : cout<<"uuid2 is Max to uuid1\n";

    // UUID 的几种生成器,它们都是函数对象
    // 第一种 
    // Nil生成器(生成一个全零/无效的UUID/is_nil)
    uuid nilUUID1 = nil_generator()();	// 第一个括号是构造函数,第二个是调用重载操作符(operator())
    uuid nilUUID2 = nil_uuid();	// 内联函数
    cout<< "nilUUID1: "<<nilUUID1<<endl;
    cout<< "nilUUID2: "<<nilUUID2<<endl;

    // 第二种:
    // 字符串生成器(可是c数组,string,wstring,或迭代器指定的字符串区间)
    string_generator sgen;
    uuid strUUID1 = sgen("0123456789abcdef0123456789uvwxyz");	// 没有隔线分开,uvwxyz会变成ffffff
    uuid strUUID2 = sgen("01234567-89ab-cdef-0123-456789abcdef"); // 有分隔,分隔不能乱隔
    uuid strUUID3 = sgen(L"{01234567-89ab-cdef-0123-456789abcdef}"); // 使用"{}"括起来除{}外不能有16进制外的任何字符
    cout << "strUUID1 value: "<<strUUID1<<endl;
    cout << "strUUID2 value: "<<strUUID2<<endl;
    cout << "strUUID3 value: "<<strUUID3<<endl;

    // 第三种:
    // 名字生成器(参数可以是16进制字符外的字符)
    uuid baseUUID = sgen(L"{01234567-89ab-cdef-0123-456789abcdef}");
    name_generator ngen(baseUUID);	// 构造名字生成器,需要一个基准uuid，
    // 只要基准uuid不变,相同字符串总产生相同的uuid
    
    uuid nameUUID1 = ngen("Ajiaxi");	// 为Ajiaxi生成一个UUID, version是shal算法
    uuid nameUUID2 = ngen("Link");	// 为Link生成一个UUID
    cout << "nameUUID1 value: "<<nameUUID1<<endl;
    cout << "nameUUID2 value: "<<nameUUID2<<endl;
    */

    // 第四种:
    // 随机生成器(使用boost::random生成随机种子)
    random_generator rgen;
    uuid ranUUID = rgen();	// 生成一个随机uuid
    cout << "ranUUID value: "<<ranUUID<<endl;
    
    string s = boost::lexical_cast<std::string>(ranUUID);
    string t = boost::uuids::to_string(ranUUID);
    cout << s << std::endl;
    cout << t << std::endl;

    return 0;
}
