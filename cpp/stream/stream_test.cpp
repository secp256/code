#include <fstream>
#include <sstream>
#include <string>
#include <iostream>

using namespace std;

int main()
{
    string cppstr1 = "input.txt";

    ifstream fin;
    ofstream fout;

    fin.open(cppstr1.c_str());
    if (!fin.is_open()) {
        cerr << "open file failed" << endl;
        return -1;
    }

    string line;
    while (getline(fin, line)) {
        string a;
        double b;
        int c;

        istringstream is(line);
        is >> a >> b >> c;
        cout << a << ' ' << b << ' ' << c << endl;
    }

    fin.close();

    return 0;
}
