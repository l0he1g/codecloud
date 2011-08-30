 // record clusters by each feature
// see Context-aware query suggestion by mining click-through and sesssion data, kdd08
#ifndef DIMARRAY_H_
#define DIMARRAY_H_
#include <vector>
#include <list>
#include "Cluster.h"

class DimArray{
  public:
    void resize(long size){dim.resize(size);}

    bool empty() const {return dim.empty();}
    //    long size() {return dim.size();}
    long size() const {return dim.size();}
    list<Cluster*> at(long i) const {return dim[i];}
    
    void add(Cluster* c) {
        const list<Word>& words = c->get_centroid();
        for(list<Word>::const_iterator it = words.begin();
            it != words.end();
            ++it) {
            dim[it->id].push_back(c);
        }
    }

    // add cluster to one dimension
    void add(long i, Cluster* c) {
        if ( i > dim.size() )
            return;
        dim[i].push_back(c);
    }

    // for test
    void print() {
        for(long i=0; i < dim.size(); ++i){
            printf("%ld\t", i);
            for(list<Cluster*>::iterator it = dim[i].begin();
                it != dim[i].end();
                ++it) {
                printf("%ld ", (*it)->id());
            }
            printf("\n");
        }
    }
    
  private:
    vector< list<Cluster*> > dim;
};
#endif
