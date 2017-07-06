#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/json_parser.hpp>
#include <boost/date_time.hpp>

#include <string>
#include <vector>
#include <sstream>

using namespace boost::property_tree;
using namespace boost::gregorian;
using namespace boost;

using namespace std;

int main() {

    string strJson = "{ \"people\": [ { \"firstName\": \"Brett\", \"lastName\": \"McLaughlin\", \"email\": \"aaaa\" }, 　{ \"firstName\": \"Jason\", \"lastName\": \"Hunter\", \"email\": \"bbbb\" }, 　　{ \"firstName\": \"Elliotte\", \"lastName\": \"Harold\", \"email\": \"cccc\" }　　 ] }";

    string stra, strc;
    vector<string> vec_str;
    ptree pt, p1, p2;
    stringstream stream;

    stream << strJson;
    read_json<ptree>(stream, pt);
    p1 = pt.get_child("people");

    for (ptree::iterator it = p1.begin(); it != p1.end(); ++it) {
        p2 = it->second;
        stra = p2.get<string>("firstName");
        cout << stra << endl;
        // vec_str.push_back(item);
    }

    return 0;
}
