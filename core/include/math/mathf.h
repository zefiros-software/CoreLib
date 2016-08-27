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
#ifndef __ENGINE_MATHF_H__
#define __ENGINE_MATHF_H__

#include "common/types.h"

namespace Mathf
{
    F32 GetEpsilon();
    F32 GetPi();
    F32 Get2Pi();
    F32 GetSqrt2();
    F32 GetE();
    F32 GetLog2E();
    F32 GetLog10E();
    F32 GetLn2();
    F32 GetLn10();

    F32 Acos( const F32 f );
    F32 Asin( const F32 f );
    F32 Atan( const F32 f );
    F32 Atan2( const F32 x, const F32 y );
    F32 Cos( const F32 f );
    F32 Sin( const F32 f );
    F32 Tan( const F32 f );

    F32 Exp( const F32 f );
    F32 Log( const F32 f );
    F32 Log2( const F32 f );
    U32 Log2( const U32 f );
    U32 Log2( U64 n );
    F32 Log10( const F32 f );
    F32 Pow( const F32 base, const F32 exp );
    F32 Sqrt( const F32 f );

    F32 Square( F32 f );

    F32 Clamp( const F32 f, const F32 min, const F32 max );
    F32 Lerp( const F32 a, const F32 b, const F32 t );
    F32 RadToDeg( const F32 f );
    F32 DegToRad( const F32 f );
    F32 Abs( const F32 f );
    U32 Abs( const U32 f );
    F32 Ceil( const F32 f );
    F32 Floor( const F32 f );
    F32 Round( const F32 f );

    F32 FMod( const F32 f, const F32 m );

    bool IsPow2( const U32 n );
    U32 NextPow2( const U32 x );

    bool Equal( const F32 a, const F32 b );

    template< typename tT >
    tT GetMin( const tT &a, const tT &b )
    {
        return a < b ? a : b;
    }

    template< typename tT >
    tT GetMax( const tT &a, const tT &b )
    {
        return a > b ? a : b;
    }
}

#endif