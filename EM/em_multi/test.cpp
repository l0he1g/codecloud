#include "Matrix.h"
#include <iostream>
#include <gtest/gtest.h>

using namespace std;

TEST(Matrix, create) 
{
    Matrix<double> m( 3, 4, 1);
//    ASSERT_TRUE( m!=NULL );
//    EXPECT_EQ( m.rows(), 3);
//    EXPECT_EQ( m.cols(), 4);
//    EXPECT_GT( m[0,0], 0);
//    cout<<m[0][0]<<endl;
//    EXPECT_GT( m[2,3], 0);
//    cout<<m[2][3]<<endl;
//    m[2][3] += m[2][3];
    cout<<m[2][3]<<endl;

    Matrix<double> mm(3,4,1);
    cout<<mm[2][3]<<endl;

    mm+=m;
    cout<<mm[2][3]<<endl;
    mm.clean();
    cout<<mm[2][3]<<endl;
     // normalize
//     m->normalize();
//     EXPECT_FLOAT_EQ(m->sum(),1);
     
//     delete m;
 }


