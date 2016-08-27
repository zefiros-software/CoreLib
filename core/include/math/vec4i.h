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
#ifndef __ENGINE_VEC4I_H__
#define __ENGINE_VEC4I_H__

#include "common/types.h"

class Vec4;

class Vec4I
{
    friend Vec4I operator+( const Vec4I &, const Vec4I & );
    friend Vec4I operator*( const Vec4I &, const Vec4I & );
    friend Vec4I operator-( const Vec4I &, const Vec4I & );
    friend Vec4I operator-( const Vec4I & );
    friend Vec4I operator*( const Vec4I &, const F32 );
    friend Vec4I operator*( const F32 , const Vec4I & );
    friend Vec4I operator/( const Vec4I &, const F32 );
    friend Vec4I operator/( const Vec4I &, const Vec4I & );

public:
    Vec4I();
    Vec4I( const Vec4 &v );
    Vec4I( const S32 &x, const S32 &y, const S32 &z, const S32 &w );
    Vec4I( F32 x, F32 y, F32 z, F32 w );

    S32 &operator[]( const size_t axis );
    const S32 &operator[]( const size_t axis ) const;

    S32 GetX() const;
    void SetX( const S32 &val );

    S32 GetY() const;
    void SetY( const S32 &val );

    S32 GetZ() const;
    void SetZ( const S32 &val );

    S32 GetW() const;
    void SetW( const S32 &val );

    void SetValue( const S32 &x, const S32 &y, const S32 &z, const S32 &w );
    void Clear();

    Vec4 ToVec() const;

    static Vec4I GetZero();

    static Vec4I GetOne();

private:

    S32 mValues[4];
};

Vec4I operator+( const Vec4I &v1, const Vec4I &v2 );
Vec4I operator*( const Vec4I &v1, const Vec4I &v2 );

Vec4I operator-( const Vec4I &v1, const Vec4I &v2 );
Vec4I operator-( const Vec4I &v );

Vec4I operator*( const Vec4I &v, const F32 s );
Vec4I operator*( const F32 s, const Vec4I &v );

Vec4I operator/( const Vec4I &v, const F32 s );
Vec4I operator/( const Vec4I &v1, const Vec4I &v2 );

#endif