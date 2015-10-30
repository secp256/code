#include <stdlib.h>
#include <boost/regex.hpp>
#include <string>
#include <iostream>

// g++ match.cpp -lboost_regex
using namespace boost;

regex expression("([0-9]+)(\\-| |$)(.*)");

// process_ftp
// on success returns the ftp response code, and fills 
// msg with the ftp response message. 
int process_ftp(const char* response, std::string *code, std::string* msg)
{
    cmatch what;
    if(regex_match(response, what, expression))
    {
        // what[0] contains the whole string 
        // what[1] contains the response code 
        // what[2] contains the separator character 
        // what[3] contains the text message. 
        if (code)
            code->assign(what[1].first, what[1].second);
        if (msg)
            msg->assign(what[3].first, what[3].second);
        return std::atoi(what[1].first);
    }
    // failure did not match 
    if (code)
        code->erase();

    if (msg)
        msg->erase();
    return -1;
}

int main()
{
    const char *response = "403 forbidden";
    std::string code;
    std::string msg;
    process_ftp(response, &code, &msg);
    std::cout << "code: " << code << "\nmsg: " << msg << '\n';
    return 0;
}

