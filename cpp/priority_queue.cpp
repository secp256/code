//优先级队列 priority_queue by MoreWindows( http://blog.csdn.net/MoreWindows )
// 支持 empty() size() top() push() pop() 与stack的操作函数全部一样
//
#include <queue>
#include <list>
#include <cstdio>
#include <iostream>

using namespace std;

int main()
{
    //优先级队列默认是使用vector作容器。
    priority_queue<int> a;

    //压入数据
    for (int i = 0; i < 10; i++)
    {
        int j = i * 2 - 5;
        a.push(j);
        cout << "push " << j << endl;
        //b.push(i); //编译错误
    }

    //优先级队列的大小
    cout << "size: " << a.size() << endl;

    //取优先级队列数据并将数据移出队列
    while (!a.empty())
    {
        cout << a.top() << " ";
        a.pop();
    }
    cout << endl;

    return 0;
}
