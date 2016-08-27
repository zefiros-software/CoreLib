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

#include "math/vec2i.h"
#include "math/vec2.h"

#include <assert.h>

Vec2I::Vec2I()
{
}

Vec2I::Vec2I( const Vec2 &v )
{
    SetValue( ( S32 )v.GetX(), ( S32 )v.GetY() );
}

Vec2I::Vec2I( const S32 x, const S32 y )
{
    SetValue( x, y );
}

Vec2I::Vec2I( const F32 x, const F32 y )
{
    SetValue( ( S32 )x, ( S32 )y );
}

S32 &Vec2I::operator[]( const U8 axis )
{
    return mValues[ axis ];
}

const S32 &Vec2I::operator[]( const U8 axis ) const
{
    return mValues[ axis ];
}

void Vec2I::SetX( const S32 &val )
{
    mValues[0] = val;
}

S32 Vec2I::GetX() const
{
    return mValues[0];
}

void Vec2I::SetY( const S32 &val )
{
    mValues[1] = val;
}

S32 Vec2I::GetY() const
{
    return mValues[1];
}

void Vec2I::SetValue( const S32 &x, const S32 &y )
{
    mValues[0] = x;
    mValues[1] = y;
}

Vec2 Vec2I::ToVec() const
{
    return Vec2( ( F32 )mValues[0], ( F32 )mValues[1] );
}

void Vec2I::Clear()
{
    SetValue( 0, 0 );
}

Vec2I Vec2I::GetZero()
{
    return Vec2I( 0, 0 );
}

Vec2I Vec2I::GetOne()
{
    return Vec2I( 1, 1 );
}

Vec2I Vec2I::GetDown()
{
    return Vec2I( 0, -1 );
}

Vec2I Vec2I::GetUp()
{
    return Vec2I( 0, 1 );
}

Vec2I Vec2I::GetLeft()
{
    return Vec2I( -1, 0 );
}

Vec2I Vec2I::GetRight()
{
    return Vec2I( 1, 0 );
}

Vec2I operator+( const Vec2I &v1, const Vec2I &v2 )
{
    return Vec2I( v1.mValues[0] + v2.mValues[0],
                  v1.mValues[1] + v2.mValues[1] );
}

Vec2I operator*( const Vec2I &v1, const Vec2I &v2 )
{
    return Vec2I( v1.mValues[0] * v2.mValues[0],
                  v1.mValues[1] * v2.mValues[1] );
}

Vec2I operator-( const Vec2I &v1, const Vec2I &v2 )
{
    return Vec2I( v1.mValues[0] - v2.mValues[0],
                  v1.mValues[1] - v2.mValues[1] );
}

Vec2I operator-( const Vec2I &v )
{
    return Vec2I( -v.mValues[0], -v.mValues[1] );
}

Vec2I operator*( const Vec2I &v, const F32 s )
{
    return Vec2I( v.mValues[0] * s, v.mValues[1] * s );
}

Vec2I operator*( const F32 s, const Vec2I &v )
{
    return v * s;
}

Vec2I operator/( const Vec2I &v, const F32 s )
{
    assert( s != 0.0f );

    return v * ( F32( 1.0 ) / s );
}

Vec2I operator/( const Vec2I &v1, const Vec2I &v2 )
{
    return Vec2I( v1.mValues[0] / v2.mValues[0],
                  v1.mValues[1] / v2.mValues[1] );
}