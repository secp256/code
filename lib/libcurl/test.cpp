#include <iostream>
#include <string>

#include "ccurl.h"

using namespace std;

// g++ test.cpp ccurl.cpp -lcurl

int main()
{
    curl_global_init(CURL_GLOBAL_ALL); 

    Curl ccurl;
    ccurl.set_url("http://m.youku.com");
    int rc = ccurl.download();

    if (0 != rc) {
        cout << "curl perform failed, code: " << rc << endl;
        return -1;
    }
    // cout << "response header: " << ccurl.get_head() << endl;

    // get download info
    cout << "status code: " << ccurl.get_curl_status_code() << endl;

    char *p = NULL;
    ccurl.get_last_url(&p);
    cout << "last url: " << p << endl;

    curl_global_cleanup();
    return 0;
}

