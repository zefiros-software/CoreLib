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

#include "common/nullable.h"

#include "engineTest.h"

namespace
{
    TEST(Nullable, SanityCheck)
    {
        Nullable< U8 > m;
        EXPECT_TRUE(m.IsNull());
    }

    TEST(Nullable, SanityCheck2)
    {
        Nullable< U8 > m(50);
        EXPECT_FALSE(m.IsNull());
        EXPECT_EQ(50, m);
    }

    TEST(Nullable, Op)
    {
        Nullable< U8 > m(50);
        EXPECT_FALSE(m.IsNull());
        m = 100;
        EXPECT_EQ(100, m);
    }

    TEST(Nullable, SetNull)
    {
        Nullable< U8 > m(50);
        EXPECT_FALSE(m.IsNull());
        EXPECT_TRUE(m.SetNull().IsNull());
        EXPECT_TRUE(m.IsNull());
    }

    TEST(Nullable, Clear)
    {
        Nullable< U8 > m(50);
        EXPECT_FALSE(m.IsNull());
        m.Clear();
        EXPECT_TRUE(m.IsNull());
    }

    TEST(Nullable, Copy)
    {
        Nullable< U8 > m(50);
        Nullable< U8 > m2(20);
        EXPECT_FALSE(m.IsNull());
        m = m2;
        EXPECT_FALSE(m.IsNull());
    }

    TEST(Nullable, Copy2)
    {
        Nullable< U8 > m(50);
        Nullable< U8 > m2;
        EXPECT_FALSE(m.IsNull());
        m = m2;
        EXPECT_TRUE(m.IsNull());
    }

    TEST(Nullable, Get)
    {
        Nullable< U8 > m(50);
        Nullable< U8 > m2;
        EXPECT_EQ(50, m.Get());
        EXPECT_EQ(50, m.Get(100));
        EXPECT_EQ(100, m2.Get(100));
    }

    TEST(Nullable, Eq)
    {
        Nullable< U8 > m(50);
        Nullable< U8 > m2(50);
        EXPECT_EQ(m, m2);
    }

    TEST(Nullable, Eq2)
    {
        Nullable< U8 > m;
        Nullable< U8 > m2;
        EXPECT_EQ(m, m2);
    }

    TEST(Nullable, Eq3)
    {
        Nullable< U8 > m(50);
        EXPECT_EQ(m, Nullable< U8 > {50});
    }

    TEST(Nullable, Ne)
    {
        Nullable< U8 > m(50);
        Nullable< U8 > m2;
        EXPECT_NE(m, m2);
    }

    TEST(Nullable, Ne2)
    {
        Nullable< U8 > m;
        Nullable< U8 > m2(50);
        EXPECT_NE(m, m2);
    }

    TEST(Nullable, Ne3)
    {
        Nullable< U8 > m(100);
        Nullable< U8 > m2(50);
        EXPECT_NE(m, m2);
    }

    TEST(Nullable, Ge)
    {
        Nullable< U8 > m(100);
        Nullable< U8 > m2(50);
        EXPECT_TRUE(m > m2);
    }

    TEST(Nullable, Ge2)
    {
        Nullable< U8 > m(100);
        Nullable< U8 > m2(50);
        EXPECT_FALSE(m2 > m);
    }

    TEST(Nullable, Ge3)
    {
        Nullable< U8 > m(100);
        Nullable< U8 > m2(100);
        EXPECT_FALSE(m2 > m);
    }

    TEST(Nullable, Le)
    {
        Nullable< U8 > m(100);
        Nullable< U8 > m2(50);
        EXPECT_TRUE(m2 < m);
    }

    TEST(Nullable, Le2)
    {
        Nullable< U8 > m(100);
        Nullable< U8 > m2(50);
        EXPECT_FALSE(m < m2);
    }

    TEST(Nullable, Le3)
    {
        Nullable< U8 > m(100);
        Nullable< U8 > m2(100);
        EXPECT_FALSE(m < m2);
    }

    TEST(Nullable, Le4)
    {
        Nullable< U8 > m;
        Nullable< U8 > m2(100);
        EXPECT_TRUE(m < m2);
    }

    TEST(Nullable, Le5)
    {
        Nullable< U8 > m;
        Nullable< U8 > m2;
        EXPECT_FALSE(m < m2);
    }

    TEST(Nullable, Le6)
    {
        Nullable< U8 > m(100);
        Nullable< U8 > m2;
        EXPECT_FALSE(m < m2);
    }
}