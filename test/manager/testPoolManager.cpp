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

#include "manager/poolManager.h"


#include "engineTest.h"

namespace
{
    class PoolTest
    {
    public:

        void OnInit()
        {
            init = true;
        }

        void OnRelease()
        {
            released = true;
        }

        bool released = false;
        bool init = false;
    };

    TEST(PoolManager, Sanity)
    {
        PoolManager m;

        EXPECT_FALSE(m.HasPools(0));
    }

    TEST(PoolManager, Add)
    {
        PoolManager m;
        auto pool = m.Add< PoolTest >();

        EXPECT_TRUE(m.Has< PoolTest >());
        EXPECT_EQ(pool, m.Get< PoolTest >());

        EXPECT_TRUE(m.HasPools(0));
    }

    TEST(PoolManager, AddFromFactory)
    {
        PoolManager m;

        FactoryManager f;
        f.Add< U32 >();

        ManagerHolder h = {};
        h.factory = &f;
        m.SetManagers(&h);

        auto pool = m.AddFromFactory< U32 >();

        EXPECT_TRUE(m.Has< U32 >());
        EXPECT_EQ(pool, m.Get< U32 >());

        EXPECT_TRUE(m.HasPools(0));

        f.ClearAll();
        ASSERT_FALSE(f.Has< U32 >());
    }

    TEST(PoolManager, AddFromFactory2)
    {
        PoolManager m;

        FactoryManager f;
        f.Add< U32 >();

        ManagerHolder h = {};
        h.factory = &f;
        m.SetManagers(&h);

        m.AddFromFactory< U32 >();
        EXPECT_ANY_THROW(m.AddFromFactory< U32 >());
    }

    TEST(PoolManager, AddPoolTwice)
    {
        PoolManager m;
        m.Add< PoolTest >();
        EXPECT_ANY_THROW(m.Add< PoolTest >());
    }

    TEST(PoolManager, Release)
    {
        PoolManager m;
        auto pool = m.Add< PoolTest >();

        EXPECT_TRUE(m.Has< PoolTest >());
        EXPECT_EQ(pool, m.Get< PoolTest >());

        m.Remove< PoolTest >();

        EXPECT_FALSE(m.Has< PoolTest >());
        EXPECT_EQ(nullptr, m.Get< PoolTest >());
    }

    TEST(PoolManager, ReleasePoolNS)
    {
        PoolManager m;
        auto pool = m.Add< PoolTest >();

        EXPECT_TRUE(m.Has< PoolTest >());
        EXPECT_EQ(pool, m.Get< PoolTest >());


        auto pool2 = m.Add< PoolTest >(1);

        EXPECT_TRUE(m.Has< PoolTest >(1));
        EXPECT_EQ(pool2, m.Get< PoolTest >(1));

        m.Remove< PoolTest >();

        EXPECT_FALSE(m.Has< PoolTest >());
        EXPECT_EQ(nullptr, m.Get< PoolTest >());

        EXPECT_TRUE(m.Has< PoolTest >(1));
        EXPECT_EQ(pool2, m.Get< PoolTest >(1));
    }

    TEST(PoolManager, ReleasePools)
    {
        PoolManager m;
        auto pool = m.Add< PoolTest >();

        EXPECT_TRUE(m.Has< PoolTest >());
        EXPECT_EQ(pool, m.Get< PoolTest >());


        auto pool2 = m.Add< PoolTest >(1);

        EXPECT_TRUE(m.Has< PoolTest >(1));
        EXPECT_EQ(pool2, m.Get< PoolTest >(1));

        m.ClearAll(1);

        EXPECT_FALSE(m.Has< PoolTest >(1));
        EXPECT_EQ(nullptr, m.Get< PoolTest >(1));

        EXPECT_TRUE(m.Has< PoolTest >());
        EXPECT_EQ(pool, m.Get< PoolTest >());
    }

    TEST(PoolManager, OnReleaseNS)
    {
        PoolManager m;
        auto pool = m.Add< PoolTest >();

        EXPECT_TRUE(m.Has< PoolTest >());
        EXPECT_EQ(pool, m.Get< PoolTest >());


        auto pool2 = m.Add< PoolTest >(1);

        EXPECT_TRUE(m.Has< PoolTest >(1));
        EXPECT_EQ(pool2, m.Get< PoolTest >(1));

        m.OnRelease(1);

        EXPECT_FALSE(m.Has< PoolTest >(1));
        EXPECT_EQ(nullptr, m.Get< PoolTest >(1));

        EXPECT_TRUE(m.Has< PoolTest >());
        EXPECT_EQ(pool, m.Get< PoolTest >());

        EXPECT_TRUE(m.HasPools(0));
        EXPECT_FALSE(m.HasPools(1));
    }
}
