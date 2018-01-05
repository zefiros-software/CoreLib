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

#include "threading/threadPtr.h"


#include "engineTest.h"


namespace
{
    TEST(ThreadPtr, Sanity)
    {
        ThreadPtr< U8 > p;
    }

    TEST(ThreadPtr, Constr)
    {
        U8 val = 0;
        ThreadPtr< U8 > p(&val);
    }

    TEST(ThreadPtr, OnSynchronise)
    {
        U8 val = 0;
        ThreadPtr< U8 > p(&val);
        p.OnSynchronise();

        EXPECT_EQ(nullptr, p.UnsynchronisedGet());
    }

    TEST(ThreadPtr, OnSynchronise2)
    {
        U8 val = 0;
        ThreadPtr< U8 > p;
        p.Set(&val);
        p.OnSynchronise();

        EXPECT_EQ(&val, p.UnsynchronisedGet());
    }

    TEST(ThreadPtr, SetTwice)
    {
        U8 val = 0;
        U8 val2 = 0;
        ThreadPtr< U8 > p;
        p.Set(&val);
        p.Set(&val2);
        p.OnSynchronise();

        EXPECT_EQ(&val, p.UnsynchronisedGet());
    }

    TEST(ThreadPtr, Get)
    {
        U8 val = 0;
        ThreadPtr< U8 > p;
        p.Set(&val);
        p.OnSynchronise();

        EXPECT_EQ(&val, p.Get());
    }

    TEST(ThreadPtr, Get2)
    {
        U8 val = 0;
        ThreadPtr< U8 > p;
        p.Set(&val);

        EXPECT_EQ(&val, p.Get());
    }

    TEST(ThreadPtr, Get3)
    {
        U8 val = 0;
        ThreadPtr< U8 > p;
        p.Set(&val);

        EXPECT_EQ(&val, p.Get());

        std::thread t([&]
        {
            SystemManager::Get()->GetManagers()->schedule->SetCurrentThreadID(1);
            EXPECT_EQ(nullptr, p.Get());
        });

        t.join();
    }

    TEST(ThreadPtr, Get4)
    {
        U8 val = 0;
        ThreadPtr< U8 > p;
        p.ForcedUnsynchronisedSet(&val);

        EXPECT_EQ(&val, p.Get());

        std::thread t([&]
        {
            SystemManager::Get()->GetManagers()->schedule->SetCurrentThreadID(1);
            EXPECT_EQ(&val, p.Get());
        });

        t.join();
    }

    TEST(ThreadPtr, Op)
    {
        U8 val = 0;
        ThreadPtr< U8 > p;
        p.ForcedUnsynchronisedSet(&val);

        EXPECT_EQ(&val, p);
    }

    TEST(ThreadPtr, Op2)
    {
        U32 val = ::Test::GenerateRandomU32();
        ThreadPtr< U32 > p;
        p.ForcedUnsynchronisedSet(&val);

        EXPECT_EQ(val, *p);
    }

    TEST(ThreadPtr, Op3)
    {
        U32 val = ::Test::GenerateRandomU32();
        ThreadPtr< U32 > p;
        p.ForcedUnsynchronisedSet(&val);

        EXPECT_EQ(&val, p.operator->());
    }

}
