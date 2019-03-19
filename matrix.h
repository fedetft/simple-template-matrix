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

#pragma once

#include <tuple>
#include <initializer_list>
#include <stdexcept>
#include <ostream>

/**
 * A simple fully template matrix class.
 * 
 * You do not want to use MatrixBase directly, but rather use a using
 * declaration to specify its template paramenters:
 * \code
 * using Matrix3f = MatrixBase<float,3,3>;
 * 
 * Matrix3f myMatrix(0);
 * \endcode
 */
template<typename T, unsigned R, unsigned C>
class MatrixBase
{
    T m[R][C];
    
public:
    MatrixBase(const MatrixBase&) = default;
    MatrixBase& operator= (const MatrixBase&) = default;
    
    /**
     * Default constructor.
     * Leaves the matrix uninitialized for performace.
     */
    MatrixBase() {}
    
    /**
     * Construct a matrix filling every element with the same value
     * \param v value to fill the matrix
     * \code
     * Matrix3f myMatrix(0);
     * \endcode
     */
    explicit MatrixBase(T v)
    {
        for(unsigned r = 0; r < R; r++)
            for(unsigned c = 0; c < C; c++)
                m[r][c] = v;
    }
    
    /**
     * Construct a matrix from an initializer list
     * \param args initializer list. Its size must be equalt to R*C
     * \code
     * Matrix3f myMatrix
     * {
     *     1.f, 2.f, 3.f,
     *     4.f, 5.f, 6.f,
     *     7.f, 8.f, 9.f
     * };
     * \endcode
     */
    explicit MatrixBase(std::initializer_list<T> args)
    {
        if(args.size() != R*C)
            throw std::range_error("MatrixBase(initializer_list<T> v)");
        
        auto it = args.begin();
        for(unsigned r = 0; r < R; r++)
            for(unsigned c = 0; c < C; c++)
                m[r][c] = *it++;
    }
    
    /**
     * \return the number of rows of the matrix 
     */
    unsigned rows() const { return R; }
    
    /**
     * \return the number of columns of the matrix 
     */
    unsigned cols() const { return C; }
    
    /**
     * \return the size of the matrix 
     * \code
     * auto sz = myMatrix.size();
     * unsigned rows = get<0>(sz);
     * unsigned cols = get<1>(sz);
     * \endcode
     */
    std::tuple<unsigned,unsigned> size() const { return std::make_tuple(R,C); }
    
    /**
     * Access an element (read-only) (without bound checks)
     * \param r row    (from 0 to R-1)
     * \param c column (from 0 to C-1)
     * \return the corresponding element
     * \code
     * float f = myMatrix(0,0);
     * \endcode
     */
    T operator() (unsigned r, unsigned c) const { return m[r][c]; }
    
    /**
     * Access an element (read-write) (without bound checks)
     * \param r row    (from 0 to R-1)
     * \param c column (from 0 to C-1)
     * \return the corresponding element
     * \code
     * myMatrix(0,0) = 1.0f;
     * \endcode
     */
    T& operator() (unsigned r, unsigned c) { return m[r][c]; }
    
    /**
     * Access an element (read-only) (with bound checks)
     * \param r row    (from 0 to R-1)
     * \param c column (from 0 to C-1)
     * \return the corresponding element
     * \throws range_error if r and/or c are not in range
     * \code
     * float f = myMatrix.at(0,0);
     * \endcode
     */
    T at(unsigned r, unsigned c) const
    {
        if(r >= R || c >= C) throw std::range_error("MatrixBase::at()");
        return m[r][c];
    }
    
    /**
     * Access an element (read-write) (with bound checks)
     * \param r row    (from 0 to R-1)
     * \param c column (from 0 to C-1)
     * \return the corresponding element
     * \throws range_error if r and/or c are not in range
     * \code
     * myMatrix.at(0,0) = 1.0f;
     * \endcode
     */
    T& at(unsigned r, unsigned c)
    {
        if(r >= R || c >= C) throw std::range_error("MatrixBase::at()");
        return m[r][c];
    }
};

/**
 * \code
 * Matrix3f a(0);
 * cout << a << endl;
 * \endcode
 */
template<typename T, unsigned R, unsigned C>
std::ostream& operator<< (std::ostream& os, const MatrixBase<T,R,C>& a)
{
    for(unsigned r = 0; r < R; r++)
    {
        for(unsigned c = 0; c < C; c++)
            os << a(r,c) <<' ';
        os << '\n';
    }
    return os;
}

/**
 * \code
 * Matrix3f a(0), b(0);
 * b = transpose(a);
 * \endcode
 */
template<typename T, unsigned R, unsigned C>
MatrixBase<T,C,R> transpose(const MatrixBase<T,R,C>& a)
{
    MatrixBase<T,C,R> result;
    for(unsigned r = 0; r < R; r++)
        for(unsigned c = 0; c < C; c++)
            result(c,r) = a(r,c);
    return result;
}

