#include <iostream>
#include <vector>
#include <boost/function.hpp>
#include <boost/bind.hpp>

using namespace std;

void print(int a) 
{ 
    cout << a << endl; 
}

typedef boost::function<void (int)> SuccessPrint;

int main() 
{ 
    vector<SuccessPrint> printList;

    SuccessPrint printOne = boost::bind(print, _1); 
    printList.push_back(printOne); 
    SuccessPrint printTwo = boost::bind(print, _1); 
    printList.push_back(printTwo); 
    SuccessPrint printThree = boost::bind(print, _1); 
    printList.push_back(printTwo); 

    // do something else
    for (int i = 0; i < printList.size(); ++i) 
        printList.at(i)(i);
    return 0; 
}
