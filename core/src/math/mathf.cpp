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

#include <assert.h>
#include <cstdlib>
#include <limits>

#define _USE_MATH_DEFINES
#include <math.h>

F32 Mathf::GetEpsilon()
{
    return std::numeric_limits<F32>::epsilon();
}

F32 Mathf::GetPi()
{
    return ( F32 )M_PI;
}

F32 Mathf::Get2Pi()
{
    return ( F32 )M_PI_2;
}

F32 Mathf::GetSqrt2()
{
    return ( F32 )M_SQRT2;
}

F32 Mathf::GetE()
{
    return ( F32 )M_E;
}

F32 Mathf::GetLog2E()
{
    return ( F32 )M_LOG2E;
}

F32 Mathf::GetLog10E()
{
    return ( F32 )M_LOG10E;
}

F32 Mathf::GetLn2()
{
    return ( F32 )M_LN2;
}

F32 Mathf::GetLn10()
{
    return ( F32 )M_LN10;
}

F32 Mathf::Abs( const F32 f )
{
    return fabsf( f );
}

U32 Mathf::Abs( const U32 f )
{
    return abs( ( int )f );
}

F32 Mathf::Acos( const F32 f )
{
    assert( f >= -1.0f && f <= 1.0f );
    return acosf( f );
}

F32 Mathf::Asin( const F32 f )
{
    assert( f >= -1.0f && f <= 1.0f );
    return asinf( f );
}

F32 Mathf::Atan( const F32 f )
{
    return atanf( f );
}

F32 Mathf::Atan2( const F32 x, const F32 y )
{
    return atan2f( x, y );
}

F32 Mathf::Cos( const F32 f )
{
    return cosf( f );
}

F32 Mathf::Sin( const F32 f )
{
    return sinf( f );
}

F32 Mathf::Tan( const F32 f )
{
    return tanf( f );
}

F32 Mathf::Exp( const F32 f )
{
    return expf( f );
}

F32 Mathf::Ceil( const F32 f )
{
    return ceilf( f );
}

F32 Mathf::Floor( const F32 f )
{
    return floorf( f );
}

F32 Mathf::Round( const F32 f )
{
    return f >= 0.0 ? f + 0.5f : ( ( f - ( F32 )( S32 )f ) <= -0.5 ? f : f - 0.5f );
}

F32 Mathf::FMod( const F32 f, const F32 m )
{
    return std::fmodf( f, m );
}

F32 Mathf::Log( const F32 f )
{
    assert( f > 0.0f );
    return logf( f );
}

F32 Mathf::Log2( const F32 f )
{
    assert( f > 0.0f );
    return logf( f ) / logf( 2 );
}

U32 Mathf::Log2( U64 n )
{
    static const U32 tab64[64] =
    {
        0, 58, 1, 59, 47, 53, 2, 60, 39, 48, 27, 54, 33, 42, 3, 61,
        51, 37, 40, 49, 18, 28, 20, 55, 30, 34, 11, 43, 14, 22, 4, 62,
        57, 46, 52, 38, 26, 32, 41, 50, 36, 17, 19, 29, 10, 13, 21, 56,
        45, 25, 31, 35, 16, 9, 12, 44, 24, 15, 8, 23, 7, 6, 5, 63
    };

    n |= n >> 1;
    n |= n >> 2;
    n |= n >> 4;
    n |= n >> 8;
    n |= n >> 16;
    n |= n >> 32;
    return tab64[( n * 0x03f6eaf2cd271461 ) >> 58];
}

U32 Mathf::Log2( U32 f )
{
    U32 s, t;

    t = ( f > 0xffff ) << 4;
    f >>= t;
    s = ( f > 0xff ) << 3;
    f >>= s, t |= s;
    s = ( f > 0xf ) << 2;
    f >>= s, t |= s;
    s = ( f > 0x3 ) << 1;
    f >>= s, t |= s;

    return ( t | ( f >> 1 ) );
}


F32 Mathf::Log10( const F32 f )
{
    assert( f > 0.0f );
    return log10f( f );
}

F32 Mathf::Pow( const F32 base, const F32 exp )
{
    return powf( base, exp );
}

F32 Mathf::Sqrt( const F32 f )
{
    assert( f >= 0.0f );
    return sqrtf( f );
}

F32 Mathf::Square( F32 f )
{
    return f * f;
}

F32 Mathf::Clamp( const F32 f, const F32 min, const F32 max )
{
    return f < min ? min : ( f > max ? max : f );
}

F32 Mathf::RadToDeg( const F32 f )
{
    return f * 180.0f / GetPi();
}

F32 Mathf::DegToRad( const F32 f )
{
    return f * GetPi() / 180.0f;
}

F32 Mathf::Lerp( const F32 a, const F32 b, const F32 t )
{
    return a + ( b - a ) * t;
}

bool Mathf::IsPow2( const U32 n )
{
    return ( !( n & ( n - 1 ) ) && n );
}

U32 Mathf::NextPow2( const U32 x )
{
    U32 y = x - 1;
    y |= y >> 1;
    y |= y >> 2;
    y |= y >> 4;
    y |= y >> 8;
    y |= y >> 16;
    return ++y;
}

bool Mathf::Equal( const F32 a, const F32 b )
{
    return fabs( a - b ) < GetEpsilon();
}
