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

#include "math/quaternion.h"
#include "math/mathf.h"

#include <assert.h>

Matrix3::Matrix3()
{
}

Matrix3::Matrix3( const Quaternion &q )
{
    SetRotation( q );
}

Matrix3::Matrix3( const F32 a11, const F32 a12, const F32 a13,
                  const F32 a21, const F32 a22, const F32 a23,
                  const F32 a31, const F32 a32, const F32 a33 )
{
    SetValue( a11, a12, a13, a21, a22, a23, a31, a32, a33 );
}

Matrix3::Matrix3( const Matrix3 &other )
{
    mValues[0] = other.mValues[0];
    mValues[1] = other.mValues[1];
    mValues[2] = other.mValues[2];
}

Matrix3::Matrix3( const Vec3 &v1, const Vec3 &v2, const Vec3 &v3 )
{
    mValues[0] = v1;
    mValues[1] = v2;
    mValues[2] = v3;
}

Matrix3 &Matrix3::operator=( const Matrix3 &m )
{
    mValues[0] = m.mValues[0];
    mValues[1] = m.mValues[1];
    mValues[2] = m.mValues[2];

    return *this;
}

Matrix3 &Matrix3::operator*=( const Matrix3 &m )
{
    SetValue( m.Dotx( mValues[0] ), m.Doty( mValues[0] ), m.Dotz( mValues[0] ),
              m.Dotx( mValues[1] ), m.Doty( mValues[1] ), m.Dotz( mValues[1] ),
              m.Dotx( mValues[2] ), m.Doty( mValues[2] ), m.Dotz( mValues[2] ) );

    return *this;
}

Matrix3 &Matrix3::operator+=( const Matrix3 &m )
{
    SetValue( mValues[0][0] + m.mValues[0][0], mValues[0][1] + m.mValues[0][1], mValues[0][2] + m.mValues[0][2],
              mValues[1][0] + m.mValues[1][0], mValues[1][1] + m.mValues[1][1], mValues[1][2] + m.mValues[1][2],
              mValues[2][0] + m.mValues[2][0], mValues[2][1] + m.mValues[2][1], mValues[2][2] + m.mValues[2][2] );

    return *this;
}

Matrix3 &Matrix3::operator-=( const Matrix3 &m )
{
    SetValue( mValues[0][0] - m.mValues[0][0], mValues[0][1] - m.mValues[0][1], mValues[0][2] - m.mValues[0][2],
              mValues[1][0] - m.mValues[1][0], mValues[1][1] - m.mValues[1][1], mValues[1][2] - m.mValues[1][2],
              mValues[2][0] - m.mValues[2][0], mValues[2][1] - m.mValues[2][1], mValues[2][2] - m.mValues[2][2] );

    return *this;
}

bool Matrix3::operator==( const Matrix3 &m ) const
{
    return mValues[0] == m.mValues[0] && mValues[1] == m.mValues[1] && mValues[2] == m.mValues[2];
}

bool Matrix3::operator!=( const Matrix3 &m ) const
{
    return !( *this == m );
}

void Matrix3::SetColumn( const U8 column, const Vec3 &v )
{
    mValues[0][column] = v[0];
    mValues[1][column] = v[1];
    mValues[2][column] = v[2];
}

Vec3 Matrix3::GetColumn( const U8 column ) const
{
    return Vec3( mValues[0][column], mValues[1][column], mValues[2][column] );
}

void Matrix3::SetRow( const U8 row, const Vec3 &v )
{
    mValues[row] = v;
}

Vec3 Matrix3::GetRow( const U8 row ) const
{
    return mValues[row];
}

void Matrix3::SetRotation( const Quaternion &q )
{
    const F32 norm2 = q.Length2();

    assert( norm2 != 0.0f );

    const F32 s     = 2.0f / norm2;

    const F32 xs  = q[0] * s;
    const F32 ys  = q[1] * s;
    const F32 zs  = q[2] * s;
    const F32 wxs = q[3] * xs;
    const F32 wys = q[3] * ys;
    const F32 wzs = q[3] * zs;
    const F32 xxs = q[0] * xs;
    const F32 xys = q[0] * ys;
    const F32 xzs = q[0] * zs;
    const F32 yys = q[1] * ys;
    const F32 yzs = q[1] * zs;
    const F32 zzs = q[2] * zs;

    SetValue( 1.0f - ( yys + zzs ), xys - wzs, xzs + wys,
              xys + wzs, 1.0f - ( xxs + zzs ), yzs - wxs,
              xzs - wys, yzs + wxs, 1.0f - ( xxs + yys ) );
}

