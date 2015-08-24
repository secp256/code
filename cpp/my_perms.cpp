#include <stdio.h>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

void swap(char &a, char &b)
{
    char tmp = a;
    a = b;
    b = tmp;
}

void my_perms(char *str, char *p_begin)
{
    if ('\0' == *p_begin)
        printf("%s\n", str);

    for(char *p = p_begin; *p != '\0'; ++p) {
        swap(*p, *p_begin);
        my_perms(str, p_begin + 1);
        swap(*p, *p_begin);
    }
}

void my_perms(char *str)
{
    if (str == NULL)
        return;
    my_perms(str, str);
}

string insert_char_at(string word, char c, int i)
{
    string s = word.substr(0, i);
    string e = word.substr(i);
    return s + c + e;
}

vector<string>get_perms(string str)
{
    vector<string> perms;
    if (str.empty()) {
        perms.push_back("");
    }
    else {
        char first = str.at(0);
        string remainder = str.substr(1);

        vector<string> words = get_perms(remainder);
        for (size_t i = 0; i < words.size(); ++i) {
            string word = words.at(i);
            for (size_t j = 0; j <= word.size(); ++j) {
                string s = insert_char_at(word, first, j);
                perms.push_back(s);
            }
        }
    }
    return perms;
}

int main()
{
    // 1
    // char str[] = "abcd";
    // my_perms(str);

    // 2
    std::string str2="abc";
    vector<string> res = get_perms(str2);
    for (size_t i = 0; i < res.size(); ++i) {
        cout << res.at(i) << endl;
    }

    return 0;
}

