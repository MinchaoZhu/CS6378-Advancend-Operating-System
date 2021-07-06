#include "util/string_helper.h"
#include <vector>
#include <string>
using namespace std;


vector<string> split(const string& s, const string& delimiters) {
    string::size_type lastPos=s.find_first_not_of(delimiters,0);
    string::size_type pos=s.find_first_of(delimiters,lastPos);
    vector<string> res;
    while(lastPos!=string::npos || pos!=string::npos)
    {
        res.push_back(s.substr(lastPos,pos-lastPos));
        lastPos=s.find_first_not_of(delimiters,pos);
        pos=s.find_first_of(delimiters,lastPos);
    } 
    return res;
}