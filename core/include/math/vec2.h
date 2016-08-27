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
#ifndef __ENGINE_VEC2_H__
#define __ENGINE_VEC2_H__

#include "common/types.h"

class Message;
class Vec2I;

class Vec2
{
    friend Vec2 operator+( const Vec2 &, const Vec2 & );
    friend Vec2 operator*( const Vec2 &, const Vec2 & );
    friend Vec2 operator-( const Vec2 &, const Vec2 & );
    friend Vec2 operator-( const Vec2 & );
    friend Vec2 operator*( const Vec2 &, const F32 );
    friend Vec2 operator*( const F32 , const Vec2 & );
    friend Vec2 operator/( const Vec2 &, const F32 );
    friend Vec2 operator/( const Vec2 &, const Vec2 & );
public:

    Vec2();
    Vec2( const F32 x, const F32 y );
    Vec2( const Vec2 &v );
    Vec2( const Vec2I &v );

    Vec2 &operator=( const Vec2 &other );
    Vec2 &operator-=( const Vec2 &v );
    Vec2 &operator+=( const Vec2 &v );
    Vec2 &operator/=( const F32 s );
    Vec2 &operator*=( const F32 s );

    bool operator==( const Vec2 &other ) const;
    bool operator!=( const Vec2 &other ) const;

    F32 &operator[]( const U8 axis );
    const F32 &operator[]( const U8 axis ) const;

    F32 Dot( const Vec2 &v ) const;

    F32 Length2() const;
    F32 Length() const;

    F32 Distance2( const Vec2 &v ) const;
    F32 Distance( const Vec2 &v ) const;

    void SetValue( F32 x, F32 y );
    void Clear();

    Vec2 SafeNormalise();
    Vec2 Normalise();

    Vec2 Lerp( const Vec2 &v, const F32 t ) const;
    Vec2 Nlerp( const Vec2 &v, const F32 t ) const;

    Vec2 Rotate( const F32 angle ) const;
    Vec2 Absolute() const;

    U8 MinAxis() const;
    U8 MaxAxis() const;

    F32 Angle( const Vec2 &v ) const;

    void SetX( const F32 x );
    F32 GetX() const;

    void SetY( const F32 y );
    F32 GetY() const;

    void SetZero();
    bool IsZero() const;

    bool IsFuzzyZero() const;

    static Vec2 GetZero();

    static Vec2 GetOne();

    static Vec2 GetDown();

    static Vec2 GetUp();

    static Vec2 GetLeft();

    static Vec2 GetRight();

    void OnStore( Message &message );

private:

    F32 mValues[2];
};

Vec2 operator+( const Vec2 &v1, const Vec2 &v2 );

Vec2 operator-( const Vec2 &v1, const Vec2 &v2 );
Vec2 operator-( const Vec2 &v );

Vec2 operator*( const Vec2 &v1, const Vec2 &v2 );
Vec2 operator*( const Vec2 &v, const F32 s );
Vec2 operator*( const F32 s, const Vec2 &v );

Vec2 operator/( const Vec2 &v, const F32 s );
Vec2 operator/( const Vec2 &v1, const Vec2 &v2 );

#endif