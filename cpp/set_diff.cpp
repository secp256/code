#include<set>
#include<iterator>
#include<iostream>
using namespace std;
int main()
{
    set<int>eg1;
    //插入
    eg1.insert(1);
    eg1.insert(100);
    eg1.insert(5);
    eg1.insert(1);//元素1因为已经存在所以set中不会再次插入1
    eg1.insert(10);
    eg1.insert(9);
    //遍历set，可以发现元素是有序的
    set<int>::iterator set_iter=eg1.begin();
    cout<<"Set named eg1:"<<endl;
    for(;set_iter!=eg1.end();set_iter++) cout<<*set_iter<<" ";
    cout<<endl;
    //使用size()函数可以获得当前元素个数
    cout<<"Now there are "<<eg1.size()<<" elements in the set eg1"<<endl;
    if(eg1.find(200)==eg1.end())//find()函数可以查找元素是否存在
        cout<<"200 isn't in the set eg1"<<endl;

    set<int>eg2;
    for(int i=6;i<15;i++)
        eg2.insert(i);
    cout<<"Set named eg2:"<<endl;
    for(set_iter=eg2.begin();set_iter!=eg2.end();set_iter++)
        cout<<*set_iter<<" ";
    cout<<endl;
    //获得两个set的并
    set<int>eg3;
    cout<<"Union:";
    set_union(eg1.begin(),eg1.end(),eg2.begin(),eg2.end(),insert_iterator<set<int> >(eg3,eg3.begin()));//注意第五个参数的形式
    copy(eg3.begin(),eg3.end(),ostream_iterator<int>(cout," "));
    cout<<endl;
    //获得两个set的交，注意进行集合操作之前接收结果的set要调用clear()函数清空一下
    eg3.clear();
    set_intersection(eg1.begin(),eg1.end(),eg2.begin(),eg2.end(),insert_iterator<set<int> >(eg3,eg3.begin()));
    cout<<"Intersection:";
    copy(eg3.begin(),eg3.end(),ostream_iterator<int>(cout," "));
    cout<<endl;
    //获得两个set的差
    eg3.clear();
    set_difference(eg1.begin(),eg1.end(),eg2.begin(),eg2.end(),insert_iterator<set<int> >(eg3,eg3.begin()));
    cout<<"Difference:";
    copy(eg3.begin(),eg3.end(),ostream_iterator<int>(cout," "));
    cout<<endl;
    //获得两个set的对称差，也就是假设两个集合分别为A和B那么对称差为AUB-A∩B
    eg3.clear();
    set_symmetric_difference(eg1.begin(),eg1.end(),eg2.begin(),eg2.end(),insert_iterator<set<int> >(eg3,eg3.begin()));
    copy(eg3.begin(),eg3.end(),ostream_iterator<int>(cout," "));
    cout<<endl;
    return 0;
}
