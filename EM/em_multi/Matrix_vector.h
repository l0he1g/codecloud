/*
 * Matrix.h
 *
 *  Created on: 2011-2-26
 *      Author: xh
 */
#ifndef MATRIX_H
#define MATRIX_H
#include <cassert>
#include <cstdlib>
#include <ctime>
#include <vector>
using std::vector;

template<class T>
class Matrix
{
public:
    Matrix( int M, int N, int init_method=0 ):array(M)
    {
        for( int i = 0; i < M; i++ )
            array[i].resize(N);

        // initialize elements
        if( init_method!=0 )
            rand_init();
    }

    // type convert
    template<class U>
    Matrix( const Matrix<U>& ma )
    //    Matrix<T> & operator=(const Matrix<U> &ma)
    {
        int M = ma.rows();
        int N = ma.cols();
        array.resize(M);
        for( int i=0; i<M; ++i )
        {
            array[i].resize(N);
            for( int j=0; j<N; ++j )
                array[i][j] = (T)ma[i][j];
        }
    }

    const int rows() const {return array.size();}
    const int cols() const {return rows()?array[0].size():0;}

    vector<T> &operator[] ( int m ){return array[m];}
    vector<T> const &operator[] ( int m ) const {return array[m];}

    T sum();                // sum of all elements

    Matrix<T>& normalize()
    {
        T s = this->sum();

        for( int i = 0 ; i < rows() ; ++i )
            for(int j=0; j< cols() ; ++j)
                array[i][j] /= s;
        return *this;
    }
// normalize elements in a row
    Matrix<T>& normalize_row()
    {
        for( int i = 0 ; i < rows() ; ++i )
        {
            T s=0;
            for(int j=0; j< cols() ; ++j)
                s += array[i][j];
            for(int j=0; j< cols() ; ++j)
                array[i][j] /= s;
        }
        return *this;
    }

private:
    void rand_init()
    {
        srand( time(NULL) );
        for( int i = 0 ; i < rows() ; ++i )
        {
            for(int j=0; j< cols() ; ++j)
            {
                array[i][j] = (T)((rand() % 10000000) / 10000000.0);
                assert( array[i][j]>0 );
            }
        }
    }
private:
//public:
    vector< vector<T> > array;
};

template<class T>
T Matrix<T>::sum()
{
    T s=0;
    for( int i = 0 ; i < rows() ; ++i )
        for(int j=0; j< cols() ; ++j)
            s += array[i][j];
    return s;
}

#endif