/**
 * \code
 * Matrix3f a(0), b(0), c(0);
 * c = a + b;
 * \endcode
 */
template<typename T, unsigned R, unsigned C>
MatrixBase<T,R,C> operator+ (const MatrixBase<T,R,C>& a, const MatrixBase<T,R,C>& b)
{
    MatrixBase<T,R,C> result;
    for(unsigned r = 0; r < R; r++)
        for(unsigned c = 0; c < C; c++)
            result(r,c) = a(r,c) + b(r,c);
    return result;
}

/**
 * \code
 * Matrix3f a(0), c(0);
 * float b = 0;
 * c = a + b;
 * \endcode
 */
template<typename T, unsigned R, unsigned C>
MatrixBase<T,R,C> operator+ (const MatrixBase<T,R,C>& a, T b)
{
    MatrixBase<T,R,C> result;
    for(unsigned r = 0; r < R; r++)
        for(unsigned c = 0; c < C; c++)
            result(r,c) = a(r,c) + b;
    return result;
}

/**
 * \code
 * float a = 0;
 * Matrix3f b(0), c(0);
 * c = a + b;
 * \endcode
 */
template<typename T, unsigned R, unsigned C>
MatrixBase<T,R,C> operator+ (T a, const MatrixBase<T,R,C>& b)
{
    MatrixBase<T,R,C> result;
    for(unsigned r = 0; r < R; r++)
        for(unsigned c = 0; c < C; c++)
            result(r,c) = a + b(r,c);
    return result;
}

/**
 * \code
 * Matrix3f a(0), b(0);
 * a += b;
 * \endcode
 */
template<typename T, unsigned R, unsigned C>
void operator+= (MatrixBase<T,R,C>& a, const MatrixBase<T,R,C>& b)
{
    for(unsigned r = 0; r < R; r++)
        for(unsigned c = 0; c < C; c++)
            a(r,c) += b(r,c);
}

/**
 * \code
 * Matrix3f a(0);
 * float b = 0;
 * a += b;
 * \endcode
 */
template<typename T, unsigned R, unsigned C>
void operator+= (MatrixBase<T,R,C>& a, T b)
{
    for(unsigned r = 0; r < R; r++)
        for(unsigned c = 0; c < C; c++)
            a(r,c) += b;
}

/**
 * \code
 * Matrix3f a(0), b(0), c(0);
 * c = a - b;
 * \endcode
 */
template<typename T, unsigned R, unsigned C>
MatrixBase<T,R,C> operator- (const MatrixBase<T,R,C>& a, const MatrixBase<T,R,C>& b)
{
    MatrixBase<T,R,C> result;
    for(unsigned r = 0; r < R; r++)
        for(unsigned c = 0; c < C; c++)
            result(r,c) = a(r,c) - b(r,c);
    return result;
}

/**
 * \code
 * Matrix3f a(0), c(0);
 * float b = 0;
 * c = a - b;
 * \endcode
 */
template<typename T, unsigned R, unsigned C>
MatrixBase<T,R,C> operator- (const MatrixBase<T,R,C>& a, T b)
{
    MatrixBase<T,R,C> result;
    for(unsigned r = 0; r < R; r++)
        for(unsigned c = 0; c < C; c++)
            result(r,c) = a(r,c) - b;
    return result;
}

/**
 * \code
 * float a = 0;
 * Matrix3f b(0), c(0);
 * c = a - b;
 * \endcode
 */
template<typename T, unsigned R, unsigned C>
MatrixBase<T,R,C> operator- (T a, const MatrixBase<T,R,C>& b)
{
    MatrixBase<T,R,C> result;
    for(unsigned r = 0; r < R; r++)
        for(unsigned c = 0; c < C; c++)
            result(r,c) = a - b(r,c);
    return result;
}

/**
 * \code
 * Matrix3f a(0), b(0);
 * a -= b;
 * \endcode
 */
template<typename T, unsigned R, unsigned C>
void operator-= (MatrixBase<T,R,C>& a, const MatrixBase<T,R,C>& b)
{
    for(unsigned r = 0; r < R; r++)
        for(unsigned c = 0; c < C; c++)
            a(r,c) -= b(r,c);
}

/**
 * \code
 * Matrix3f a(0);
 * float b = 0;
 * a -= b;
 * \endcode
 */
template<typename T, unsigned R, unsigned C>
void operator-= (MatrixBase<T,R,C>& a, T b)
{
    for(unsigned r = 0; r < R; r++)
        for(unsigned c = 0; c < C; c++)
            a(r,c) -= b;
}

/**
 * \code
 * Matrix3f a(0), b(0), c(0);
 * c = a * b;
 * \endcode
 */
