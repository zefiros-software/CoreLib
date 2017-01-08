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

#include "container/unorderedContiguousSet.h"

#include "engineTest.h"

namespace
{
    TEST( UnorderedContiguousSet, SanityCheck )
    {
        UnorderedContiguousSet< U8 > f;
        EXPECT_EQ( 0, f.Size() );
    }

    TEST( UnorderedContiguousSet, Insert )
    {
        UnorderedContiguousSet< U8 > f;
        EXPECT_EQ( 0, f.Size() );
        EXPECT_TRUE( f.Insert( 2 ) );
        EXPECT_TRUE( f.Insert( 4 ) );
        EXPECT_TRUE( f.Has( 2 ) );
        EXPECT_FALSE( f.Has( 3 ) );
        EXPECT_EQ( 2, f.Size() );
    }

    TEST( UnorderedContiguousSet, InsertDouble )
    {
        UnorderedContiguousSet< U8 > f;
        EXPECT_EQ( 0, f.Size() );
        EXPECT_TRUE( f.Insert( 2 ) );
        EXPECT_FALSE( f.Insert( 2 ) );
        EXPECT_TRUE( f.Has( 2 ) );
        EXPECT_EQ( 1, f.Size() );
    }

    TEST( UnorderedContiguousSet, GetValues )
    {
        UnorderedContiguousSet< U8 > f;
        EXPECT_EQ( 0, f.Size() );
        EXPECT_TRUE( f.Insert( 2 ) );
        EXPECT_TRUE( f.Insert( 4 ) );
        auto matcher = ::testing::UnorderedElementsAre< U8, U8 >( 2u, 4u );
        EXPECT_THAT( f.GetValues(), matcher );
    }

    TEST( UnorderedContiguousSet, GetValuesConst )
    {
        UnorderedContiguousSet< U8 > f;
        EXPECT_EQ( 0, f.Size() );
        EXPECT_TRUE( f.Insert( 2 ) );
        EXPECT_TRUE( f.Insert( 4 ) );
        auto matcher = ::testing::UnorderedElementsAre< U8, U8 >( 2u, 4u );
        EXPECT_THAT( ( ( const UnorderedContiguousSet<U8> & )f ).GetValues(), matcher );
    }

    TEST( UnorderedContiguousSet, Delete )
    {
        UnorderedContiguousSet< U8 > f;
        EXPECT_TRUE( f.Insert( 2 ) );
        EXPECT_TRUE( f.Insert( 4 ) );
        auto matcher = ::testing::UnorderedElementsAre< U8, U8 >( 2u, 4u );
        EXPECT_THAT( ( ( const UnorderedContiguousSet<U8> & )f ).GetValues(), matcher );

        EXPECT_TRUE( f.Delete( 4 ) );
        EXPECT_FALSE( f.Has( 4 ) );
        EXPECT_EQ( 1, f.Size() );
        auto matcher2 = ::testing::UnorderedElementsAre< U8 >( 2u );
        EXPECT_THAT( ( ( const UnorderedContiguousSet<U8> & )f ).GetValues(), matcher2 );
    }

    TEST( UnorderedContiguousSet, DeleteNotExists )
    {
        UnorderedContiguousSet< U8 > f;
        EXPECT_TRUE( f.Insert( 2 ) );
        EXPECT_TRUE( f.Insert( 4 ) );
        auto matcher = ::testing::UnorderedElementsAre< U8, U8 >( 2u, 4u );
        EXPECT_THAT( ( ( const UnorderedContiguousSet<U8> & )f ).GetValues(), matcher );

        EXPECT_FALSE( f.Delete( 20 ) );
        EXPECT_EQ( 2, f.Size() );
        EXPECT_THAT( ( ( const UnorderedContiguousSet<U8> & )f ).GetValues(), matcher );
    }

    TEST( UnorderedContiguousSet, Clear )
    {
        UnorderedContiguousSet< U8 > f;
        EXPECT_TRUE( f.Insert( 2 ) );
        EXPECT_TRUE( f.Insert( 4 ) );
        auto matcher = ::testing::UnorderedElementsAre< U8, U8 >( 2u, 4u );
        EXPECT_THAT( ( ( const UnorderedContiguousSet<U8> & )f ).GetValues(), matcher );

        f.Clear();

        EXPECT_EQ( 0, f.Size() );
        EXPECT_FALSE( f.Has( 2 ) );
        EXPECT_FALSE( f.Has( 4 ) );
        EXPECT_EQ( 0, f.GetValues().size() );
    }
}
