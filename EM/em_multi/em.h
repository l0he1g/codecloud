#ifndef EM_H
#define EM_H
#include <vector>
#include <iostream>
#include "Matrix.h"
using namespace std;

struct triple
{
    int qr,u,qt;
    float v;

    triple(){qr=0;qt=0;u=0;v=0;}
    triple(int i, int j, int k):qr(i),u(j),qt(k){v=0;}
};

int I=2;                   // dimension of I1
int J=2;                  // dimension of I2
int n_thread = 1;
int R,U,T;

vector<triple> nz;

class EM
{
public:
    Matrix<float> &p_i_j;
    Matrix<float> &p_r_i;
    Matrix<float> &p_u_i;
    Matrix<float> &p_t_j;
    const vector<triple> &data;

    EM(const vector<triple> &d,Matrix<float> &pij, Matrix<float> &pri, Matrix<float> &pui, Matrix<float> &ptj):
        data(d), p_i_j(pij), p_r_i(pri), p_u_i(pui), p_t_j(ptj)
    {}
    
// optimization by combining E and M steps: store result in E direct into M
    void operator()()
    {
//    cout<<"thread "<<boost::this_thread::get_id()<<" start for "<<data.size()<<" triples."<<endl;
        Matrix<double> m(I,J);
        Matrix<float> pij(I,J),pri(I,R),pui(I,U),ptj(J,T); // store M-step result
        for(vector<triple>::const_iterator it=data.begin();it!=data.end();++it){
            for(int i=0;i<I;++i){
                for(int j=0;j<J;++j){
                    m[i][j] = ((double)p_i_j[i][j])*p_r_i[i][it->qr]*p_u_i[i][it->u]*p_t_j[j][it->qt]*it->v;
                }
            }
            m.normalize();
            // convert m to float
            Matrix<float> mm(m);
            // combine to four matrix for m_step
            for(int i=0;i<I;++i){
                for(int j=0;j<J;++j){
                    pij[i][j] += mm[i][j];
                    pri[i][it->qr] += mm[i][j];
                    pui[i][it->u] += mm[i][j];
                    ptj[j][it->qt] += mm[i][j];
                }
            }
            // clean m m.clear();
        }

        p_i_j = pij;
        p_r_i = pri;
        p_u_i = pui;
        p_t_j = ptj;
//    cout<<"thread "<<boost::this_thread::get_id()<<" finished"<<endl;
    }
};

    
#endif
