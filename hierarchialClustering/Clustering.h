// main clustering process
#ifndef CLUATERING_H_
#define CLUSTERING_H_

#define EXIT_ERROR( s ) {printf("[Error] %s\n", s);\
        exit(EXIT_FAILURE);}
#define EXIT_ERROR2( s1, s2 ) {printf("[Error] %s%s\n", s1, s2);\
        exit(EXIT_FAILURE);}

#include <vector>
#include <iostream>
#include <fstream>

#include "Cluster.h"
#include "Config.h"
#include "DimArray.h"

using namespace std;
class Clustering {
  public:
    Clustering(const char* config_file): m_conf(config_file), m_threshold(-1){}
    
    // prepare data
    bool initialize();

    // main clustering process
    void run();

    // write clustering result
    void write_clusters();
    
    ~Clustering();    

  private:
    // read clusters into vector 'clusters' from data_pt
    // each line is a cluster:k:v,k:v
    void read_docs(const char* data_pt);

    // assign clusters to feature dimensionArray
    void init_dims();
  private:
    // minimal similarity for two cluster to concatenate a edge
    float m_threshold;   
    Config m_conf;
    vector<Cluster*> m_clusters;
    vector<Doc*> m_docs;
    DimArray m_dims;
};

#endif
