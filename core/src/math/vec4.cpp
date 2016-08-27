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
#include "math/vec4I.h"
#include "math/vec4.h"
#include "math/vec3.h"
#include "math/vec2.h"

#include "serialisation/serialisation.h"

Vec4::Vec4()
{
}

Vec4::Vec4( const F32 x, const F32 y, const F32 z, const F32 w )
{
    mValues[0] = x;
    mValues[1] = y;
    mValues[2] = z;
    mValues[3] = w;
}

Vec4::Vec4( const Vec4 &v )
{
    mValues[0] = v.mValues[0];
    mValues[1] = v.mValues[1];
    mValues[2] = v.mValues[2];
    mValues[3] = v.mValues[3];
}

Vec4::Vec4( const Vec4I &v )
{
    mValues[0] = ( F32 )v.GetX();
    mValues[1] = ( F32 )v.GetY();
    mValues[2] = ( F32 )v.GetZ();
    mValues[3] = ( F32 )v.GetW();
}

Vec4::Vec4( const Vec3 &other, F32 w )
{
    mValues[0] = other[0];
    mValues[1] = other[1];
    mValues[2] = other[2];
    mValues[3] = w;
}

Vec4::Vec4( const Vec2 &other1, const Vec2 &other2 )
{
    mValues[0] = other1[0];
    mValues[1] = other1[1];

    mValues[2] = other2[0];
    mValues[3] = other2[1];
}

Vec4 &Vec4::operator=( const Vec4 &other )
{
    mValues[0] = other.mValues[0];
    mValues[1] = other.mValues[1];
    mValues[2] = other.mValues[2];
    mValues[3] = other.mValues[3];

    return *this;
}

Vec4 &Vec4::operator-=( const Vec4 &v )
{
    mValues[0] -= v.mValues[0];
    mValues[1] -= v.mValues[1];
    mValues[2] -= v.mValues[2];
    mValues[3] -= v.mValues[3];

    return *this;
}

Vec4 &Vec4::operator+=( const Vec4 &v )
{
    mValues[0] += v.mValues[0];
    mValues[1] += v.mValues[1];
    mValues[2] += v.mValues[2];
    mValues[3] += v.mValues[3];

    return *this;
}

Vec4 &Vec4::operator/=( F32 s )
{
    assert( s != 0.0f );

    mValues[0] /= s;
    mValues[1] /= s;
    mValues[2] /= s;
    mValues[3] /= s;

    return *this;
}

Vec4 &Vec4::operator*=( F32 s )
{
    mValues[0] *= s;
    mValues[1] *= s;
    mValues[2] *= s;
    mValues[3] *= s;

    return *this;
}

bool Vec4::operator==( const Vec4 &other ) const
{
    return Mathf::Equal( mValues[0], other.mValues[0] ) && Mathf::Equal( mValues[1], other.mValues[1] ) &&
           Mathf::Equal( mValues[2], other.mValues[2] ) && Mathf::Equal( mValues[3], other.mValues[3] );
}

bool Vec4::operator!=( const Vec4 &other ) const
{
    return !( *this == other );
}

F32 &Vec4::operator[]( const U8 axis )
{
    return mValues[ axis ];
}

const F32 &Vec4::operator[]( const U8 axis ) const
{
    return mValues[ axis ];
}

F32 Vec4::Dot( const Vec4 &v ) const
{
    return mValues[0] * v.mValues[0] + mValues[1] * v.mValues[1] + mValues[2] * v.mValues[2] + mValues[3] * v.mValues[3];
}

F32 Vec4::Length2() const
{
    return Dot( *this );
}

F32 Vec4::Length() const
{
    return Mathf::Sqrt( Length2() );
}

F32 Vec4::Distance2( const Vec4 &v ) const
{
    return ( *this - v ).Length2();
}

F32 Vec4::Distance( const Vec4 &v ) const
{
    return ( *this - v ).Length();
}

void Vec4::SetValue( const F32 x, const F32 y, const F32 z, const F32 w )
{
    mValues[0] = x;
    mValues[1] = y;
    mValues[2] = z;
    mValues[3] = w;
}

Vec4 Vec4::SafeNormalise()
{
    Vec4 absv = Absolute();
    U8 max = absv.MaxAxis();

    if ( absv.mValues[max] > 0 )
    {
        assert( absv[max] != 0.0f );
        assert( Length() != 0.0f );

        *this /= absv.mValues[max];
        return *this /= Length();
    }

    SetValue( 1.0f, 0.0f, 0.0f, 0.0f );
    return *this;
}

Vec4 Vec4::Normalise()
{
    return *this /= Length();
}

