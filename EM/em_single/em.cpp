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
#include <iostream>
#include <fstream>
#include <cassert>
#include <cstdlib>              // for system
#include <string>
#include <cmath>
#include <vector>
#include <ctime>
#include "Matrix.h"
#include <cstdio>
using namespace std;

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

int I=2;                   // dimension of I1
int J=2;                  // dimension of I2
int R,U,T;

struct triple
{
    int qr,u,qt;
    float v;

    triple(){qr=0;qt=0;u=0;v=0;}
    triple(int i, int j, int k):qr(i),u(j),qt(k){v=0;}
};

vector<triple> nz;

// return dim of a tensor, d = 1,2,3
int tensor_dim(const vector<triple> & nz, int d )
{
    int m=-1;
    for( vector<triple>::const_iterator it=nz.begin();
         it!=nz.end();
         it++)
    {
        switch( d )
        {
            case 1:
                if(it->qr > m)
                    m = it->qr;
                break;
            case 2:
                if(it->u > m)
                    m = it->u;
                break;
            case 3:
                if(it->qt > m)
                    m = it->qt;
                break;
            default:
                cout<<"bad d:"<<d<<endl;
        }
    }
    return m+1;
}

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

// optimization by combining E and M steps: store result in E direct into M
void EM(Matrix<float> &p_i_j,Matrix<float> &p_r_i,Matrix<float> &p_u_i,Matrix<float> &p_t_j)
{
    Matrix<double> m(I,J);
    Matrix<float> pij(I,J),pri(I,R),pui(I,U),ptj(J,T); // store M-step result
    for(vector<triple>::iterator it=nz.begin();it!=nz.end();++it){
        for(int i=0;i<I;++i){
            for(int j=0;j<J;++j){
                m[i][j] = ((double)p_i_j[i][j])*p_r_i[i][it->qr]*p_u_i[i][it->u]*p_t_j[j][it->qt];
            }
        }
        m.normalize();
        // convert m to float
        Matrix<float> mm(m);
        cout<<"p(i,j|"<<it->qr<<","<<it->u<<","<<it->qt<<")"<<endl;
        mm.print();
        
        // combine to four matrix for m_step---TODO
        for(int i=0;i<I;++i){
            for(int j=0;j<J;++j){
                float tmp = it->v*mm[i][j];
                pij[i][j] += tmp;
                pri[i][it->qr] += tmp;
                pui[i][it->u] += tmp;
                ptj[j][it->qt] += tmp;
                printf("p(t=%d|j=%d) = %f * %f = %f\n", it->qt, j, it->v, mm[i][j], tmp);
            }
        }
        // clean m m.clear();
    }// for <r,u,t>
    p_i_j = pij.normalize();
    p_r_i = pri.normalize_row();
    p_u_i = pui.normalize_row();
    p_t_j = ptj.normalize_row();
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

void write_triples(const Matrix<float> &p_i_j,const Matrix<float> &p_r_i,const Matrix<float> &p_u_i,const Matrix<float> &p_t_j)
{
//    string p1 = path + "test_data/result.txt";
//    ofstream wf(p1.c_str());
    cout<<"p(r|i):IxR"<<endl;
    for(int i=0; i<I; ++i)
    {
        for(int r=0; r<R; ++r)
            cout<<p_r_i[i][r]<<"\t";
        cout<<endl;
    }
    cout<<endl<<"p(u|i):IxU"<<endl;
    for(int i=0; i<I; ++i)
    {
        for(int u=0; u<U; ++u)
            cout<<p_u_i[i][u]<<"\t";
        cout<<endl;
    }
    cout<<endl<<"p(t|j):JxT"<<endl;
    for(int j=0; j<J; ++j)
    {
        for(int t=0; t<T; ++t)
            cout<<p_t_j[j][t]<<"\t";
        cout<<endl;
    }
    cout<<endl<<"p(i,j):IxJ"<<endl;
    for(int i=0; i<I; ++i)
    {
        for(int j=0; j<J; ++j)
            cout<<p_i_j[i][j]<<"\t";
        cout<<endl;
    }
    cout<<endl<<"p(r,u,t):"<<endl;
    float s=0;
    for(int r=0;r<R;++r)
        for(int u=0;u<U;++u)
            for(int t=0;t<T;++t)
            {
                float tmp = 0;
                for(int i=0; i<I; ++i)
                    for(int j=0; j<J; ++j)
                        tmp += (p_i_j[i][j]*p_r_i[i][r]*p_u_i[i][u]*p_t_j[j][t]);
                if(tmp>0)
                {
                    cout<< r << '\t' << u << '\t' <<t << '\t' << tmp <<endl;
                    s+= tmp;
                }
            }
    cout<<"sum of p(r,u,t) = "<<s<<endl;
//    wf.close();
}


int main(int argc,char* argv[])
{
    switch ( argc )
    {
        case 1:
                break;
        case 2:
                sscanf(argv[1], "%d", &I );
                break;
        case 3:
                sscanf(argv[2], "%d", &J );
                break;
        default:
            cout<<"Usage:"<<argv[0]<<" [-i num] [-j num] [-t num]"<<endl;
            cout<<"option:"<<endl;
            cout<<"\t-i    dimension of I, default=10"<<endl;
            cout<<"\t-j    dimension of J, default=10"<<endl;
            exit(0);                  
    }
    
    // read triples
    const char* data_path = "test_data/test.txt";
    cout<<"1. read triples <qr,u,qt> from :"<<data_path<<endl;
    string triple_path(data_path);
    read_triple( data_path ,nz );
    cout<<"\ttriples num: "<<nz.size()<<endl;
    
    cout<<endl<<"2. determine dimensions of variable"<<endl;
/*    R = file_lines("test_data/voca_qr.txt");
    T = file_lines("test_data/voca_qt.txt");
    U = file_lines("test_data/voca_cate.txt");
*/
    R = tensor_dim( nz,1 );
    U = tensor_dim( nz,2 );
    T = tensor_dim( nz,3 );
    
    cout<<"\tobserved: R="<<R<<" T="<<T<<" U="<<U<<endl;
    cout<<"\tlatent:   I="<<I<<" J="<<J<<endl;
    assert( R>=I && T>=I && U>=I && R>=J && T>J );
    
    cout<<endl<<"3. random init and EM"<<endl;
    Matrix<float> p_i_j(I,J,1), p_r_i(I,R,1), p_u_i(I,U,1), p_t_j(J,T,1);
    random_init(p_i_j,p_r_i,p_u_i,p_t_j);
    float ol = log_likelihood(p_i_j,p_r_i,p_u_i,p_t_j);
    write_triples(p_i_j,p_r_i,p_u_i,p_t_j);
    
    cout<<"\tEM..."<<endl;
//    EM(p_i_j,p_r_i,p_u_i,p_t_j);
//    float ol = log_likelihood(p_i_j,p_r_i,p_u_i,p_t_j);
    cout<<"1st log likelihood: "<<ol<<endl;
    
    // begin EM iteration
    int max_iter = 5;
    float eps = 0.00000000;
    float ll,dif_l;
    for(int ct=0;ct<max_iter;++ct)
    {
        EM(p_i_j,p_r_i,p_u_i,p_t_j);
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
        write_triples(p_i_j,p_r_i,p_u_i,p_t_j);
    }
        
    // write result
    cout<<endl<<"4. write result"<<endl;
    //write(p_i_j,p_r_i,p_u_i,p_t_j);
    //write_triples(p_i_j,p_r_i,p_u_i,p_t_j);

}

