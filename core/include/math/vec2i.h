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
#ifndef __ENGINE_VEC2I_H__
#define __ENGINE_VEC2I_H__

#include "common/types.h"

class Vec2;

class Vec2I
{
    friend Vec2I operator+( const Vec2I &, const Vec2I & );
    friend Vec2I operator*( const Vec2I &, const Vec2I & );
    friend Vec2I operator-( const Vec2I &, const Vec2I & );
    friend Vec2I operator-( const Vec2I & );
    friend Vec2I operator*( const Vec2I &, const F32 );
    friend Vec2I operator*( const F32 , const Vec2I & );
    friend Vec2I operator/( const Vec2I &, const F32 );
    friend Vec2I operator/( const Vec2I &, const Vec2I & );
public:

    Vec2I();
    Vec2I( const Vec2 &v );
    Vec2I( const S32 x, const S32 y );
    Vec2I( const F32 x, const F32 y );

    S32 &operator[]( const U8 axis );
    const S32 &operator[]( const U8 axis ) const;

    void SetX( const S32 &val );
    S32 GetX() const;

    void SetY( const S32 &val );
    S32 GetY() const;

    void SetValue( const S32 &x, const S32 &y );
    void Clear();

    Vec2 ToVec() const;

    static Vec2I GetZero();

    static Vec2I GetOne();

    static Vec2I GetDown();

    static Vec2I GetUp();

    static Vec2I GetLeft();

    static Vec2I GetRight();

private:

    S32 mValues[2];

};

Vec2I operator+( const Vec2I &v1, const Vec2I &v2 );

Vec2I operator*( const Vec2I &v1, const Vec2I &v2 );

Vec2I operator-( const Vec2I &v1, const Vec2I &v2 );
Vec2I operator-( const Vec2I &v );

Vec2I operator*( const Vec2I &v, const F32 s );
Vec2I operator*( const F32 s, const Vec2I &v );

Vec2I operator/( const Vec2I &v, const F32 s );
Vec2I operator/( const Vec2I &v1, const Vec2I &v2 );

#endif