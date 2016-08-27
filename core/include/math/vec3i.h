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
#ifndef __ENGINE_VEC3I_H__
#define __ENGINE_VEC3I_H__

#include "common/types.h"

class Vec3;

class Vec3I
{
    friend Vec3I operator+( const Vec3I &, const Vec3I & );
    friend Vec3I operator*( const Vec3I &, const Vec3I & );
    friend Vec3I operator-( const Vec3I &, const Vec3I & );
    friend Vec3I operator-( const Vec3I & );
    friend Vec3I operator*( const Vec3I &, const F32 );
    friend Vec3I operator*( const F32 , const Vec3I & );
    friend Vec3I operator/( const Vec3I &, const F32 );
    friend Vec3I operator/( const Vec3I &, const Vec3I & );

public:

    Vec3I();
    Vec3I( const Vec3 &v );
    Vec3I( const S32 x, const S32 y, const S32 z );
    Vec3I( const F32 x, const F32 y, const F32 z );

    S32 &operator[]( const U8 axis );
    const S32 &operator[]( const U8 axis ) const;

    void SetX( const S32 val );
    S32 GetX() const;

    void SetY( const S32 val );
    S32 GetY() const;

    void SetZ( const S32 val );
    S32 GetZ() const;

    void SetValue( const S32 x, const S32 y, const S32 z );
    void Clear();

    Vec3 ToVec() const;

    static Vec3I GetZero();

    static Vec3I GetOne();

    static Vec3I GetBack();

    static Vec3I GetForward();

    static Vec3I GetDown();

    static Vec3I GetUp();

    static Vec3I GetLeft();

    static Vec3I GetRight();

private:

    S32 mValues[3];
};

Vec3I operator+( const Vec3I &v1, const Vec3I &v2 );
Vec3I operator*( const Vec3I &v1, const Vec3I &v2 );

Vec3I operator-( const Vec3I &v1, const Vec3I &v2 );
Vec3I operator-( const Vec3I &v );

Vec3I operator*( const Vec3I &v, const F32 s );
Vec3I operator*( F32 s, const Vec3I &v );

Vec3I operator/( const Vec3I &v, F32 s );
Vec3I operator/( const Vec3I &v1, const Vec3I &v2 );

#endif