#include <algorithm>
#include <functional>
#include <iostream>
#include <vector>
using namespace std;

//for_each 的谓词，为了输出不同的对象， 这里利用了函数对象
template <class T>
class Display : public binary_function<T, T, bool> {
public:
    void operator()(const T& a) { cout << a << endl; }
};

class A  //待排序的类
{
public:
    A(int i) : m_i(i) {}
    int Get() const { return m_i; }
    friend ostream& operator<<(ostream& out, const A& a) {
        out << a.Get();
        return out;
    }

private:
    int m_i;
};

//方法一，使用谓词， 升序排序
bool LessThan(const A& a, const A& b) { return a.Get() < b.Get(); }
//方法二， 重载 <,升序排序
inline bool operator<(const A& a, const A& b) { return a.Get() < b.Get(); }

// 方法三, 重载>, 降序排序， 配合下面的Greater类
inline bool operator>(const A& a, const A& b) { return a.Get() > b.Get(); }

template <class T>
class Greater : public binary_function<T, T, bool> {
public:
    bool operator()(const T& a, const T& b) { return a > b; }
};

int main() {
   vector<A> Avec;
   Avec.push_back(A(5));
   Avec.push_back(A(10));
   Avec.push_back(A(2));
   Avec.push_back(A(2));

   /*
      sort(Avec.begin(), Avec.end(), LessThan);//谓词函数
      for_each(Avec.begin(), Avec.end(), Display<A>()); //升序排序
      cout << endl;

      sort(Avec.begin(), Avec.end()); //默认升序排列类对象对象容器
      for_each(Avec.begin(), Avec.end(), Display<A>());//输出升序排列对象容器
      cout << endl;
      */

   sort(Avec.begin(), Avec.end(), Greater<A>());  //降序排列对象
   for_each(Avec.begin(), Avec.end(), Display<A>());  //输出降序排列对象容器
   cout << endl;

   vector<string> str_vec;
   str_vec.push_back(string("15055556666"));
   str_vec.push_back(string("2"));
   str_vec.push_back(string("success"));
   sort(str_vec.begin(), str_vec.end());
   cout << "-----" << endl;
   for (const auto& item : str_vec) cout << item << endl;

   uint64_t n = (uint64_t(1) << 32);
   n /= 17;

   n /= 60;
   n /= 60;

   n /= 24;
   n /= 365;

   cout << n << endl;

   vector<int> vv = {0, 1, 2, 3, 4, 5, 6, 7, 8};
   int idx = 0;
   for_each(vv.begin(), vv.end(), [&](const int val)->void {  ++idx; });
   cout << idx << endl;

   vector<int>(10, 1).swap(vv);
   for (auto& item : vv)
       cout << item << endl;

   struct data_transaction_commission_rate_t {
       // 10 percent is 1000
       uint16_t league_data_market_commission_rate = (10 );
       uint16_t free_data_market_commission_rate = (10 );
   };
   cout << data_transaction_commission_rate_t().league_data_market_commission_rate << endl;

   return 0;
}
