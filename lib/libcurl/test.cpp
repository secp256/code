#include <iostream>
#include <string>

#include "ccurl.h"

using namespace std;

#define USER_AGENT "Mozilla/5.0 (X11; Linux i686) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/46.0.2490.86 Safari/537.36"

int main(int argc,char * argv[])
{
    curl_global_init(CURL_GLOBAL_ALL); 

    Curl ccurl;

    if (argv[1]) {
        ccurl.set_url(argv[1]);
    }
    else {
        ccurl.set_url("http://m.youku.com");
    }

    ccurl.add_request_head("User-Agent", USER_AGENT);
    int rc = ccurl.download();

    if (0 != rc) {
        cout << "curl perform failed, code: " << rc << endl;
        return -1;
    }

    std::string head = ccurl.get_head();
    // std::string content = ccurl.get_body();
    // cout << "response header: " << head << endl;

    // get download info
    cout << "status code: " << ccurl.get_curl_status_code() << endl;
    cout << "url: " << ccurl.get_last_url() << endl;

    curl_global_cleanup();
    return 0;
}

