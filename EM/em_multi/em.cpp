/*
 EM for click-based context-aware query recommendation model
 Input: voca_triples_cate.txt   <qr,cate,qt,score>
        I,J,R,T,U     is the lines voca_* file, must set manualy
 Output: I_J/
        p_i_j.txt   <i,j>:p     a matrix represents p(i,j)
        p_r_i.txt   <i,qr>:p    a matrix represetns p(qr|i),each row with the same i
        p_u_i.txt   <i,u>:p     a matrix represetns p(u|i),each row with the same i
        p_t_j.txt   <j,t>:p     a matrix represetns p(qt|j),each row with the same j
 */
#include <fstream>
#include <cassert>
#include <cstdlib>              // for system
#include <string>
#include <cmath>
#include <cstdio>
#include <ctime>
#include <boost/thread/thread.hpp>
#include <boost/bind.hpp>
#include <unistd.h>             // for getopt
#include "em.h"

#define TIME_TEST(cmd) {time_t s_time=time(NULL);               \
        cmd;                                    \
        time_t e_time=time(NULL);               \
        printf("\t%s cost %d s\n",#cmd,(int)difftime(e_time,s_time));}

#define CLOCK_TEST(cmd) {                       \
        clock_t start, end;                     \
        double   duration;                      \
        start = clock();                        \
        cmd;                                    \
        end = clock();                                                  \
        printf("\t%s cost %f s\n",#cmd,(double)(end - start)/CLOCKS_PER_SEC);}


int file_lines( const char* path )
{
    ifstream rf( path );
    int i=0;
    string line;
    while(getline(rf, line))
        ++i;
    return i;
}

// read qr,u,qt,v from file
void read_triple( const char* path,vector<triple>& nz )
{
    ifstream rf( path );
    string line;
    while(getline(rf, line))
    {
        triple t;
        sscanf( line.c_str(),"%d\t%d\t%d\t%f",&t.qr,&t.u,&t.qt,&t.v );
        nz.push_back( t );
    }
}

void random_init(Matrix<float> &p_i_j,Matrix<float> &p_r_i,Matrix<float> &p_u_i,Matrix<float> &p_t_j)
{
    p_i_j.normalize();
    p_r_i.normalize_row();
    p_u_i.normalize_row();
    p_t_j.normalize_row();
}

void combine( vector<Matrix<float> > &v, Matrix<float> &p)
{
    p.clean();
    for(vector<Matrix<float> >::iterator it=v.begin();
        it!=v.end();
        ++it)
    {
        p += *it;
     }
}


void EM_multi(const vector< vector<triple> > &data,
              Matrix<float> &p_i_j,
              Matrix<float> &p_r_i,
              Matrix<float> &p_u_i,
              Matrix<float> &p_t_j
    )
{
    vector< Matrix<float> > v_ij, v_ri, v_ui, v_tj;
    for(int i=0;i<n_thread;++i)
    {
        Matrix<float> t_ij(p_i_j), t_ri(p_r_i), t_ui(p_u_i), t_tj(p_t_j);
        v_ij.push_back(t_ij);
        v_ri.push_back(t_ri);
        v_ui.push_back(t_ui);
        v_tj.push_back(t_tj);
    }
    // begine multi-thread
    boost::thread_group grp;
    for(int i=0;i<n_thread;++i)
    {
        EM em( data[i],v_ij[i],v_ri[i],v_ui[i],v_tj[i] );
        boost::thread *th = new boost::thread( em );
        grp.add_thread(th);
    }
    
    grp.join_all();
    // reduce all ther result
    combine( v_ij, p_i_j );
    combine( v_ri, p_r_i );
    combine( v_ui, p_u_i );
    combine( v_tj, p_t_j );
    
    p_i_j.normalize();
    p_r_i.normalize_row();
    p_u_i.normalize_row();
    p_t_j.normalize_row();
    // clean all result vector---add into combine()
}
// Note:this is likelihood of observed data,not excepted of complete data
// log_likelihood=sum_{qr,u,qt}{n(qr,u,qt)log{sum_{Ir,It}{p(Ir,It)p(qr|Ir)p(u|Ir)p(qt|It)}}}
float log_likelihood(const Matrix<float> &p_i_j,const Matrix<float> &p_r_i,const Matrix<float> &p_u_i,const Matrix<float> &p_t_j)
{
    float ll=0;
    for(vector<triple>::iterator it=nz.begin();it!=nz.end();++it)
    {
        double p=0;
        // sum joint p on Ir,It
        for(int i=0;i<I;++i)
            for(int j=0;j<J;++j)
                p += p_i_j[i][j]*p_r_i[i][it->qr]*p_u_i[i][it->u]*p_t_j[j][it->qt];

        ll += it->v*(float)log10(p);
    }

    return ll;
}

void write(const Matrix<float> &p_i_j,const Matrix<float> &p_r_i,const Matrix<float> &p_u_i,const Matrix<float> &p_t_j)
{
    // mkdir I_J
    char dir[20];
    char cmd[30];
    sprintf(dir,"%d_%d",I,J);
    sprintf(cmd,"mkdir %s",dir);
    system(cmd);

    string path(dir);
    // write p_i_j,row:Ir  col:It
    string p1 = path + "/p_i_j.txt";
    ofstream wf(p1.c_str());
    for(int i=0;i<I;++i)
    {
        for(int j=0; j<J-1; ++j)
            wf<<p_i_j[i][j]<<",";
        wf<<p_i_j[i][J-1]<<endl;
    }
    wf.close();
    cout<<"p_i_j has been writen: "<<p1<<endl;

// write p_r_i,row:I  col:qr
    string p2 = path + "/p_r_i.txt";
    wf.open(p2.c_str());
    for(int i=0;i<I;++i)
    {
        for(int j=0; j<R-1; ++j)
            wf<<p_r_i[i][j]<<","; // row:i  col:qr
        wf<<p_r_i[i][R-1]<<endl;
    }
    wf.close();
    cout<<"p_r_i has been writen: "<<p2<<endl;

    // write p_u_i,row:I  col:u
    string p3 = path + "/p_u_i.txt";
    wf.open(p3.c_str());
    for(int i=0;i<I;++i)
    {
        for(int j=0; j<U-1; ++j)
            wf<<p_u_i[i][j]<<","; // row:i  col:qr
        wf<<p_u_i[i][U-1]<<endl;
    }
    wf.close();
    cout<<"p_u_i has been writen: "<<p3<<endl;

    // write p_u_i,row:I  col:u
    string p4 = path + "/p_t_j.txt";
    wf.open(p4.c_str());
    for(int j=0;j<J;j++)
    {
        for(int t=0; t<T-1; ++t)
            wf<<p_t_j[j][t]<<","; // row:i  col:qr
        wf<<p_t_j[j][T-1]<<endl;
    }
    wf.close();
    cout<<"p_t_j has been writen: "<<p4<<endl;
}

int main(int argc,char* argv[])
{
  cout<<argc<<endl;
    switch ( argc )
    {
        case 2:
                sscanf(argv[1], "%d", &I );
                break;
        case 3:
                sscanf(argv[2], "%d", &J );
                break;
        case 4:
	  sscanf(argv[3], "%d", &n_thread );
	  break;
    default:
        cout<<"Usage:"<<argv[0]<<" [-i num] [-j num] [-t num]"<<endl;
        cout<<"option:"<<endl;
        cout<<"\t-i    dimension of I, default=10"<<endl;
        cout<<"\t-j    dimension of J, default=10"<<endl;
        cout<<"\t-t    num of threads, default=16"<<endl;
       exit(0);                  
    }

    // read triples
    const char* data_path = "test_data/voca_triples_cate.txt";
    cout<<"1. read triples <qr,u,qt> from :"<<data_path<<endl;
    string triple_path(data_path);
    read_triple( data_path ,nz );
    cout<<"\ttriples num: "<<nz.size()<<endl;
    
    cout<<endl<<"2. determine dimensions of variable"<<endl;
    R = file_lines("test_data/voca_qr.txt");
    T = file_lines("test_data/voca_qt.txt");
    U = file_lines("test_data/voca_cate.txt");
    cout<<"\tobserved: R="<<R<<" T="<<T<<" U="<<U<<endl;
    cout<<"\tlatent:   I="<<I<<" J="<<J<<endl;
    assert( R>=I && T>=I && U>=I && R>=J && T>=J && U>=J );
    
    cout<<endl<<"3. random init and EM"<<endl;
    Matrix<float> p_i_j(I,J,1), p_r_i(I,R,1), p_u_i(I,U,1), p_t_j(J,T,1);
    random_init(p_i_j,p_r_i,p_u_i,p_t_j);

    // split nz triples to n_thread parts
    int interval = (int)ceil( ((float)nz.size())/n_thread);
    vector< vector<triple> > data;
    for(int i=0;i<n_thread;++i)
    {
        int start = i*interval;
        int end = start+interval;
        end = end >nz.size()? nz.size():end;
        vector<triple> tmp(nz.begin()+start, nz.begin()+end);
        data.push_back( tmp );
    }

    cout<<"\tEM..."<<endl;

    TIME_TEST( EM_multi(data,p_i_j,p_r_i,p_u_i,p_t_j) );//mulit-thread
    float ol = log_likelihood(p_i_j,p_r_i,p_u_i,p_t_j);
    cout<<"1st log likelihood: "<<ol<<endl;
    
    // begin EM iteration
    int max_iter = 100;
    float eps = 0.01;
    float ll,dif_l;
    for(int ct=0;ct<max_iter;++ct)
    {
        TIME_TEST( EM_multi(data,p_i_j,p_r_i,p_u_i,p_t_j) );//mulit-thread
//        EM(nz,p_i_j,p_r_i,p_u_i,p_t_j);//single-thread
        ll = log_likelihood(p_i_j,p_r_i,p_u_i,p_t_j);
        dif_l = ll - ol;
        cout<<"[iter "<<ct<<"] log likelihood change: "<<ll<<" - "<<ol<<" = "<<dif_l<<endl;
        if( dif_l < eps )
        {
            cout<<"\tEM iteration end since no more progress."<<endl;
            break;
        }
        else
            ol = ll;
    }
        
    // write result
    cout<<endl<<"4. write result"<<endl;
    write(p_i_j,p_r_i,p_u_i,p_t_j);
}

