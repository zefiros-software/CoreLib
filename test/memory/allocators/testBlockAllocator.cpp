/**
 * @cond ___LICENSE___
 *
 * Copyright (c) 2017 Koen Visscher, Paul Visscher and individual contributors.
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

#include "memory/allocators/blockAllocator.h"

#include "engineTest.h"

namespace
{
    TEST( BlockAllocator, SanityCheck )
    {
        BlockAllocator b( 4 );
    }

    TEST( BlockAllocator, Alloc )
    {
        BlockAllocator s( 4 );
        EXPECT_EQ( 0, s.Alloc<U8>() );
        EXPECT_EQ( 1, s.Alloc<U8>() );
        EXPECT_EQ( 4, s.Alloc<U64>() );
        EXPECT_EQ( 8, s.Alloc<U64>() );
    }

    TEST( BlockAllocator, Move )
    {
        U64 f = 503;
        BlockAllocator s( 4 );
        EXPECT_EQ( 0, s.Alloc<U8>() );
        EXPECT_EQ( 1, s.Alloc<U8>() );
        EXPECT_EQ( 4, s.Move( f ) );
        EXPECT_EQ( 8, s.Alloc<U64>() );

        EXPECT_EQ( f, *s.Extract<U64>( 4 ) );
    }

    TEST( BlockAllocator, Move2 )
    {
        U64 f = 503;
        BlockAllocator s( 4 );
        EXPECT_EQ( 0, s.Alloc<U8>() );
        EXPECT_EQ( 4, s.Move( f ) );
        EXPECT_EQ( 12, s.Alloc<U8>() );
        EXPECT_EQ( 8, s.Alloc<U64>() );

        EXPECT_EQ( f, *( ( const BlockAllocator & )s ).Extract<U64>( 4 ) );
    }

    TEST( BlockAllocator, Clear )
    {
        const U8 f[2] = { 8, 14 };
        BlockAllocator s( 1 );
        BlockAllocator::BlockLocation l = s.Move( f[0] );
        BlockAllocator::BlockLocation l2 = s.Move( f[1] );

        EXPECT_EQ( 8, *s.Extract<U8>( l ) );
        EXPECT_EQ( 14, *s.Extract<U8>( l2 ) );

        s.Clear();

        const U8 f2[2] = { 12, 80 };
        BlockAllocator::BlockLocation fl = s.Move( f2[0] );
        BlockAllocator::BlockLocation fl2 = s.Move( f2[1] );

        EXPECT_EQ( 12, *s.Extract<U8>( fl ) );
        EXPECT_EQ( 80, *s.Extract<U8>( fl2 ) );
    }
}