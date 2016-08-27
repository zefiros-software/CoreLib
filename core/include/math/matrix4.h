/**
 * @cond ___LICENSE___
 *
 * Copyright (c) 2016 Koen Visscher, Paul Visscher and individual contributors.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 *
 * @endcond
 */

#pragma once
#ifndef __ENGINE_MATRIX4_H__
#define __ENGINE_MATRIX4_H__

#include "math/matrix3.h"
#include "math/vec4.h"

class Vec3;
class Quaternion;

class Matrix4
{
    friend Matrix4 operator+( const Matrix4 &m1, const Matrix4 &m2 );
    friend Matrix4 operator-( const Matrix4 &m1, const Matrix4 &m2 );
    friend Matrix4 operator*( const Matrix4 &m1, const Matrix4 &m2 );
    friend Matrix4 operator*( const Matrix4 &m, const F32 s );
    friend Vec4 operator*( const Matrix4 &m, const Vec4 &v );

public:

    Matrix4();

    Matrix4( const F32 a11, const F32 a12, const F32 a13, const F32 a14,
             const F32 a21, const F32 a22, const F32 a23, const F32 a24,
             const F32 a31, const F32 a32, const F32 a33, const F32 a34,
             const F32 a41, const F32 a42, const F32 a43, const F32 a44 );

    Matrix4( const Vec4 &v1, const Vec4 &v2, const Vec4 &v3, const Vec4 &v4 );

    Matrix4( const Matrix4 &other );

    Matrix4 &operator=( const Matrix4 &m );

    Matrix4 &operator*=( const Matrix4 &m );

    Matrix4 &operator+=( const Matrix4 &m );

    Matrix4 &operator-=( const Matrix4 &m );

    bool operator==( const Matrix4 &m ) const;

    bool operator!=( const Matrix4 &m ) const;

    const Vec4 &operator[]( const U8 row ) const;

    void SetColumn( const U8 column, const Vec4 &v );

    Vec4 GetColumn( const U8 column ) const;

    void SetRow( const U8 row, const Vec4 &v );

    Vec4 GetRow( const U8 row ) const;

    Matrix4 Scale( const Vec4 &v ) const;

    Matrix4 GetTranspose() const;

    Matrix4 GetAbsolute() const;

    Matrix4 GetAdjoint() const;

    Matrix4 GetInverse() const;

    Matrix3 Extract3x3() const;

    void Compose( const Vec3 &scaling, const Quaternion &rotation, const Vec3 &translation );
    void Decompose( Vec3 &scaling, Quaternion &rotation, Vec3 &translation ) const;

    void SetIdentity();

    F32 GetCofactor( const U8 row, const U8 column ) const;

    F32 GetDeterminant() const;

    void SetValue( const F32 a11, const F32 a12, const F32 a13, const F32 a14,
                   const F32 a21, const F32 a22, const F32 a23, const F32 a24,
                   const F32 a31, const F32 a32, const F32 a33, const F32 a34,
                   const F32 a41, const F32 a42, const F32 a43, const F32 a44 );

    static Matrix4 GetZero();

    static Matrix4 GetIdentity();

private:

    Vec4 mValues[4];

    inline F32 Dotx( const Vec4 &v ) const
    {
        return mValues[0][0] * v[0] + mValues[1][0] * v[1] + mValues[2][0] * v[2] + mValues[3][0] * v[3];
    }

    inline F32 Doty( const Vec4 &v ) const
    {
        return mValues[0][1] * v[0] + mValues[1][1] * v[1] + mValues[2][1] * v[2] + mValues[3][1] * v[3];
    }

    inline F32 Dotz( const Vec4 &v ) const
    {
        return mValues[0][2] * v[0] + mValues[1][2] * v[1] + mValues[2][2] * v[2] + mValues[3][2] * v[3];
    }

    inline F32 Dotw( const Vec4 &v ) const
    {
        return mValues[0][3] * v[0] + mValues[1][3] * v[1] + mValues[2][3] * v[2] + mValues[3][3] * v[3];
    }

};

Matrix4 operator+( const Matrix4 &m1, const Matrix4 &m2 );
Matrix4 operator-( const Matrix4 &m1, const Matrix4 &m2 );
Matrix4 operator*( const Matrix4 &m1, const Matrix4 &m2 );
Matrix4 operator*( const Matrix4 &m, const F32 s );
Vec4 operator*( const Matrix4 &m, const Vec4 &v );

#endif