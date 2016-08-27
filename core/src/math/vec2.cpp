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

#include "math/mathf.h"
#include "math/vec2i.h"
#include "math/vec2.h"

#include "serialisation/serialisation.h"

Vec2::Vec2()
{
}

Vec2::Vec2( const F32 x, const F32 y )
{
    mValues[0] = x;
    mValues[1] = y;
}

Vec2::Vec2( const Vec2 &v )
{
    mValues[0] = v.mValues[0];
    mValues[1] = v.mValues[1];
}

Vec2::Vec2( const Vec2I &v )
{
    mValues[0] = ( F32 )v.GetX();
    mValues[1] = ( F32 )v.GetY();
}

Vec2 &Vec2::operator=( const Vec2 &other )
{
    mValues[0] = other.mValues[0];
    mValues[1] = other.mValues[1];

    return *this;
}

Vec2 &Vec2::operator-=( const Vec2 &v )
{
    mValues[0] -= v.mValues[0];
    mValues[1] -= v.mValues[1];

    return *this;
}

Vec2 &Vec2::operator+=( const Vec2 &v )
{
    mValues[0] += v.mValues[0];
    mValues[1] += v.mValues[1];

    return *this;
}

Vec2 &Vec2::operator/=( const F32 s )
{
    assert( s != 0.0f );

    mValues[0] /= s;
    mValues[1] /= s;

    return *this;
}

Vec2 &Vec2::operator*=( const F32 s )
{
    mValues[0] *= s;
    mValues[1] *= s;

    return *this;
}

bool Vec2::operator==( const Vec2 &other ) const
{
    return Mathf::Equal( mValues[0], other.mValues[0] ) && Mathf::Equal( mValues[1], other.mValues[1] );
}

bool Vec2::operator!=( const Vec2 &other ) const
{
    return !( *this == other );
}

F32 &Vec2::operator[]( const U8 axis )
{
    return mValues[ axis ];
}

const F32 &Vec2::operator[]( const U8 axis ) const
{
    return mValues[ axis ];
}

F32 Vec2::Dot( const Vec2 &v ) const
{
    return mValues[0] * v.mValues[0] + mValues[1] * v.mValues[1];
}

F32 Vec2::Length2() const
{
    return Dot( *this );
}

F32 Vec2::Length() const
{
    return Mathf::Sqrt( Length2() );
}

F32 Vec2::Distance2( const Vec2 &v ) const
{
    return ( *this - v ).Length2();
}

F32 Vec2::Distance( const Vec2 &v ) const
{
    return ( *this - v ).Length();
}

void Vec2::SetValue( const F32 x, const F32 y )
{
    mValues[0] = x;
    mValues[1] = y;
}

Vec2 Vec2::SafeNormalise()
{
    Vec2 absv = Absolute();
    U8 max = absv.MaxAxis();

    if ( absv.mValues[max] > 0 )
    {
        *this /= absv.mValues[max];
        return *this /= Length();
    }

    SetValue( 1.0f, 0.0f );
    return *this;
}

Vec2 Vec2::Normalise()
{
    assert( Length() != 0.0f );

    return *this /= Length();
}

Vec2 Vec2::Lerp( const Vec2 &v, const F32 t ) const
{
    return Vec2( mValues[0] + ( v.mValues[0] - mValues[0] ) * t, mValues[1] + ( v.mValues[1] - mValues[1] ) * t );
}

Vec2 Vec2::Nlerp( const Vec2 &v, const F32 t ) const
{
    return Lerp( v, t ).Normalise();
}

Vec2 Vec2::Rotate( const F32 angle ) const
{
    return Vec2( Mathf::Cos( angle ) * mValues[0] - Mathf::Sin( angle ) * mValues[1],
                 Mathf::Sin( angle ) * mValues[0] - Mathf::Cos( angle ) * mValues[1] );
}

Vec2 Vec2::Absolute() const
{
    return Vec2( Mathf::Abs( mValues[0] ), Mathf::Abs( mValues[1] ) );
}

U8 Vec2::MinAxis() const
{
    return mValues[0] < mValues[1] ? 0 : 1;
}

U8 Vec2::MaxAxis() const
{
    return mValues[0] > mValues[1] ? 0 : 1;
}

F32 Vec2::Angle( const Vec2 &v ) const
{
    F32 s = Mathf::Sqrt( Length2() * v.Length2() );

    assert( s != 0.0f );

    return Mathf::Acos( Mathf::Clamp( Dot( v ) / s, -1.0f, 1.0f ) );
}

void Vec2::SetX( const F32 x )
{
    mValues[0] = x;
}

F32 Vec2::GetX() const
{
    return mValues[0];
}

void Vec2::SetY( const F32 y )
{
    mValues[1] = y;
}

F32 Vec2::GetY() const
{
    return mValues[1];
}

void Vec2::SetZero()
{
    SetValue( 0.0f, 0.0f );
}

bool Vec2::IsZero() const
{
    return mValues[0] == 0.0f && mValues[1] == 0.0f;
}

bool Vec2::IsFuzzyZero() const
{
    return Length2() < Mathf::GetEpsilon();
}

void Vec2::Clear()
{
    SetValue( 0.0f, 0.0f );
}

Vec2 Vec2::GetZero()
{
    return Vec2( 0.0f, 0.0f );
}

Vec2 Vec2::GetOne()
{
    return Vec2( 1.0f, 1.0f );
}

Vec2 Vec2::GetDown()
{
    return Vec2( 0.0f, -1.0f );
}

Vec2 Vec2::GetUp()
{
    return Vec2( 0.0f, 1.0f );
}

Vec2 Vec2::GetLeft()
{
    return Vec2( -1.0f, 0.0f );
}

Vec2 Vec2::GetRight()
{
    return Vec2( 1.0f, 0.0f );
}

void Vec2::OnStore( Message &message )
{
    message.StoreContainer( mValues, 0 );
}

Vec2 operator+( const Vec2 &v1, const Vec2 &v2 )
{
    return Vec2( v1.mValues[0] + v2.mValues[0],
                 v1.mValues[1] + v2.mValues[1] );
}

Vec2 operator*( const Vec2 &v1, const Vec2 &v2 )
{
    return Vec2( v1.mValues[0] * v2.mValues[0],
                 v1.mValues[1] * v2.mValues[1] );
}

Vec2 operator-( const Vec2 &v1, const Vec2 &v2 )
{
    return Vec2( v1.mValues[0] - v2.mValues[0],
                 v1.mValues[1] - v2.mValues[1] );
}

Vec2 operator-( const Vec2 &v )
{
    return Vec2( -v.mValues[0], -v.mValues[1] );
}

Vec2 operator*( const Vec2 &v, const F32 s )
{
    return Vec2( v.mValues[0] * s, v.mValues[1] * s );
}

Vec2 operator*( const F32 s, const Vec2 &v )
{
    return v * s;
}

Vec2 operator/( const Vec2 &v, const F32 s )
{
    assert( s != 0.0f );

    return v * ( F32( 1.0 ) / s );
}

Vec2 operator/( const Vec2 &v1, const Vec2 &v2 )
{
    return Vec2( v1.mValues[0] / v2.mValues[0],
                 v1.mValues[1] / v2.mValues[1] );
}
