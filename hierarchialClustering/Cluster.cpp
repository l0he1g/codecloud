#include <cmath>
#include <sstream>
#include "Cluster.h"

void Cluster::insert( const Doc* doc) {
    // re-compute centroid
    long n = m_docs.size();
    const vector<Word>& words = doc->get_words();
    vector<Word>::const_iterator it = words.begin();
    // 2. add words in doc
    list<Word>::iterator p = m_centroid.begin();
    for (; p != m_centroid.end(); ++p) {
        // 1. *n
        p->v *= n;

        while (it != words.end() && it->id < p->id) {
            // word not in centroid, insert word
            m_centroid.insert(p,*it);
            ++it;
        }
        // word in centroid, add the v
        if (it != words.end() && it->id == p->id) {
            p->v += it->v;
            ++it;
        }
    }

    // remain words in doc, just insert before end
    while (it != words.end()) {
        m_centroid.insert(p, *it);
        ++it;
    }
    // 3. devide n+1
    for (list<Word>::iterator p = m_centroid.begin();
         p != m_centroid.end();
         ++p) {
        p->v /= (n+1);
    }
    
    // add to docs
    m_docs.push_back(doc);
}

float Cluster::distance(const Doc* doc) const {
    float s = 0;
    const vector<Word>& words = doc->get_words();
    vector<Word>::const_iterator it = words.begin();
    // 2. add words in doc
    list<Word>::const_iterator p = m_centroid.begin();
    for (; p != m_centroid.end(); ++p) {
        while (it != words.end() && it->id < p->id) {
            // word not in centroid, insert word
            s += (it->v)*(it->v);
            ++it;
        }
        // word in centroid, add the v
        if (it != words.end() && it->id == p->id) {
            float t = p->v - it->v;
            s += t*t;
            ++it;
        }
        else {
            s += (p->v) * (p->v);
        }
    }
    // remain words in doc, just insert before end
    while (it != words.end()) {
        s += (it->v)*(it->v);
        ++it;
    }
    return sqrt(s);
}

float Cluster::square_sum (const Doc* d1, const Doc* d2) const {
    float s = 0.0;
    const vector<Word>& words1 = d1->get_words();
    const vector<Word>& words2 = d2->get_words();
    vector<Word>::const_iterator p1 = words1.begin();
    vector<Word>::const_iterator p2 = words2.begin();
    // 2. add words in doc
    for (; p1 != words1.end(); ++p1) {
        while (p2 != words2.end() && p2->id < p1->id) {
            // word not in centroid, insert word
            s += (p2->v)*(p2->v);
            ++p2;
        }
        // word in centroid, add the v
        if (p2 != words2.end() && p2->id == p1->id) {
            float t = p1->v - p2->v;
            s += t*t;
            ++p2;
        }
        else {
            s += (p1->v) * (p1->v);
        }
    }
    // remain words in doc, just insert before end
    while (p2 != words2.end()) {
        s += (p2->v)*(p2->v);
        ++p2;
    }
    return s;    
}

float Cluster::diameter (const Doc* d) const {
    float s = 0;
    if (m_docs.empty()) {
        return 0;
    }
    
    vector<const Doc*> docs = m_docs;
    docs.push_back(d);

    for (vector<const Doc *>::const_iterator p1 = docs.begin();
         p1 != docs.end() - 1;
         ++p1) {
        for (vector<const Doc *>::const_iterator p2 = p1+1;
             p2 != docs.end();
             ++p2) {
            s += square_sum(*p1, *p2);
        }
    }
    
    long n = docs.size();
    return sqrt( s/(n*(n-1)) );
}

string Cluster::toString() const {
    ostringstream os;
    //os << "id:" << m_id << endl;
    //os << "centroid:";
    for(list<Word>::const_iterator it = m_centroid.begin();
        it != m_centroid.end();
        ++it){
        os << it->id << ":" << it->v << ",";
    }
    // os << endl;
    // os << "docs:";
    // for (vector<const Doc*>::const_iterator it=m_docs.begin();
    //      it != m_docs.end();
    //      ++it)
    //     os << (*it)->id() << ",";
    return os.str();
}

//test
// int main() {
//     string s("37:1,38:1,39:1");
//     Doc* d = new Doc(0, s);
//     cout<< d->toString() <<endl;
//     Cluster c(1);
//     c.insert( d );
    
//     string s2("3:2,8:2,39:2");
//     Doc* d2 = new Doc(1, s2);
//     //c.insert( d2 );
//     cout << d2->toString() <<endl;
//     //    cout << c.toString() <<endl;
//     //    cout << c.distance(d2) <<endl;
//     cout << c.diameter(d2) <<endl;
//     delete d;
//     delete d2;
// }
