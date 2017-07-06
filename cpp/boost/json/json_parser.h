#ifndef __mongoose__json_parser__  
#define __mongoose__json_parser__  
#include <string>  
#include <sstream>  
#include <vector>  

class sms_info  
{  
    sms_info()  
    {  
        index_ = 0;  
    }  
public:  
    static std::string INDEX;  
    static std::string TO;  
    static std::string FROM;  
    static std::string MSG;  
    static std::string SPLITTER;  
    static std::string TAG;  
private:  
    int index_;  
    std::string to_;  
    std::string from_;  
    std::string msg_;  

public:  
    sms_info(int index, const std::string& to, const std::string& from, const std::string& msg)  
    {  
        index_ = index;  
        to_ = to;  
        from_ = from;  
        msg_ = msg;  
    }  
    int index()  
    {  
        return index_;  
    }  
    std::string to()  
    {  
        return to_;  
    }  
    std::string from()  
    {  
        return from_;  
    }  
    std::string msg()  
    {  
        return msg_;  
    }  
};  

class json_parser  
{  
private:  
    static std::string ROOTNAME;  
public:  
    static std::string generate(const std::vector<sms_info>& smss);  
    static bool parse(const std::string& s, std::vector<sms_info>& smss);  
public:  
    static void tester();  
};
