/**
 * @cond ___LICENSE___
 *
 * Copyright (c) 2016-2018 Zefiros Software.
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

#include "memory/allocators/stackAllocator.h"

#include "engineTest.h"

namespace
{
    TEST(StackAllocator, SanityCheck)
    {
        StackAllocator();
    }

    TEST(StackAllocator, SanityCheck2)
    {
        StackAllocator(5);
    }

    TEST(StackAllocator, Alloc)
    {
        StackAllocator s(4);
        EXPECT_EQ(0, s.Alloc<U8>());
        EXPECT_TRUE(s.FitsInStack(3));
        EXPECT_EQ(1, s.Alloc<U8>());
        EXPECT_TRUE(s.FitsInStack(2));
        EXPECT_EQ(2, s.Alloc<U8>());
        EXPECT_TRUE(s.FitsInStack(1));
        EXPECT_EQ(3, s.Alloc<U8>());
        EXPECT_FALSE(s.FitsInStack(1));

        EXPECT_EQ(4, s.Alloc<U8>());
        EXPECT_TRUE(s.FitsInStack(1));
    }

    TEST(StackAllocator, Alloc2)
    {
        StackAllocator s(4);
        EXPECT_EQ(0, s.Alloc<U8>());
        EXPECT_TRUE(s.FitsInStack(3));
        EXPECT_EQ(1, s.Alloc<U8>());
        EXPECT_TRUE(s.FitsInStack(2));
        EXPECT_EQ(2, s.Alloc<U64>());
        EXPECT_TRUE(s.FitsInStack(1));
        EXPECT_EQ(10, s.Alloc<U8>());
        EXPECT_TRUE(s.FitsInStack(1));

        EXPECT_EQ(11, s.Alloc<U8>());
        EXPECT_TRUE(s.FitsInStack(1));
    }

    TEST(StackAllocator, Move)
    {
        U8 f = 8;
        StackAllocator s(4);
        StackAllocator::StackLocation l = s.Move(f);

        f += 1;

        EXPECT_EQ(8, *s.Extract<U8>(l));
    }

    TEST(StackAllocator, Move2)
    {
        const U8 f[2] = {8, 14};
        StackAllocator s(4);
        StackAllocator::StackLocation l = s.Move(f[0]);
        StackAllocator::StackLocation l2 = s.Move(f[1]);

        s.Alloc< U64 >();

        EXPECT_EQ(8, *s.Extract<U8>(l));
        EXPECT_EQ(14, *s.Extract<U8>(l2));
    }

    TEST(StackAllocator, Extract)
    {
        const U8 f = 8;
        StackAllocator s(4);
        StackAllocator::StackLocation l = s.Move(f);

        EXPECT_EQ((const U8)8, *((const StackAllocator &)s).Extract<U8>(l));
    }

    TEST(StackAllocator, Clear)
    {
        const U8 f[2] = { 8, 14 };
        StackAllocator s(4);
        StackAllocator::StackLocation l = s.Move(f[0]);
        StackAllocator::StackLocation l2 = s.Move(f[1]);

        EXPECT_EQ(8, *s.Extract<U8>(l));
        EXPECT_EQ(14, *s.Extract<U8>(l2));

        s.Clear();

        const U8 f2[2] = { 12, 80 };
        StackAllocator::StackLocation fl = s.Move(f2[0]);
        StackAllocator::StackLocation fl2 = s.Move(f2[1]);

        EXPECT_EQ(12, *s.Extract<U8>(fl));
        EXPECT_EQ(80, *s.Extract<U8>(fl2));
    }

}