template<typename T, unsigned R1, unsigned C1R2, unsigned C2>
MatrixBase<T,R1,C2> operator* (const MatrixBase<T,R1,C1R2>& a, const MatrixBase<T,C1R2,C2>& b)
{
    MatrixBase<T,R1,C2> result;
    for(unsigned r = 0; r < R1; r++)
    {
        for(unsigned c = 0; c < C2; c++)
        {
            result(r,c) = 0;
            for(unsigned x = 0; x < C1R2; x++) result(r,c) += a(r,x) * b(x,c);
        }
    }
    return result;
}

/**
 * \code
 * float b = 0;
 * Matrix3f a(0), c(0);
 * c = a * b;
 * \endcode
 */
template<typename T, unsigned R, unsigned C>
MatrixBase<T,R,C> operator* (const MatrixBase<T,R,C>& a, T b)
{
    MatrixBase<T,R,C> result;
    for(unsigned r = 0; r < R; r++)
        for(unsigned c = 0; c < C; c++)
            result(r,c) = a(r,c) * b;
    return result;
}

/**
 * \code
 * float a;
 * Matrix3f b(0), c(0);
 * c = a * b;
 * \endcode
 */
template<typename T, unsigned R, unsigned C>
MatrixBase<T,R,C> operator* (T a, const MatrixBase<T,R,C>& b)
{
    MatrixBase<T,R,C> result;
    for(unsigned r = 0; r < R; r++)
        for(unsigned c = 0; c < C; c++)
            result(r,c) = a * b(r,c);
    return result;
}

/**
 * \code
 * Matrix3f a(0), b(0);
 * a *= b;
 * \endcode
 */
template<typename T, unsigned R1, unsigned C1R2C2>
void operator*= (MatrixBase<T,R1,C1R2C2>& a, const MatrixBase<T,C1R2C2,C1R2C2>& b)
{
    MatrixBase<T,R1,C1R2C2> temp;
    for(unsigned r = 0; r < R1; r++)
    {
        for(unsigned c = 0; c < C1R2C2; c++)
        {
            temp(r,c) = 0;
            for(unsigned x = 0; x < C1R2C2; x++) temp(r,c) += a(r,x) * b(x,c);
        }
    }
    a = temp;
}

/**
 * \code
 * Matrix3f a(0);
 * float b = 0;
 * a *= b;
 * \endcode
 */
template<typename T, unsigned R, unsigned C>
void operator*= (MatrixBase<T,R,C>& a, T b)
{
    for(unsigned r = 0; r < R; r++)
        for(unsigned c = 0; c < C; c++)
            a(r,c) *= b;
}

/**
 * Determinant of 2x2 matrix
 * \code
 * Matrix2f a{1,2,3,4};
 * float d = det(a);
 * \endcode
 */
template<typename T>
T det(const MatrixBase<T,2,2>& a)
{
    return a(0,0)*a(1,1)-a(0,1)*a(1,0);
}

/**
 * Determinant of 3x3 matrix
 * \code
 * Matrix2f a{1,2,3,4,5,6,7,8,9};
 * float d = det(a);
 * \endcode
 */
// template<typename T>
// T det(const MatrixBase<T,3,3>& a)
// {
//     //TODO
// }

/**
 * Inverse of 2x2 matrix
 * \code
 * Matrix2f a{1,2,3,4};
 * auto b = inv(a);
 * \endcode
 */
template<typename T>
MatrixBase<T,2,2> inv(const MatrixBase<T,2,2>& a)
{
    T d = det(a);
    if(d == 0) throw std::runtime_error("matrix singular");
    return (1/d)*MatrixBase<T,2,2>{
         a(1,1), -a(0,1),
        -a(1,0),  a(0,0)
    };
}

/**
 * Inverse of 3x3 matrix
 * \code
 * Matrix2f a{1,2,3,4,5,6,7,8,9};
 * auto b = inv(a);
 * \endcode
 */
// template<typename T>
// MatrixBase<T,3,3> inv(const MatrixBase<T,3,3>& a)
// {
//     //TODO
// }

/**
 * \code
 * float b = 0;
 * Matrix3f a(0), c(0);
 * c = a / b;
 * \endcode
 */
template<typename T, unsigned R, unsigned C>
MatrixBase<T,R,C> operator/ (const MatrixBase<T,R,C>& a, T b)
{
    MatrixBase<T,R,C> result;
    for(unsigned r = 0; r < R; r++)
        for(unsigned c = 0; c < C; c++)
            result(r,c) = a(r,c) / b;
    return result;
}

/**
 * \code
 * Matrix3f a(0);
 * float b = 0;
 * a /= b;
 * \endcode
 */
template<typename T, unsigned R, unsigned C>
void operator/= (MatrixBase<T,R,C>& a, T b)
{
    for(unsigned r = 0; r < R; r++)
        for(unsigned c = 0; c < C; c++)
            a(r,c) /= b;
}
