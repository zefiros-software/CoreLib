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

#include "events/profileEvents.h"

#include "engineTest.h"


namespace
{
    TEST(ProfileEvent, SanityCheck)
    {
        auto now = std::chrono::high_resolution_clock::now();
        ProfileEvent a("Now", now);
        EXPECT_EQ("Now", a.GetName());
        EXPECT_EQ(now, a.GetTime());
    }

    TEST(ProfileStartEvent, SanityCheck)
    {
        auto now = std::chrono::high_resolution_clock::now();
        ProfileStartEvent a("Now", now);
        EXPECT_EQ("Now", a.GetName());
        EXPECT_EQ(now, a.GetTime());
    }

    TEST(ProfileEndEvent, SanityCheck)
    {
        auto now = std::chrono::high_resolution_clock::now();
        ProfileEndEvent a("Now", now, std::chrono::microseconds(1));
        EXPECT_EQ("Now", a.GetName());
        EXPECT_EQ(now, a.GetTime());
        EXPECT_EQ(std::chrono::microseconds(1), a.GetDuration());
    }

    TEST(ProfileWaypointEvent, SanityCheck)
    {
        auto now = std::chrono::high_resolution_clock::now();
        ProfileWaypointEvent a("Now", "Comment", now, std::chrono::microseconds(1));
        EXPECT_EQ("Now", a.GetName());
        EXPECT_EQ("Comment", a.GetCommment());
        EXPECT_EQ(now, a.GetTime());
    }

    TEST(ProfileUpdateEvent, SanityCheck)
    {
        auto now = std::chrono::high_resolution_clock::now();
        ProfileUpdateEvent a(now, std::chrono::microseconds(1));
        EXPECT_EQ(now, a.GetTime());
        EXPECT_EQ(std::chrono::microseconds(1), a.GetDuration());
    }
}