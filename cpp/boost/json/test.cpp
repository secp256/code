#include <boost/property_tree/ptree.hpp>
#include <boost/typeof/typeof.hpp>
#include <boost/property_tree/json_parser.hpp> 
#include <boost/property_tree/xml_parser.hpp>
#include <iostream> 

using namespace std;
using namespace boost::property_tree;

int main(int argc, char *argv[])
{
    string s = "{\"age\":26,\"study\":{\"language\":{\"one\":\"chinese\",\"two\":\"math\"},\"fee\":500,\"subject\":[{\"one\":\"china\"},{\"one\":\"Eglish\"}]},\"person\":[{\"id\":1,\"name\":\"chen\"},{\"id\":2,\"name\":\"zhang\"}],\"name\" : \"huchao\"}"; 
    cout << s << endl;
    ptree pt;

    stringstream stream(s); 
    read_json<ptree>( stream, pt); 
    pt.put("study.language.one","physics");
    pt.put("study.fee",600); 

    string s1=pt.get<string>("age");
    cout<<s1<<endl;

    string s2=pt.get<string>("name");
    cout<<s2<<endl;

    string s3=pt.get_child("study").get_child("language").get<string>("one");
    cout<<s3<<endl;

    string s4=pt.get_child("study").get<string>("fee");
    cout<<s4<<endl; 

    ptree p1,p2;
    p1 = pt.get_child("study").get_child("subject");
    for (ptree::iterator it = p1.begin(); it != p1.end(); ++it) { 
        p2 = it->second; 
        cout<<"subject="<<p2.get<string>("one")<<endl;
    } 
    return 0;
}
