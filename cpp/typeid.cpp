#include <string>  
#include <sstream>  
#include <iostream>  
using namespace std;  

#define varName(x) #x  
#define printExp(exp) cout<<#exp<<"为:\t\t"<<(exp)<<endl  
#define printExpToString(exp) cout<<(string(#exp)+"为:\t\t")<<(exp).toString()<<endl //注意exp加括号更安全  

namespace LC {  
    template<typename T> class Base {  
    public:  
        virtual string toString() {  
            stringstream ss;  
            ss << typeid(*this).name();  
            return ss.str();  
        }  

        Base operator+(int a) {  
            return *this;  
        }  
    };  

    template<typename T> class Derived : public Base<T> {  
    public:  
        virtual string toString() {  
            stringstream ss;  
            ss << typeid(*this).name();  
            return ss.str();  
        }  
    };  
}  

typedef LC::Base<double> BaseDouble;  

typedef LC::Derived<int> DerivedInt;  

int main(int argc, char** argv)  
{  
    int aInt = 1;  
    double d = 1.0;
    printExp(aInt);  
    printExp(typeid(int).name());  
    printExp(typeid(aInt).name());  
    printExp(typeid(d).name());  
    printExp(typeid(std::string).name());  
    cout << "变量名称为：" << varName(aInt) << endl;  

    cout << endl;  
    /*
    BaseDouble base;  
    printExpToString(base);  
    printExpToString(base + 5 + 4);//若定义中exp不加括号就无法编译了  
    printExpToString((LC::Base<int>)DerivedInt());  

    cout << endl;  
    DerivedInt derived;  
    LC::Base<int>* pBase = &derived;  
    LC::Base<int> baseCast = derived;  
    printExpToString(*pBase);  
    printExpToString(derived);  
    printExpToString(baseCast);  

    printExp(typeid(*pBase) == typeid(derived));  
    printExp(typeid(baseCast) == typeid(derived));  
    */

    return 0;  
} 

