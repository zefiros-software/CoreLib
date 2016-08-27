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
#include "math/vec3i.h"
#include "math/vec3.h"

#include "serialisation/serialisation.h"

Vec3::Vec3()
{
}

Vec3::Vec3( const F32 x, const F32 y, const F32 z )
{
    mValues[0] = x;
    mValues[1] = y;
    mValues[2] = z;
}

Vec3::Vec3( const S32 x, const S32 y, const S32 z )
{
    mValues[0] = static_cast< F32 >( x );
    mValues[1] = static_cast< F32 >( y );
    mValues[2] = static_cast< F32 >( z );
}

Vec3::Vec3( const Vec3 &v )
{
    mValues[0] = v.mValues[0];
    mValues[1] = v.mValues[1];
    mValues[2] = v.mValues[2];
}

Vec3::Vec3( const Vec3I &v )
{
    mValues[0] = ( F32 )v.GetX();
    mValues[1] = ( F32 )v.GetY();
    mValues[2] = ( F32 )v.GetZ();
}

Vec3 &Vec3::operator=( const Vec3 &other )
{
    mValues[0] = other.mValues[0];
    mValues[1] = other.mValues[1];
    mValues[2] = other.mValues[2];

    return *this;
}

Vec3 &Vec3::operator-=( const Vec3 &v )
{
    mValues[0] -= v.mValues[0];
    mValues[1] -= v.mValues[1];
    mValues[2] -= v.mValues[2];

    return *this;
}

Vec3 &Vec3::operator+=( const Vec3 &v )
{
    mValues[0] += v.mValues[0];
    mValues[1] += v.mValues[1];
    mValues[2] += v.mValues[2];

    return *this;
}

Vec3 &Vec3::operator/=( const F32 s )
{
    assert( s != 0.0f );

    mValues[0] /= s;
    mValues[1] /= s;
    mValues[2] /= s;

    return *this;
}

Vec3 &Vec3::operator*=( const F32 s )
{
    mValues[0] *= s;
    mValues[1] *= s;
    mValues[2] *= s;

    return *this;
}

bool Vec3::operator==( const Vec3 &other ) const
{
    return Mathf::Equal( mValues[0], other.mValues[0] ) && Mathf::Equal( mValues[1], other.mValues[1] ) &&
           Mathf::Equal( mValues[2], other.mValues[2] );
}

bool Vec3::operator!=( const Vec3 &other ) const
{
    return !( *this == other );
}

F32 &Vec3::operator[]( const U8 axis )
{
    return mValues[ axis ];
}

const F32 &Vec3::operator[]( const U8 axis ) const
{
    return mValues[ axis ];
}

F32 Vec3::Dot( const Vec3 &v ) const
{
    return mValues[0] * v.mValues[0] + mValues[1] * v.mValues[1] + mValues[2] * v.mValues[2];
}

F32 Vec3::Length2() const
{
    return Dot( *this );
}

F32 Vec3::Length() const
{
    return Mathf::Sqrt( Length2() );
}

F32 Vec3::Distance2( const Vec3 &v ) const
{
    return ( *this - v ).Length2();
}

F32 Vec3::Distance( const Vec3 &v ) const
{
    return ( *this - v ).Length();
}

void Vec3::SetValue( const F32 x, const F32 y, const F32 z )
{
    mValues[0] = x;
    mValues[1] = y;
    mValues[2] = z;
}

Vec3 Vec3::SafeNormalise()
{
    Vec3 absv = Absolute();
    U8 max = absv.MaxAxis();

    if ( absv.mValues[max] > 0 )
    {
        assert( absv[max] != 0.0f );
        assert( Length() != 0.0f );

        *this /= absv.mValues[max];
        return *this /= Length();
    }

    SetValue( 1.0f, 0.0f, 0.0f );
    return *this;
}

Vec3 Vec3::Normalise()
{
    assert( Length() != 0.0f );

    return *this /= Length();
}

Vec3 Vec3::Lerp( const Vec3 &v, const F32 t ) const
{
    return Vec3( Mathf::Lerp( mValues[0], v.mValues[0], t ), Mathf::Lerp( mValues[1], v.mValues[1], t ),
                 Mathf::Lerp( mValues[2], v.mValues[2], t ) );
}

Vec3 Vec3::Slerp( const Vec3 &v, const F32 t ) const
{
    F32 dotp = Dot( v );

    // Make sure the floating point accuracy doesn't
    // crash our program
    Mathf::Clamp( dotp , -1.0f, 1.0f );

    F32 theta = Mathf::Acos( dotp ) * t;
    Vec3 relative = v - *this * dotp;
    relative.Normalise();

    return *this * Mathf::Cos( theta ) + relative * Mathf::Sin( theta );

}

Vec3 Vec3::Nlerp( const Vec3 &v, const F32 t ) const
{
    return Lerp( v, t ).Normalise();
}

Vec3 Vec3::Rotate( const Vec3 &axis, const F32 angle ) const
{
    Vec3 v1 = axis * axis.Dot( *this );
    return v1 + ( *this - v1 ) * Mathf::Cos( angle ) + axis.Cross( *this ) * Mathf::Sin( angle );
}

