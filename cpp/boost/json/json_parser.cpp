#include "json_parser.h"  
#include <boost/progress.hpp>  
#include "sstream"  
#include <boost/property_tree/ptree.hpp>  
#include <boost/property_tree/json_parser.hpp>  
#include <boost/property_tree/xml_parser.hpp>  
#include <boost/typeof/typeof.hpp>  
#include <boost/foreach.hpp>  
std::string sms_info::INDEX = "index";  
std::string sms_info::TO = "to";  
std::string sms_info::FROM = "from";  
std::string sms_info::MSG = "msg";  
std::string sms_info::SPLITTER = ",";  
std::string sms_info::TAG = "SMSInfo";  

std::string json_parser::ROOTNAME = "smss";  
void json_parser::tester()  
{  
    std::vector<sms_info> smss1;  
    for(int i = 0; i < 5; i++)  
    {  
        int index = i;  
        std::string to = "1860000" ;  
        std::string from = "1880000" ;  
        std::string msg = "这个短信发给Alex，   谢谢。 ";  
        smss1.push_back(sms_info(index, to, from, msg));  
    }  
    std::string s = generate(smss1);  
    std::vector<sms_info> smss2;  
    parse(s, smss2);  
    assert(smss1.size() == smss2.size());  
}  
std::string json_parser::generate(const std::vector<sms_info>& smss)  
{  
    boost::property_tree::ptree pt_root;  
    boost::property_tree::ptree children;  
    boost::property_tree::ptree child;  

    for(size_t i = 0; i < smss.size(); i++)  
    {  
        sms_info sms = smss.at(i);  
        child.put(sms_info::INDEX, sms.index());  
        child.put(sms_info::TO, sms.to());  
        child.put(sms_info::FROM, sms.from());  
        child.put(sms_info::MSG, sms.msg());  
        children.push_back(std::make_pair("", child));  
    }  

    pt_root.add_child(ROOTNAME, children);  
    std::stringstream ss;  
    boost::property_tree::write_json(ss, pt_root);  
    std::string s = ss.str();  
    return s;  
}  
bool json_parser::parse(const std::string& s, std::vector<sms_info>& smss)  
{  
    std::istringstream iss;  
    iss.str(s.c_str());  
    boost::property_tree::ptree parser;  
    boost::property_tree::json_parser::read_json(iss, parser);  
    boost::property_tree::ptree sms_array = parser.get_child(ROOTNAME);  
    BOOST_FOREACH(boost::property_tree::ptree::value_type &v, sms_array)  
    {  
        boost::property_tree::ptree p = v.second;  
        int index = p.get<int>(sms_info::INDEX);  
        std::string to = p.get<std::string>(sms_info::TO);  
        std::string from = p.get<std::string>(sms_info::FROM);  
        std::string msg = p.get<std::string>(sms_info::MSG);  

        smss.push_back(sms_info(index, to, from, msg));  
    }  
    return true;  
}  
