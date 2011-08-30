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

template<class T>
class Matrix
{
private:
    T** array;
    int m_m;
    int m_n;
    
public:
    Matrix( int M, int N, int init_method=0 ):m_m(M),m_n(N)
    {
        array = new T*[m_m];
        for( int i = 0; i < m_m; i++ )
            array[i] = new T[m_n];

        // initialize elements
        if( init_method!=0 )
            rand_init();
        else
            clean();
    }

    void clean()
    {
        for(int i=0;i<m_m;++i)
            for(int j=0;j<m_n;++j)
                array[i][j] = 0;
    }
    
    Matrix( const Matrix<T>& ma ){
        m_m = ma.rows();
        m_n = ma.cols();
        array = new T*[m_m];
        for( int i = 0; i < m_m; i++ )
            array[i] = new T[m_n];

        for(int i=0;i<m_m;++i)
            for(int j=0;j<m_n;++j)
                array[i][j] = ma[i][j];
    }

    // type convert
    template<class U>
    Matrix( const Matrix<U>& ma )
    //    Matrix<T> & operator=(const Matrix<U> &ma)
    {
        m_m = ma.rows();
        m_n = ma.cols();
        array = new T*[m_m];
        for( int i = 0; i < m_m; i++ )
            array[i] = new T[m_n];

        for(int i=0;i<m_m;++i)
            for(int j=0;j<m_n;++j)
                array[i][j] = (T)ma[i][j];
    }
    
    Matrix<T> & operator=(const Matrix<T> &ma)
    {
        assert(m_m == ma.rows());
        assert(m_n == ma.cols());
        for(int i=0;i<m_m;++i)
            for(int j=0;j<m_n;++j)
                array[i][j] = ma[i][j];
        return *this;
    }

    Matrix<T>& operator +=(const Matrix<T> &ma)
    {
        assert(m_m == ma.rows());
        assert(m_n == ma.cols());
        for(int i=0;i<m_m;++i)
            for(int j=0;j<m_n;++j)
                array[i][j] += ma[i][j];
        return *this;
    }


//    const int rows() const {return sizeof(array)/sizeof(array[0]);}
//    const int cols() const {return rows()?sizeof(array[0])/sizeof(array[0][0]):0;}
    const int rows() const {return m_m;}
    const int cols() const {return m_n;}

    T* &operator[] ( int m ){return array[m];}
    T* const &operator[] ( int m ) const {return array[m];}

// sum of all elements
    T sum()
    {
        T s=0;
        for( int i = 0 ; i < rows() ; ++i )
            for(int j=0; j< cols() ; ++j)
                s += array[i][j];
        return s;
    }


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

    ~Matrix()
    {
        for(int i=0;i<rows();i++)
           delete []array[i];
        delete []array;
    }
    
private:
    void rand_init()
    {
        srand( time(NULL) );
//        srand( 1 );
        for( int i = 0 ; i < rows() ; ++i )
        {
            for(int j=0; j< cols() ; ++j)
            {
                T tmp = (T)((rand() % 10000000) / 10000000.0);
                while( tmp <=0 )
                    tmp = (T)((rand() % 10000000) / 10000000.0);
                array[i][j] = tmp;
                assert( array[i][j]>0 );
            }
        }
    }
};

#endif

