#ifndef DOC_H
#define DOC_H

#include <cassert>
#include <iostream>
#include <sstream>
#include <string>
#include <cmath>
#include <vector>
#include "string_util.h"

using namespace std;

class Word
{
  public:
    Word():id(-1),v(0) {}
    Word( long id, float v):id(id),v(v){}
    // construct from string : id:v
    Word(string s){
        sscanf(s.c_str(), "%ld:%f", &id, &v);
        assert(id >=0 && v > 0);
    }
    void set_v(float v){this->v = v;}

    bool operator < (Word& w) {return id < w.id;}    
  public:    
    long id;
    float v;
};

class Doc {
  public:
    //Doc(): m_id(-1) {}
    Doc(Doc& d): m_id(-1) { m_words = d.get_words();}
    // s format:id:v,id:v,...
    Doc(long id, const string& s);
    
    long id() const {return m_id;}
    const vector<Word>& get_words() const {return m_words;}
    long size() const {return m_words.size();}

    void normalize();
    string toString() const;
    
  private:
    long m_id;
    vector<Word> m_words;
};

#endif
