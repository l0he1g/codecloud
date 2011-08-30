#include <cstdlib>
#include <cassert>
#include <set>
#include "Clustering.h"

bool Clustering::initialize() {
	if (!m_conf.parse()) 
		EXIT_ERROR("Parse error in config.in!");

    m_threshold = atof(m_conf.get("threshold").c_str());
    if (m_threshold == -1)
        EXIT_ERROR("threshold is not set in config.in!");
    
    string data_pt = m_conf.get("data_pt");
    if (data_pt.empty()) 
		EXIT_ERROR("data_pt is not set in config.in!");
        
    read_docs(data_pt.c_str());
    if (m_docs.empty())
        EXIT_ERROR2("no queries read from ", data_pt.c_str());
    cout << "read queries, size = " << m_docs.size() <<endl;

    init_dims();
    if (m_dims.empty())
        EXIT_ERROR("Bad init cluster data format, feature dimension is zero!");
    cout << "DimArray initialized, size = " << m_dims.size() <<endl;
    
    return true;
}

void Clustering::read_docs(const char* data_pt) {
    ifstream inf(data_pt);
	if (!inf) 
		EXIT_ERROR2("Cannot open initial clusters file: ", data_pt);

    string line;
    long i = 0;
    while (getline(inf, line)) {
		util::trim(line);
		if (line.empty()) {
            cout<<i<<endl;
            continue;
        }
        Doc* c = new Doc(i, line);
        m_docs.push_back(c);
        ++i;
    }
}

void Clustering::init_dims() {
    long dim_size = atol( m_conf.get("feature_dimension").c_str() );
    m_dims.resize(dim_size);
}

void Clustering::run() {
    // one traversal
    int n_docs = m_docs.size();
    int counter = 0;
    for (vector<Doc*>::iterator it = m_docs.begin();
         it != m_docs.end();
         ++it) {
       // cout<< (*it)->id() <<"\t" << (*it)->toString()<<endl;
        // collect neighbor clusters set
        cout<<1;
        const vector<Word>& words = (*it)->get_words();
        set<Cluster*> nc;
        for (vector<Word>::const_iterator p = words.begin();
             p != words.end();
             ++p) {
            list<Cluster*> clusters = m_dims.at(p->id);
            for (list<Cluster*>::iterator pc = clusters.begin();
                 pc != clusters.end();
                 ++pc) {
                nc.insert(*pc);
            }// end for pc
        }// end for p
        cout<< "doc " << counter << "\t neighbours: "<<nc.size()<<endl;
        ++counter;
        // find the closest cluster for doc by utilizing dimAarray
        float min_d = 2;
        Cluster* min_c = NULL;          // record the nearest cluster of *it
        for (set<Cluster*>::iterator ps = nc.begin();
             ps != nc.end();
             ++ps ) {
            cout<< "\t" << (*ps)->get_centroid().size() <<endl;
            float cur_d = (*ps)->distance(*it);
            if (cur_d < min_d) {
                min_d = cur_d;
                min_c = *ps;
            }
        }
        cout<<2;
        if (min_c == NULL || min_c->diameter(*it) > m_threshold) {
            // new cluster
            min_c = new Cluster(m_clusters.size());
            min_c->insert(*it);
            m_clusters.push_back( min_c );
            
            // update dimAarray
            m_dims.add(min_c);
        }
        else {
            // insert it in to min_c
            min_c->insert(*it);

            // update dimAarray for words in doc but not in cluster
            // the centroid and doc must keep word in ascending order
            const list<Word>& cluster_words = min_c->get_centroid();
            const vector<Word>& doc_words = (*it)->get_words();
            vector<Word>::const_iterator p_dw = doc_words.begin();
            list<Word>::const_iterator p_cw = cluster_words.begin();
            while (p_dw != doc_words.end()) {
                while (p_cw != cluster_words.end() && p_cw->id < p_dw->id)
                    ++p_cw;
                if (p_cw == cluster_words.end() || p_cw->id != p_dw->id)
                    m_dims.add(p_dw->id, min_c);
                ++p_dw;
            }
        }
        cout<<3<<endl;
    } // end for it
}

void Clustering::write_clusters() {
    string path = m_conf.get("result_pt");
    if (path.empty()) {
        path = "result.txt";
        cout <<"[Warn] resutl_pt is not set in config.in! ";
    }
    cout << "clustering result write to: " << path <<endl;

    ofstream wf(path.c_str());
    for (vector<Cluster*>::iterator it = m_clusters.begin();
         it != m_clusters.end();
         ++it) {
        if ((*it)->id() != -1)
            //cout << (*it)->toString();  // for test
            wf << (*it)->toString() <<endl;  // for test
    }
    wf.close();
}
  
Clustering::~Clustering() {
    // delete docs
    for (vector<Doc*>::iterator it = m_docs.begin();
         it != m_docs.end();
         ++it) 
        delete (*it);
    
    // delete vector<cluster*>
    for (vector<Cluster*>::iterator it = m_clusters.begin();
         it != m_clusters.end();
         ++it) {
        delete (*it);
    }
}
