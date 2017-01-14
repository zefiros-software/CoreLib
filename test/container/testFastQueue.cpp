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

#include "container/fastQueue.h"

#include "engineTest.h"

namespace
{
    TEST( FastQueue, SanityCheck )
    {
        FastQueue< U8 > m;
        EXPECT_TRUE( m.IsEmpty() );
    }

    TEST( FastQueue, Queue )
    {
        FastQueue< U8 > m;
        m.Queue( 1 );
        m.Queue( 2 );
        m.Queue( 3 );
        m.Queue( 4 );
        m.Queue( 5 );

        EXPECT_EQ( 1, m.Front() );
    }

    TEST( FastQueue, IsEmpty )
    {
        FastQueue< U8 > m;
        m.Queue( 1 );
        EXPECT_FALSE( m.IsEmpty() );
    }

    TEST( FastQueue, IsEmpty2 )
    {
        FastQueue< U8 > m;
        m.Queue( 1 );
        m.Dequeue();
        EXPECT_TRUE( m.IsEmpty() );
    }

    TEST( FastQueue, Dequeue )
    {
        FastQueue< U8 > m;
        m.Queue( 1 );
        m.Queue( 2 );
        m.Queue( 3 );
        m.Queue( 4 );
        m.Queue( 5 );

        EXPECT_EQ( 1, m.Front() );
        m.Dequeue();
        EXPECT_EQ( 2, m.Front() );
    }
}
