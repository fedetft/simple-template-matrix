/*
 * Copyright (c) 2019, Terraneo Federico
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 *    * Redistributions of source code must retain the above copyright
 *      notice, this list of conditions and the following disclaimer.
 *    * Redistributions in binary form must reproduce the above copyright
 *      notice, this list of conditions and the following disclaimer in the
 *      documentation and/or other materials provided with the distribution.
 *    * Neither the name of the <organization> nor the
 *      names of its contributors may be used to endorse or promote products
 *      derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL <COPYRIGHT HOLDER> BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "matrix.h"
#include <cassert>
#include <cmath>
#include <iostream>

using namespace std;

using Matrix2f =  MatrixBase<float,2,2>;
using Matrix3f =  MatrixBase<float,3,3>;
using Matrix32f = MatrixBase<float,3,2>;
using Matrix23f = MatrixBase<float,2,3>;

template<unsigned R, unsigned C>
bool compare(const MatrixBase<float,R,C>& a, const MatrixBase<float,R,C>& b)
{
    float epsilon = 1e-9;
    
    for(unsigned r = 0; r < R; r++)
        for(unsigned c = 0; c < C; c++)
            if(fabs(a(r,c)-b(r,c))>epsilon) return false;
    return true;
}

int main()
{
    // Test scalar constructor and initializer_list constructor
    assert(compare(Matrix2f(1), Matrix2f{1,1,1,1}));
    
    Matrix32f a
    {
        1, 2,
        3, 4,
        5, 6
    };
    
    // Test rows(), cols(), size()
    assert(a.rows() == 3);
    assert(a.cols() == 2);
    auto sz = a.size();
    assert(get<0>(sz) == 3);
    assert(get<1>(sz) == 2);
    
    // test operator() and at()
    assert(a(0,0) == 1);
    assert(a(0,1) == 2);
    assert(a(1,0) == 3);
    assert(a(1,1) == 4);
    assert(a(0,0) == a.at(0,0));
    assert(a(0,1) == a.at(0,1));
    assert(a(1,0) == a.at(1,0));
    assert(a(1,1) == a.at(1,1));
    a(0,0) = 10;
    assert(a(0,0) == 10);
    a.at(0,0) = 1;
    assert(a(0,0) == 1);
    a(0,1) = 10;
    assert(a(0,1) == 10);
    a.at(0,1) = 2;
    assert(a(0,1) == 2);
    a(1,0) = 10;
    assert(a(1,0) == 10);
    a.at(1,0) = 3;
    assert(a(1,0) == 3);
    a(1,1) = 10;
    assert(a(1,1) == 10);
    a.at(1,1) = 4;
    assert(a(1,1) == 4);
    try {
        a.at(0,2) = 0;
        assert(false);
    } catch(range_error& e) {}
    try {
        a.at(3,0) = 0;
        assert(false);
    } catch(range_error& e) {}
    
    // Test transpose
    Matrix23f b = transpose(a);
    assert(compare(b, Matrix23f{1,3,5,2,4,6}));
    
    Matrix2f c
    {
        1, 2,
        3, 4
    };
    Matrix32f d;
    
    //Test operator+
    assert(compare(a + a, Matrix32f{2,4,6,8,10,12}));
    assert(compare(a + 1, Matrix32f{2,3,4,5,6,7}));
    assert(compare(1 + a, Matrix32f{2,3,4,5,6,7}));
    
    //Test operator+=
    d = a;
    d += a;
    assert(compare(d, Matrix32f{2,4,6,8,10,12}));
    d = a;
    d += 1;
    assert(compare(d, Matrix32f{2,3,4,5,6,7}));
    
    //Test operator-
    assert(compare(a - a, Matrix32f(0)));
    assert(compare(a - 1, Matrix32f{0,1,2,3,4,5}));
    assert(compare(1 - a, Matrix32f{0,-1,-2,-3,-4,-5}));
    
    //Test operator-=
    d = a;
    d -= a;
    assert(compare(d, Matrix32f(0)));
    d = a;
    d -= 1;
    assert(compare(d, Matrix32f{0,1,2,3,4,5}));
    
    //Test operator*
    assert(compare(a * c, Matrix32f{7,10,15,22,23,34}));
    assert(compare(a * 2, Matrix32f{2,4,6,8,10,12}));
    assert(compare(2 * a, Matrix32f{2,4,6,8,10,12}));
    
    //Test operator*=
    d = a;
    d *= c;
    assert(compare(d, Matrix32f{7,10,15,22,23,34}));
    d = a;
    d *= 2;
    assert(compare(d, Matrix32f{2,4,6,8,10,12}));
    
    //Test det()
    assert(det(c) == -2);
    
    //Test inv()
    assert(compare(inv(c), Matrix2f{-2,1,1.5,-0.5}));
    
    //Test operator/
    assert(compare(a / 2, Matrix32f{0.5,1,1.5,2,2.5,3}));
    
    //Test operator/=
    d = a;
    d /= 2;
    assert(compare(d, Matrix32f{0.5,1,1.5,2,2.5,3}));

    cout<<"Tests passed."<<endl;
    return 0;
}
