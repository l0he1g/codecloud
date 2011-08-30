// Copyright [2011-03-30] <yanxiaohui@software.ict.ac.cn>
#ifndef CONFIG_H_
#define CONFIG_H_
#include <cstdlib>
#include <string>
#include <fstream>
#include <map>
#include <utility>
#include "string_util.h"

using namespace std;

class Config {
public:
    Config(const char* file): m_file(file) {}
    bool parse() {
        ifstream inf(m_file);
		if (!inf) {
			cerr << "[Error] Cannot open config initial clusters file: " << m_file <<endl;
			exit(EXIT_FAILURE);
		}
        string line;

        while (getline(inf, line)) {
			util::trim(line);
			if (line.empty()) continue;
            if (line.at(0) == '#') continue;   // filter comment

            int p = line.find('=');
            if(p == string::npos) continue;
            
            string k = line.substr(0, p);
            string v = line.substr(p+1);
            util::trim(k);
            util::trim(v);
            _map.insert(make_pair(k, v));
        }

        return (_map.size() > 0);
    }

    string get(const char* k) {
        return _map[k];
    }

    void set(const char* k, const char* v) {
        _map.insert(make_pair(k, v));
    }
    //test
    void print() {
        for( map<string, string>::iterator it = _map.begin();
             it != _map.end();
             ++it){
            printf("k=%s, v=%s\n",  it->first.c_str(), it->second.c_str());
        }
    }
    
private:
    const char* m_file;
    map<string, string> _map;
};

#endif  // _HOME_XH_MYPAPER_PPF_CODE_MSN2_CLUSTER_CPP_CONFIG_H_