Quaternion Matrix3::GetRotation() const
{
    F32 t[4];
    const F32 trace = mValues[0][0] + mValues[1][1] + mValues[2][2];


    if ( trace > 0.0f )
    {
        const F32 s  = Mathf::Sqrt( trace + 1.0f );
        const F32 s2 = 0.5f / s;

        t[0] = ( mValues[2][1] - mValues[1][2] ) * s2;
        t[1] = ( mValues[0][2] - mValues[2][0] ) * s2;
        t[2] = ( mValues[1][0] - mValues[0][1] ) * s2;
        t[3] = 0.5f * s;
    }
    else
    {
        const U8 i = mValues[0][0] < mValues[1][1] ?
                     ( mValues[1][1] < mValues[2][2] ? 2 : 1 ) :
                     ( mValues[0][0] < mValues[2][2] ? 2 : 0 );

        const U8 j = ( i + 1 ) % 3;
        const U8 k = ( i + 2 ) % 3;

        const F32 s = Mathf::Sqrt( mValues[i][i] - mValues[j][j] - mValues[k][k] + 1.0f );

        assert( s != 0.0f );

        const F32 s2 = 0.5f / s;

        t[i] = 0.5f * s;
        t[j] = ( mValues[j][i] - mValues[i][j] ) * s2;
        t[k] = ( mValues[k][i] - mValues[i][k] ) * s2;
        t[3] = ( mValues[k][j] - mValues[j][k] ) * s2;
    }

    return Quaternion( t[0], t[1], t[2], t[3] );
}

void Matrix3::SetEuler( const F32 yaw, const F32 pitch, const F32 roll )
{
    F32 cy( Mathf::Cos( yaw ) );
    F32 cp( Mathf::Cos( pitch ) );
    F32 cr( Mathf::Cos( roll ) );

    F32 sy( Mathf::Sin( yaw ) );
    F32 sp( Mathf::Sin( pitch ) );
    F32 sr( Mathf::Sin( roll ) );


    F32 cc = cr * cy;
    F32 cs = cr * sy;
    F32 sc = sr * cy;
    F32 ss = sr * sy;

    SetValue( cp * cy, sp * sc - cs, sp * cc + ss,
              cp * sy, sp * ss + cc, sp * cs - sc,
              -sp, cp * sr, cp * cr );
}

Vec3 Matrix3::GetEuler() const
{
    return GetRotation().GetAxis();
}

Matrix3 Matrix3::Scale( const Vec3 &v ) const
{
    return Matrix3( mValues[0][0] * v[0], mValues[0][1] * v[1], mValues[0][2] * v[2],
                    mValues[1][0] * v[0], mValues[1][1] * v[1], mValues[1][2] * v[2],
                    mValues[2][0] * v[0], mValues[2][1] * v[1], mValues[2][2] * v[2] );
}

Matrix3 Matrix3::GetTranspose() const
{
    return Matrix3( mValues[0][0], mValues[1][0], mValues[2][0],
                    mValues[0][1], mValues[1][1], mValues[2][1],
                    mValues[0][2], mValues[1][2], mValues[2][2] );
}

Matrix3 Matrix3::GetAbsolute() const
{
    return Matrix3( Mathf::Abs( mValues[0][0] ), Mathf::Abs( mValues[0][1] ), Mathf::Abs( mValues[0][2] ),
                    Mathf::Abs( mValues[1][0] ), Mathf::Abs( mValues[1][1] ), Mathf::Abs( mValues[1][2] ),
                    Mathf::Abs( mValues[2][0] ), Mathf::Abs( mValues[2][1] ), Mathf::Abs( mValues[2][2] ) );
}

Matrix3 Matrix3::GetAdjoint() const
{
    return Matrix3( GetCofactor( 0, 0 ), GetCofactor( 0, 1 ), GetCofactor( 0, 2 ),
                    GetCofactor( 1, 0 ), GetCofactor( 1, 1 ), GetCofactor( 1, 2 ),
                    GetCofactor( 2, 0 ), GetCofactor( 2, 1 ), GetCofactor( 2, 2 ) );
}

Matrix3 Matrix3::GetInverse() const
{
    const F32 det = GetDeterminant();

    assert( det != 0.0f );

    const F32 s = 1.0f / det;
    return GetAdjoint().GetTranspose() * s;
}

void Matrix3::SetIdentity()
{
    SetValue( 1.0f, 0.0f, 0.0f,
              0.0f, 1.0f, 0.0f,
              0.0f, 0.0f, 1.0f );
}

