#ifndef CLUSTER_H
#define CLUSTER_H

#include <cassert>
#include <iostream>
#include <sstream>
#include <list>
#include <vector>
#include "string_util.h"
#include "Doc.h"

using namespace std;

// cluster is a vector of Words
class Cluster
{
  public:
    Cluster(long id):m_id(id) {}
    //    Cluster(long id,  Doc* doc);

    bool empty() const { return m_docs.empty();}
    
    long id()  const {return m_id;}
    long size() const {return m_docs.size();}
    
    const list<Word>& get_centroid() const {return m_centroid;}
    float distance (const Doc* d) const;
 
    // compute the diameter of C \uion d
    float diameter(const Doc* d) const;

    
    // insert into word list, keep increasing order
    void insert(const Doc* doc);

    string toString() const;

  private:
    float square_sum (const Doc* q1, const Doc* q2) const;
  private:
    long m_id;
    list<Word> m_centroid;
    vector<const Doc*> m_docs;
};

#endif
