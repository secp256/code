#include <fstream>
#include <iostream>
#include <iomanip>

using namespace std;
int main()
{
    const char *file="1.txt";
    ofstream f1(file);
    if (!f1) return -1;

    f1 << "NAME: " << "Tom" << endl;
    f1 << "ADDRESS: " << "New York" << endl;
    f1.close();

    ifstream f2(file);
    if (!f2) return -1;
    string line;
    while (getline(f2, line)) {
        cout << line << endl;
    }


    file = "try.txt";
    fstream f(file, ios::out);
    f << 1234 << ' ' << 3.14 << 'A' << "How are you";
    f.close();
    
    f.open(file, ios::in);
    int i;
    double d;
    char c;
    char s[20];
    f >> i >> d >> c;
    f.getline(s, 20);
    cout << i << endl;
    cout << d << endl;
    cout << c << endl;
    cout << s << endl;
    f.close();
    return 0;
}