F32 Matrix3::GetCofactor( const U8 row, const U8 column ) const
{
    F32 cof = mValues[( row + 1 ) % 3][( column + 1 ) % 3] * mValues[( row + 2 ) % 3][( column + 2 ) % 3] -
              mValues[( row + 1 ) % 3][( column + 2 ) % 3] * mValues[( row + 2 ) % 3][( column + 1 ) % 3];

    if ( ( ( column + row ) & 1 ) == 1 )
    {
        cof *= -1;
    }

    return cof;
}

F32 Matrix3::GetDeterminant() const
{
    return  mValues[0][0] * GetCofactor( 0, 0 ) +
            mValues[1][0] * GetCofactor( 1, 0 ) +
            mValues[2][0] * GetCofactor( 2, 0 );
}

void Matrix3::SetValue( const F32 a11, const F32 a12, const F32 a13,
                        const F32 a21, const F32 a22, const F32 a23,
                        const F32 a31, const F32 a32, const F32 a33 )
{
    mValues[0].SetValue( a11, a12, a13 );
    mValues[1].SetValue( a21, a22, a23 );
    mValues[2].SetValue( a31, a32, a33 );
}

F32 Matrix3::Dotx( const Vec3 &v ) const
{
    return mValues[0][0] * v[0] + mValues[1][0] * v[1] + mValues[2][0] * v[2];
}

F32 Matrix3::Doty( const Vec3 &v ) const
{
    return mValues[0][1] * v[0] + mValues[1][1] * v[1] + mValues[2][1] * v[2];
}

F32 Matrix3::Dotz( const Vec3 &v ) const
{
    return mValues[0][2] * v[0] + mValues[1][2] * v[1] + mValues[2][2] * v[2];
}

Matrix3 Matrix3::GetZero()
{
    return Matrix3( Vec3::GetZero(), Vec3::GetZero(), Vec3::GetZero() );
}

Matrix3 Matrix3::GetIdentity()
{
    Matrix3 mat;
    mat.SetIdentity();
    return mat;
}

Matrix3 operator+( const Matrix3 &m1, const Matrix3 &m2 )
{
    return Matrix3( m1.mValues[0][0] + m2.mValues[0][0], m1.mValues[0][1] + m2.mValues[0][1],
                    m1.mValues[0][2] + m2.mValues[0][2],
                    m1.mValues[1][0] + m2.mValues[1][0], m1.mValues[1][1] + m2.mValues[1][1],
                    m1.mValues[1][2] + m2.mValues[1][2],
                    m1.mValues[2][0] + m2.mValues[2][0], m1.mValues[2][1] + m2.mValues[2][1],
                    m1.mValues[2][2] + m2.mValues[2][2] );
}

Matrix3 operator-( const Matrix3 &m1, const Matrix3 &m2 )
{
    return Matrix3( m1.mValues[0][0] - m2.mValues[0][0], m1.mValues[0][1] - m2.mValues[0][1],
                    m1.mValues[0][2] - m2.mValues[0][2],
                    m1.mValues[1][0] - m2.mValues[1][0], m1.mValues[1][1] - m2.mValues[1][1],
                    m1.mValues[1][2] - m2.mValues[1][2],
                    m1.mValues[2][0] - m2.mValues[2][0], m1.mValues[2][1] - m2.mValues[2][1],
                    m1.mValues[2][2] - m2.mValues[2][2] );
}

Matrix3 operator*( const Matrix3 &m1, const Matrix3 &m2 )
{
    return Matrix3( m2.Dotx( m1.mValues[0] ), m2.Doty( m1.mValues[0] ), m2.Dotz( m1.mValues[0] ),
                    m2.Dotx( m1.mValues[1] ), m2.Doty( m1.mValues[1] ), m2.Dotz( m1.mValues[1] ),
                    m2.Dotx( m1.mValues[2] ), m2.Doty( m1.mValues[2] ), m2.Dotz( m1.mValues[2] ) );
}

Matrix3 operator*( const Matrix3 &m, const F32 s )
{
    return Matrix3( m.mValues[0][0] * s, m.mValues[0][1] * s, m.mValues[0][2] * s,
                    m.mValues[1][0] * s, m.mValues[1][1] * s, m.mValues[1][2] * s,
                    m.mValues[2][0] * s, m.mValues[2][1] * s, m.mValues[2][2] * s );
}

Vec3 operator*( const Matrix3 &m, const Vec3 &v )
{
    return Vec3( m.mValues[0].Dot( v ), m.mValues[1].Dot( v ), m.mValues[2].Dot( v ) );
}
