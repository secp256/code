#include <boost/python.hpp>

#include <string>
#include <iostream>

using namespace std;
using namespace boost::python;

// g++ boost_python.cpp -I /usr/include/python2.7 -lpython2.7 -lboost_python
int main()
{
    Py_InitializeEx(0);
    // Py_Initialize(); // init
    object main_namespace = import("__main__").attr("__dict__");

    // python expression
    exec("result = 5 ** 2", main_namespace);
    int five_squared = extract<int>(main_namespace["result"]);
    cout << "var result is: " << five_squared << endl;

    // python loop
    // "str" is python type
    str lines = "for x in range(1, 4):\n"
                      "\tprint x";
    exec(lines, main_namespace);

    // python function
    char *funcdef = "def power(x, y):\n"
                    "\t return x**y \n"
                    "print power(5,3)\n";
    exec(funcdef, main_namespace);
    object f = main_namespace["power"];
    cout << "power(5, 3): " << extract<int>(f(4,2)) << endl;

    // eval expression
    const char *expr = "(1.1 * 2 + 2 * 3)/ 3";
    cout << "expression value: " << extract<double>(eval(expr)) << endl;

    // python module
    object sys = import("sys");
    // get python version
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

    // Py_Finalize(); // finish python environment
    return 0;
}
