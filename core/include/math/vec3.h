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
#ifndef __ENGINE_VEC3_H__
#define __ENGINE_VEC3_H__

#include "common/types.h"

class Message;
class Vec3I;

class Vec3
{
    friend Vec3 operator+( const Vec3 &, const Vec3 & );
    friend Vec3 operator*( const Vec3 &, const Vec3 & );
    friend Vec3 operator-( const Vec3 &, const Vec3 & );
    friend Vec3 operator-( const Vec3 & );
    friend Vec3 operator*( const Vec3 &, const F32 );
    friend Vec3 operator*( const F32 , const Vec3 & );
    friend Vec3 operator/( const Vec3 &, const F32 );
    friend Vec3 operator/( const Vec3 &, const Vec3 & );

public:

    Vec3();
    Vec3( const F32 x, const F32 y, const F32 z );
    Vec3( const S32 x, const S32 y, const S32 z );
    Vec3( const Vec3 &v );
    Vec3( const Vec3I &v );

    Vec3 &operator=( const Vec3 &other );
    Vec3 &operator-=( const Vec3 &v );
    Vec3 &operator+=( const Vec3 &v );
    Vec3 &operator/=( const F32 s );
    Vec3 &operator*=( const F32 s );

    bool operator==( const Vec3 &other ) const;
    bool operator!=( const Vec3 &other ) const;

    F32 &operator[]( const U8 axis );
    const F32 &operator[]( const U8 axis ) const;

    F32 Dot( const Vec3 &v ) const;

    F32 Length2() const;
    F32 Length() const;

    F32 Distance2( const Vec3 &v ) const;
    F32 Distance( const Vec3 &v ) const;

    void SetValue( const F32 x, const F32 y, const F32 z );
    void Clear();

    Vec3 SafeNormalise();
    Vec3 Normalise();

    Vec3 Lerp( const Vec3 &v, const F32 t ) const;
    Vec3 Slerp( const Vec3 &v, const F32 t ) const;
    Vec3 Nlerp( const Vec3 &v, const F32 t ) const;

    Vec3 Rotate( const Vec3 &axis, const F32 angle ) const;
    Vec3 Cross( const Vec3 &v ) const;
    Vec3 Absolute() const;

    U8 MinAxis() const;
    U8 MaxAxis() const;

    F32 Angle( const Vec3 &v ) const;

    void SetX( const F32 x );
    F32 GetX() const;

    void SetY( const F32 y );
    F32 GetY() const;

    void SetZ( const F32 z );
    F32 GetZ() const;

    void SetZero();
    bool IsZero() const;

    bool IsFuzzyZero() const;

    static Vec3 GetZero();

    static Vec3 GetOne();

    static Vec3 GetBack();

    static Vec3 GetForward();

    static Vec3 GetDown();

    static Vec3 GetUp();

    static Vec3 GetLeft();

    static Vec3 GetRight();

    void OnStore( Message &message );

private:

    F32 mValues[3];
};

Vec3 operator+( const Vec3 &v1, const Vec3 &v2 );

Vec3 operator-( const Vec3 &v1, const Vec3 &v2 );
Vec3 operator-( const Vec3 &v );

Vec3 operator*( const Vec3 &v1, const Vec3 &v2 );
Vec3 operator*( const Vec3 &v, const F32 s );
Vec3 operator*( const F32 s, const Vec3 &v );

Vec3 operator/( const Vec3 &v, const F32 s );
Vec3 operator/( const Vec3 &v1, const Vec3 &v2 );

#endif