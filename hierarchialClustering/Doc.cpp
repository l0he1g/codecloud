#include "Doc.h"

// must make sure s is ascending order
Doc::Doc(long id, const string& s): m_id(id) {
    vector<string> wvs = util::split(s, ',');
    for (vector<string>::iterator it = wvs.begin();
         it != wvs.end();
         ++it) {
        m_words.push_back(*it);
    }
    
    normalize();
}

string Doc::toString() const {
    ostringstream os;
    //os << "doc id:" << m_id << "\t";
    for(vector<Word>::const_iterator it = m_words.begin();
        it != m_words.end();
        ++it){
        os << it->id << ":" << it->v << ",";
    }
    return os.str();
}


void Doc::normalize() {
    float s = 0.0;
    for(vector<Word>::iterator it = m_words.begin();
        it != m_words.end();
        ++it){
        s += (it->v)*(it->v);
    }

    for(vector<Word>::iterator it = m_words.begin();
        it != m_words.end();
        ++it){
        it->v = it->v/sqrt(s);
    }
}
