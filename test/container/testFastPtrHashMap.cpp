/**
 * @cond ___LICENSE___
 *
 * Copyright (c) 2017 Zefiros Software
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

#include "container/fastPtrHashMap.h"

#include "engineTest.h"

namespace
{
    TEST( FastPtrHashMap, SanityCheck )
    {
        FastPtrHashMap< U8, U8 > m;
    }

    TEST( FastPtrHashMap, SanityCheck2 )
    {
        FastPtrHashMap< U8, U8 > m( 5 );
    }

    TEST( FastPtrHashMap, Insert )
    {
        U8 *ptr = new U8;
        FastPtrHashMap< U8, U8 > m;
        m.Insert( ptr, 5 );

        U8 *val = nullptr;
        EXPECT_TRUE( m.Find( ptr, val ) );
        EXPECT_TRUE( m.Has( ptr ) );

        EXPECT_EQ( 5, *val );

        delete ptr;
    }

    TEST( FastPtrHashMap, InsertTwice )
    {
        U8 *ptr = new U8;
        FastPtrHashMap< U8, U8 > m;
        m.Insert( ptr, 5 );
        m.Insert( ptr, 50 );

        U8 *val = nullptr;
        EXPECT_TRUE( m.Find( ptr, val ) );
        EXPECT_TRUE( m.Has( ptr ) );

        EXPECT_EQ( 5, *val );

        delete ptr;
    }

    TEST( FastPtrHashMap, InsertFull )
    {
        U8 *ptr = new U8;
        U8 *ptr2 = new U8;
        FastPtrHashMap< U8, U8 > m( 1 );
        m.Insert( ptr, 5 );
        m.Insert( ptr2, 50 );

        U8 *val = nullptr;
        U8 *val2 = nullptr;
        EXPECT_TRUE( m.Find( ptr, val ) );
        EXPECT_TRUE( m.Has( ptr ) );

        EXPECT_TRUE( m.Find( ptr2, val2 ) );
        EXPECT_TRUE( m.Has( ptr2 ) );

        EXPECT_EQ( 5, *val );
        EXPECT_EQ( 50, *val2 );

        delete ptr;
        delete ptr2;
    }

    TEST( FastPtrHashMap, ResizeSmaller )
    {
        FastPtrHashMap< U8, U8 > m;
        m.Resize( 0 );
    }

    TEST( FastPtrHashMap, ResizeConsistent )
    {
        U8 *ptr = new U8;
        FastPtrHashMap< U8, U8 > m;
        m.Insert( ptr, 5 );
        m.Resize( 10000 );

        U8 *val = nullptr;
        EXPECT_TRUE( m.Find( ptr, val ) );
        EXPECT_TRUE( m.Has( ptr ) );

        EXPECT_EQ( 5, *val );

        delete ptr;
    }

    TEST( FastPtrHashMap, HasNotExists )
    {
        U8 *ptr = new U8;
        FastPtrHashMap< U8, U8 > m;
        EXPECT_FALSE( m.Has( ptr ) );

        delete ptr;
    }

    TEST( FastPtrHashMap, FindNotExists )
    {
        U8 *ptr = new U8;
        FastPtrHashMap< U8, U8 > m;
        U8 *val = nullptr;
        EXPECT_FALSE( m.Find( ptr, val ) );
        EXPECT_EQ( nullptr, val );

        delete ptr;
    }

    TEST( FastPtrHashMap, Data )
    {
        U8 *ptr = new U8;
        U8 *ptr2 = new U8;
        FastPtrHashMap< U8, U8 > m( 1 );
        m.Insert( ptr, 5 );
        m.Insert( ptr2, 50 );

        auto data = m.Data();
        EXPECT_EQ( 2u, data.size() );
        EXPECT_EQ( 5u, data[0].element );
        EXPECT_EQ( ptr, data[0].ptr );

        EXPECT_EQ( 50u, data[1].element );
        EXPECT_EQ( ptr2, data[1].ptr );

        delete ptr;
        delete ptr2;
    }

    TEST( FastPtrHashMap, DataConst )
    {
        U8 *ptr = new U8;
        U8 *ptr2 = new U8;
        FastPtrHashMap< U8, U8 > m( 1 );
        m.Insert( ptr, 5 );
        m.Insert( ptr2, 50 );

        auto data = ( ( const FastPtrHashMap< U8, U8 > & )m ).Data();
        EXPECT_EQ( 2, data.size() );
        EXPECT_EQ( 5, data[0].element );
        EXPECT_EQ( ptr, data[0].ptr );

        EXPECT_EQ( 50, data[1].element );
        EXPECT_EQ( ptr2, data[1].ptr );

        delete ptr;
        delete ptr2;
    }

}