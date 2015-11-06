#include <boost/python.hpp>

#include <string>
#include <iostream>

using namespace std;
using namespace boost::python;

// g++ python.cpp -I /usr/include/python2.7 -lpython2.7 -lboost_python
int main()
{
    Py_InitializeEx(0);
    // Py_Initialize(); // 初始化
    object main_namespace = import("__main__").attr("__dict__");

    // 执行表达式
    exec("result = 5 ** 2", main_namespace);
    // 提取并查看变量result的值
    int five_squared = extract<int>(main_namespace["result"]);
    // 查看变量result的值
    cout << "The five_squeared caculated by python is " << five_squared << endl;

    // python loop
    // "str" is python type
    str lines = "for x in range(1, 4):\n"
                      "\tprint x";
    exec(lines, main_namespace);

    // call function
    char *funcdef = "def power(x, y):\n"
                    "\t return x**y \n"
                    "print power(5,3)\n";
    exec(funcdef, main_namespace);
    object f = main_namespace["power"];
    cout << "power(5, 3): " << extract<int>(f(4,2)) << endl;

    // eval expression
    const char *expr = "(1.1 * 2 + 2 * 3)/ 3";
    cout << "expression value: " << extract<double>(eval(expr)) << endl;

    // 加载sys module.
    object sys = import("sys");
    // 提取python的版本信息
    string version = extract<string>(sys.attr("version"));
    cout << "python version: " << version << endl;

    // call python script
    /*
#!/usr/bin/env python
# -*- coding: utf-8 -*-
def foo(i = 4):
    return i**3
    */
    str filename = "simple.py";

    object simple = exec_file(filename, main_namespace, main_namespace);
    object foo = main_namespace["foo"];
    cout << "Python has caculated foo as " << extract<int>(foo(5)) << endl;

    // Py_Finalize(); // 结束调用Python
    return 0;
}
