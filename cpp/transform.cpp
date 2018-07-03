#include <iostream>
#include <algorithm>
#include <vector>
#include <string>
using namespace std;

int op_increase (int i)
{
	return i+1;
}

int op_sum (int i, int j)
{
	return i+j;
}

int to_upper(int c)
{
	if (islower(c))
	{
		return (c-32);
	}

	return c;
}

int to_lower(int c)
{
	if (isupper(c))
	{
		return c+32;
	}

	return c;
}

int main () {
	vector<int> first;
	vector<int> second;
	vector<int>::iterator it;

	// set some values:
	for (int i=1; i<6; i++) first.push_back (i*10); //  first: 10 20 30 40 50

	///将first容器的元素加1赋值给second容器
	second.resize(first.size());        // allocate space !!!必须预先设置一个大小与first相同
	transform (first.begin(), first.end(), second.begin(), op_increase); // second: 11 21 31 41 51
	cout << "second contains:";
	for (it=second.begin(); it!=second.end(); ++it)
	{
		cout << " " << *it;
	}
	cout << endl;
	//*////////////////////////////////////////////

	///将first容器的元素与second容器的元素相加，并将得到的结果重新赋值给first
	transform (first.begin(), first.end(), second.begin(), first.begin(), op_sum); //  first: 21 41 61 81 101
	cout << "first contains:";
	for (it=first.begin(); it!=first.end(); ++it)
		cout << " " << *it;
	cout << endl;
	//*//////////////////////////////////////////////////////////////////////////

	///大小写转换/////////////////////////////////////
	string strsrc("Hello, World!");
	string strdest;
	strdest.resize(strsrc.size());      // !!!必须预先设置一个大小与strsrc相同
	transform(strsrc.begin(), strsrc.end(), strdest.begin(), to_upper); // 转换为大写
	cout << strdest << endl;

	transform(strsrc.begin(), strsrc.end(), strdest.begin(), to_lower); // 转换为小写
	cout << strdest << endl;
	//*/////////////////////////////////////////
    
    vector<int> vi {1,2,3,-1,-2,-3};
    transform(vi.begin(), vi.end(), vi.begin(), [](int i) { return i < 0 ? -i : i; });
    for_each(vi.begin(), vi.end(), [](int i) { cout << i << " "; });

	return 0;
}
