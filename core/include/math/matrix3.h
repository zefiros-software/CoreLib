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
#ifndef __ENGINE_MATRIX3_H__
#define __ENGINE_MATRIX3_H__

#include "math/vec3.h"

class Quaternion;

class Matrix3
{
    friend Matrix3 operator+( const Matrix3 &m1, const Matrix3 &m2 );
    friend Matrix3 operator-( const Matrix3 &m1, const Matrix3 &m2 );
    friend Matrix3 operator*( const Matrix3 &m1, const Matrix3 &m2 );
    friend Matrix3 operator*( const Matrix3 &m, const F32 s );
    friend Vec3 operator*( const Matrix3 &m, const Vec3 &v );

public:

    Matrix3();
    Matrix3( const Quaternion &q );
    Matrix3( const F32 m11, const F32 m12, const F32 m13,
             const F32 m21, const F32 m22, const F32 m23,
             const F32 m31, const F32 m32, const F32 m33 );
    Matrix3( const Vec3 &v1, const Vec3 &v2, const Vec3 &v3 );
    Matrix3( const Matrix3 &other );

    Matrix3 &operator=( const Matrix3 &m );
    Matrix3 &operator*=( const Matrix3 &m );
    Matrix3 &operator+=( const Matrix3 &m );
    Matrix3 &operator-=( const Matrix3 &m );

    bool operator==( const Matrix3 &m ) const;
    bool operator!=( const Matrix3 &m ) const;

    void SetColumn( const U8 column, const Vec3 &v );
    Vec3 GetColumn( const U8 column ) const;

    void SetRow( const U8 row, const Vec3 &v );
    Vec3 GetRow( const U8 row ) const;

    void SetRotation( const Quaternion &q );
    Quaternion GetRotation() const;

    void SetEuler( const F32 yaw, const F32 pitch, const F32 roll );
    Vec3 GetEuler() const;

    Matrix3 Scale( const Vec3 &v ) const;
    Matrix3 GetTranspose() const;
    Matrix3 GetAbsolute() const;
    Matrix3 GetAdjoint() const;
    Matrix3 GetInverse() const;

    void SetIdentity();

    F32 GetCofactor( const U8 row, const U8 column ) const;
    F32 GetDeterminant() const;

    void SetValue( const F32 m11, const F32 m12, const F32 m13,
                   const F32 m21, const F32 m22, const F32 m23,
                   const F32 m31, const F32 m32, const F32 m33 );

    static Matrix3 GetZero();

    static Matrix3 GetIdentity();

private:

    Vec3 mValues[3];

    F32 Dotx( const Vec3 &v ) const;
    F32 Doty( const Vec3 &v ) const;
    F32 Dotz( const Vec3 &v ) const;
};

Matrix3 operator+( const Matrix3 &m1, const Matrix3 &m2 );
Matrix3 operator-( const Matrix3 &m1, const Matrix3 &m2 );
Matrix3 operator*( const Matrix3 &m1, const Matrix3 &m2 );
Matrix3 operator*( const Matrix3 &m, const F32 s );
Vec3 operator*( const Matrix3 &m, const Vec3 &v );

#endif