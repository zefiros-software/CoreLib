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


#include "warnings/push.h"
#include "warnings/integerToSmallPointer.h"

#include "engineTest.h"

#include "warnings/pop.h"

namespace
{

    ENGINE_TEST( S8, Traits )
    {
        EXPECT_TRUE( std::is_integral< S8 >::value );
        EXPECT_TRUE( std::is_signed< S8 >::value );
        EXPECT_EQ( 1, sizeof( S8 ) );
    }

    ENGINE_TEST( U8, Traits )
    {
        EXPECT_TRUE( std::is_integral< U8 >::value );
        EXPECT_TRUE( std::is_unsigned< U8 >::value );
        EXPECT_EQ( 1, sizeof( U8 ) );
    }

    ENGINE_TEST( S16, Traits )
    {
        EXPECT_TRUE( std::is_integral< S16 >::value );
        EXPECT_TRUE( std::is_signed< S16 >::value );
        EXPECT_EQ( 2, sizeof( S16 ) );
    }

    ENGINE_TEST( U16, Traits )
    {
        EXPECT_TRUE( std::is_integral< U16 >::value );
        EXPECT_TRUE( std::is_unsigned< U16 >::value );
        EXPECT_EQ( 2, sizeof( U16 ) );
    }

    ENGINE_TEST( S32, Traits )
    {
        EXPECT_TRUE( std::is_integral< S32 >::value );
        EXPECT_TRUE( std::is_signed< S32 >::value );
        EXPECT_EQ( 4, sizeof( S32 ) );
    }

    ENGINE_TEST( U32, Traits )
    {
        EXPECT_TRUE( std::is_integral< U32 >::value );
        EXPECT_TRUE( std::is_unsigned< U32 >::value );
        EXPECT_EQ( 4, sizeof( U32 ) );
    }

    ENGINE_TEST( S64, Traits )
    {
        EXPECT_TRUE( std::is_integral< S64 >::value );
        EXPECT_TRUE( std::is_signed< S64 >::value );
        EXPECT_EQ( 8, sizeof( S64 ) );
    }

    ENGINE_TEST( U64, Traits )
    {
        EXPECT_TRUE( std::is_integral< U64 >::value );
        EXPECT_TRUE( std::is_unsigned< U64 >::value );
        EXPECT_EQ( 8, sizeof( U64 ) );
    }

    ENGINE_TEST( F32, Traits )
    {
        EXPECT_TRUE( std::is_floating_point< F32 >::value );
        EXPECT_EQ( 4, sizeof( F32 ) );
    }

    ENGINE_TEST( F64, Traits )
    {
        EXPECT_TRUE( std::is_floating_point< F64 >::value );
        EXPECT_EQ( 8, sizeof( F64 ) );
    }

}