Vec3 Vec3::Cross( const Vec3 &v ) const
{
    return Vec3( mValues[1] * v.mValues[2] - mValues[2] * v.mValues[1],
                 mValues[2] * v.mValues[0] - mValues[0] * v.mValues[2],
                 mValues[0] * v.mValues[1] - mValues[1] * v.mValues[0] );
}

Vec3 Vec3::Absolute() const
{
    return Vec3( Mathf::Abs( mValues[0] ), Mathf::Abs( mValues[1] ), Mathf::Abs( mValues[2] ) );
}

U8 Vec3::MinAxis() const
{
    return mValues[0] < mValues[1] ? ( mValues[0] < mValues[2] ? 0 : 2 ) : ( mValues[1] < mValues[2] ? 1 : 2 );
}

U8 Vec3::MaxAxis() const
{
    return mValues[0] < mValues[1] ? ( mValues[1] < mValues[2] ? 2 : 1 ) : ( mValues[0] < mValues[2] ? 2 : 0 );
}

F32 Vec3::Angle( const Vec3 &v ) const
{
    F32 s = Mathf::Sqrt( Length2() * v.Length2() );

    assert( s != 0.0f );

    return Mathf::Acos( Mathf::Clamp( Dot( v ) / s, -1.0f, 1.0f ) );
}

void Vec3::SetX( const F32 x )
{
    mValues[0] = x;
}

F32 Vec3::GetX() const
{
    return mValues[0];
}

void Vec3::SetY( const F32 y )
{
    mValues[1] = y;
}

F32 Vec3::GetY() const
{
    return mValues[1];
}

void Vec3::SetZ( const F32 z )
{
    mValues[2] = z;
}

F32 Vec3::GetZ() const
{
    return mValues[2];
}

void Vec3::SetZero()
{
    SetValue( 0.0f, 0.0f, 0.0f );
}

bool Vec3::IsZero() const
{
    return mValues[0] == 0.0f && mValues[1] == 0.0f && mValues[2] == 0.0f;
}

bool Vec3::IsFuzzyZero() const
{
    return Length2() < Mathf::GetEpsilon();
}

void Vec3::Clear()
{
    SetValue( 0.0f, 0.0f, 0.0f );
}

Vec3 Vec3::GetZero()
{
    return Vec3( 0.0f, 0.0f, 0.0f );
}

Vec3 Vec3::GetOne()
{
    return Vec3( 1.0f, 1.0f, 1.0f );
}

Vec3 Vec3::GetBack()
{
    return Vec3( 0.0f, -1.0f, 0.0f );
}

Vec3 Vec3::GetForward()
{
    return Vec3( 0.0f, 1.0f, 0.0f );
}

Vec3 Vec3::GetDown()
{
    return Vec3( 0.0f, 0.0f, -1.0f );
}

Vec3 Vec3::GetUp()
{
    return Vec3( 0.0f, 0.0f, 1.0f );
}

Vec3 Vec3::GetLeft()
{
    return Vec3( -1.0f, 0.0f, 0.0f );
}

Vec3 Vec3::GetRight()
{
    return Vec3( 1.0f, 0.0f, 0.0f );
}

void Vec3::OnStore( Message &message )
{
    message.StoreContainer( mValues, 0 );
}

Vec3 operator+( const Vec3 &v1, const Vec3 &v2 )
{
    return Vec3( v1.mValues[0] + v2.mValues[0],
                 v1.mValues[1] + v2.mValues[1],
                 v1.mValues[2] + v2.mValues[2] );
}

Vec3 operator*( const Vec3 &v1, const Vec3 &v2 )
{
    return Vec3( v1.mValues[0] * v2.mValues[0],
                 v1.mValues[1] * v2.mValues[1],
                 v1.mValues[2] * v2.mValues[2] );
}

Vec3 operator-( const Vec3 &v1, const Vec3 &v2 )
{
    return Vec3( v1.mValues[0] - v2.mValues[0],
                 v1.mValues[1] - v2.mValues[1],
                 v1.mValues[2] - v2.mValues[2] );
}

Vec3 operator-( const Vec3 &v )
{
    return Vec3( -v.mValues[0], -v.mValues[1], -v.mValues[2] );
}

Vec3 operator*( const Vec3 &v, const F32 s )
{
    return Vec3( v.mValues[0] * s, v.mValues[1] * s, v.mValues[2] * s );
}

Vec3 operator*( F32 s, const Vec3 &v )
{
    return v * s;
}

Vec3 operator/( const Vec3 &v, F32 s )
{
    assert( s != 0.0f );
    return v * ( F32( 1.0 ) / s );
}

Vec3 operator/( const Vec3 &v1, const Vec3 &v2 )
{
    return Vec3( v1.mValues[0] / v2.mValues[0],
                 v1.mValues[1] / v2.mValues[1] ,
                 v1.mValues[2] / v2.mValues[2] );
}

