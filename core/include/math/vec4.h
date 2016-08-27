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
#ifndef __ENGINE_VEC4_H__
#define __ENGINE_VEC4_H__

#include "common/types.h"

class Message;
class Vec4I;
class Vec3;
class Vec2;

class Vec4
{
    friend Vec4 operator+( const Vec4 &, const Vec4 & );
    friend Vec4 operator*( const Vec4 &, const Vec4 & );
    friend Vec4 operator-( const Vec4 &, const Vec4 & );
    friend Vec4 operator-( const Vec4 & );
    friend Vec4 operator*( const Vec4 &, const F32 );
    friend Vec4 operator*( const F32 , const Vec4 & );
    friend Vec4 operator/( const Vec4 &, const F32 );
    friend Vec4 operator/( const Vec4 &, const Vec4 & );

public:

    Vec4();
    Vec4( const F32 x, const F32 y, const F32 z, const F32 w );
    Vec4( const Vec3 &other, F32 w );
    Vec4( const Vec2 &other1, const Vec2 &other2 );
    Vec4( const Vec4 &v );
    Vec4( const Vec4I &v );

    Vec4 &operator=( const Vec4 &other );
    Vec4 &operator-=( const Vec4 &v );
    Vec4 &operator+=( const Vec4 &v );
    Vec4 &operator/=( const F32 s );
    Vec4 &operator*=( const F32 s );

    bool operator==( const Vec4 &other ) const;
    bool operator!=( const Vec4 &other ) const;

    F32 &operator[]( const U8 axis );
    const F32 &operator[]( const U8 axis ) const;

    F32 Dot( const Vec4 &v ) const;

    F32 Length2() const;
    F32 Length() const;

    F32 Distance2( const Vec4 &v ) const;
    F32 Distance( const Vec4 &v ) const;

    void SetValue( const F32 x, const F32 y, const F32 z, const F32 w );
    void Clear();

    Vec4 SafeNormalise();
    Vec4 Normalise();

    Vec4 Lerp( const Vec4 &v, F32 t ) const;

    Vec4 Absolute() const;

    U8 MinAxis() const;
    U8 MaxAxis() const;

    void SetX( const F32 x );
    F32 GetX() const;

    void SetY( const F32 y );
    F32 GetY() const;

    void SetZ( const F32 z );
    F32 GetZ() const;

    void SetW( const F32 w );
    F32 GetW() const;

    Vec3 GetXYZ() const;

    void SetZero();
    bool IsZero() const;
    bool IsFuzzyZero() const;

    static Vec4 GetZero();

    static Vec4 GetOne();

    void OnStore( Message &message );

private:

    F32 mValues[4];
};

Vec4 operator+( const Vec4 &v1, const Vec4 &v2 );

Vec4 operator-( const Vec4 &v1, const Vec4 &v2 );
Vec4 operator-( const Vec4 &v );

Vec4 operator*( const Vec4 &v1, const Vec4 &v2 );
Vec4 operator*( const Vec4 &v, const F32 s );
Vec4 operator*( const F32 s, const Vec4 &v );

Vec4 operator/( const Vec4 &v, const F32 s );
Vec4 operator/( const Vec4 &v1, const Vec4 &v2 );

#endif