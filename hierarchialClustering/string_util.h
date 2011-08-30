#ifndef UTILS_TRIM_HPP
#define UTILS_TRIM_HPP

#include <string>
#include <vector>
#include <sstream>
#include <iostream>

using std::string;
using std::vector;
using std::istringstream;

namespace util{
/**
 * Trims spaces off the end and the beginning of the given string.
 */
inline string trim(string& str)
{
    string::size_type pos = str.find_last_not_of(" \n\t");
    if (pos != string::npos){
        str.erase(pos + 1);
        pos = str.find_first_not_of(" \n\t");
        if (pos != string::npos)
        {
            str.erase(0, pos);
        }
    }
    else{
        // There is nothing else but whitespace in the string
        str.clear();
    }
    return str;
}

/**
 * split a string by delimiter
 */
inline vector<string> split(string str, char delim=' ') {
    vector<string> vec;
    istringstream iss(str);
    string tmps;
    
    while (getline(iss, tmps, delim)) {
        vec.push_back(tmps);
    }
    return vec;
}

//test
/*int main(){
  string s("a:b;c:1;d:2");
  vector<string> vec = split(s, ';');
  }*/
}

#endif
