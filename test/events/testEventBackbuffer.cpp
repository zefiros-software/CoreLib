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

#include "events/eventBackBuffer.h"

#include "engineTest.h"


namespace
{
    class EventTest
        : public IEvent
    {
    public:

        EventTest()
            : mVal(::Test::GenerateRandomU32())
        {
        }

        uint32_t mVal;
    };

    class Callback
    {
    public:

        Callback()
            : mVal(0),
              mWithArg(false)
        {
        }

        void OnArg(const EventTest &ev)
        {
            mWithArg = true;
            mVal = ev.mVal;
        }

        uint32_t mVal;
        bool mWithArg;
    };

    TEST(EventBackBuffer, SanityCheck)
    {
        EventBackBuffer< Callback, EventTest > m;
    }

    TEST(EventBackBuffer, SetObserver)
    {
        Callback c;
        EventBackBuffer< Callback, EventTest > m;
        m.Observe(&Callback::OnArg, &c);

        EXPECT_TRUE(m.IsEmpty());

        EventTest e;
        m.Notify(e);

        EXPECT_EQ(0u, c.mVal);
        EXPECT_FALSE(c.mWithArg);

        EXPECT_FALSE(m.IsEmpty());

        m.Flush();

        EXPECT_EQ(c.mVal, e.mVal);
        EXPECT_TRUE(c.mWithArg);

        EXPECT_TRUE(m.IsEmpty());
    }

    TEST(EventBackBuffer, Unobserve)
    {
        Callback c;
        EventBackBuffer< Callback, EventTest > m;
        m.Observe(&Callback::OnArg, &c);
        EventTest e;
        m.Notify(e);

        EXPECT_EQ(0u, c.mVal);
        EXPECT_FALSE(c.mWithArg);

        m.Unobserve();
        m.Flush();

        EXPECT_EQ(0u, c.mVal);
        EXPECT_FALSE(c.mWithArg);
    }

    TEST(EventBackBuffer, Clear)
    {
        Callback c;
        EventBackBuffer< Callback, EventTest > m;
        m.Observe(&Callback::OnArg, &c);
        EventTest e;
        m.Notify(e);

        EXPECT_EQ(0u, c.mVal);
        EXPECT_FALSE(c.mWithArg);

        m.Clear();

        EXPECT_TRUE(m.IsEmpty());

        m.Flush();

        EXPECT_EQ(0u, c.mVal);
        EXPECT_FALSE(c.mWithArg);
    }
}