Vec4 Vec4::Lerp( const Vec4 &v, const F32 t ) const
{
    return Vec4( mValues[0] + ( v.mValues[0] - mValues[0] ) * t, mValues[1] + ( v.mValues[1] - mValues[1] ) * t,
                 mValues[2] + ( v.mValues[2] - mValues[2] ) * t, mValues[3] + ( v.mValues[3] - mValues[3] ) * t );
}

Vec4 Vec4::Absolute() const
{
    return Vec4( Mathf::Abs( mValues[0] ), Mathf::Abs( mValues[1] ), Mathf::Abs( mValues[2] ), Mathf::Abs( mValues[3] ) );
}

U8 Vec4::MinAxis() const
{
    U8 axis = 0;
    F32 minvalue = mValues[0];

    for ( U8 i = 1; i < 4; ++i )
    {
        if ( minvalue > mValues[i] )
        {
            minvalue = mValues[i];
            axis = i;
        }
    }

    return axis;
}

U8 Vec4::MaxAxis() const
{
    U8 axis = 0;
    F32 maxvalue = mValues[0];

    for ( U8 i = 1; i < 4; ++i )
    {
        if ( maxvalue < mValues[i] )
        {
            maxvalue = mValues[i];
            axis = i;
        }
    }

    return axis;
}

void Vec4::SetX( const F32 x )
{
    mValues[0] = x;
}

F32 Vec4::GetX() const
{
    return mValues[0];
}

void Vec4::SetY( const F32 y )
{
    mValues[1] = y;
}

F32 Vec4::GetY() const
{
    return mValues[1];
}

void Vec4::SetZ( const F32 z )
{
    mValues[2] = z;
}

F32 Vec4::GetZ() const
{
    return mValues[2];
}

void Vec4::SetW( const F32 w )
{
    mValues[3] = w;
}

F32 Vec4::GetW() const
{
    return mValues[3];
}

void Vec4::SetZero()
{
    SetValue( 0.0f, 0.0f, 0.0f, 0.0f );
}

bool Vec4::IsZero() const
{
    return mValues[0] == 0.0f && mValues[1] == 0.0f && mValues[2] == 0.0f && mValues[3] == 0.0f;
}

bool Vec4::IsFuzzyZero() const
{
    return Length2() < Mathf::GetEpsilon();
}

void Vec4::Clear()
{
    SetValue( 0.0f, 0.0f, 0.0f, 0.0f );
}

Vec4 Vec4::GetZero()
{
    return Vec4( 0.0f, 0.0f, 0.0f, 0.0f );
}

Vec4 Vec4::GetOne()
{
    return Vec4( 1.0f, 1.0f, 1.0f, 1.0f );
}

Vec3 Vec4::GetXYZ() const
{
    return Vec3( mValues[0], mValues[1], mValues[2] );
}

void Vec4::OnStore( Message &message )
{
    message.StoreContainer( mValues, 0 );
}

Vec4 operator+( const Vec4 &v1, const Vec4 &v2 )
{
    return Vec4( v1.mValues[0] + v2.mValues[0],
                 v1.mValues[1] + v2.mValues[1],
                 v1.mValues[2] + v2.mValues[2],
                 v1.mValues[3] + v2.mValues[3] );
}

Vec4 operator*( const Vec4 &v1, const Vec4 &v2 )
{
    return Vec4( v1.mValues[0] * v2.mValues[0],
                 v1.mValues[1] * v2.mValues[1],
                 v1.mValues[2] * v2.mValues[2],
                 v1.mValues[3] * v2.mValues[3] );
}

Vec4 operator-( const Vec4 &v1, const Vec4 &v2 )
{
    return Vec4( v1.mValues[0] - v2.mValues[0],
                 v1.mValues[1] - v2.mValues[1],
                 v1.mValues[2] - v2.mValues[2],
                 v1.mValues[3] - v2.mValues[3] );
}

Vec4 operator-( const Vec4 &v )
{
    return Vec4( -v.mValues[0], -v.mValues[1], -v.mValues[2], -v.mValues[3] );
}

Vec4 operator*( const Vec4 &v, const F32 s )
{
    return Vec4( v.mValues[0] * s, v.mValues[1] * s, v.mValues[2] * s, v.mValues[3] * s );
}

Vec4 operator*( const F32 s, const Vec4 &v )
{
    return v * s;
}

Vec4 operator/( const Vec4 &v, const F32 s )
{
    assert( s != 0.0f );

    return v * ( F32( 1.0 ) / s );
}

Vec4 operator/( const Vec4 &v1, const Vec4 &v2 )
{
    return Vec4( v1.mValues[0] / v2.mValues[0],
                 v1.mValues[1] / v2.mValues[1],
                 v1.mValues[2] / v2.mValues[2],
                 v1.mValues[3] / v2.mValues[3] );
}