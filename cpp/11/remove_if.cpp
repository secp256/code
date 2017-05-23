#include <iostream>
#include <algorithm>
#include <list>
#include <ctime>
using namespace std;

int main(void)
{
    const size_t LOOP = 5000000;

    typedef list<int> li;
    {
        li vli;
        for( size_t i=0; i<LOOP; ++i )
        {
            vli.push_back( i&1 ? 2 : 1 );
        }
        clock_t ret1 = clock();
        vli.remove( 2 );
        clock_t ret2 = clock();
        cout << "list shrink:" << ret2-ret1 << endl;
    }
    {
        li vli;
        for( size_t i=0; i<LOOP; ++i )
        {
            vli.push_back( i&1 ? 2 : 1 );
        }
        clock_t ret1 = clock();
        vli.erase( remove(vli.begin(),vli.end(),2), vli.end() );
        clock_t ret2 = clock();
        cout << "list shrink:" << ret2-ret1 << endl;
    }

    return 0;
}
