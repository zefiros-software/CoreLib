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

#pragma once
#ifndef __ENGINE_PROFILEEVENTS_H__
#define __ENGINE_PROFILEEVENTS_H__

#include "events/abstract/IEvent.h"

#include <string>
#include <chrono>

class ProfileEvent
    : public IEvent
{
public:

    ProfileEvent(const std::string &name, std::chrono::time_point< std::chrono::high_resolution_clock > time) noexcept;

    std::string GetName() const noexcept;

    std::chrono::time_point< std::chrono::high_resolution_clock > GetTime() const noexcept;

private:

    std::string mName;
    std::chrono::time_point< std::chrono::high_resolution_clock > mTime;
};

class ProfileStartEvent
    : public ProfileEvent
{
public:

    ProfileStartEvent(const std::string &name,
                      std::chrono::time_point< std::chrono::high_resolution_clock > time) noexcept;
};

class ProfileEndEvent
    : public ProfileEvent
{
public:

    ProfileEndEvent(const std::string &name, std::chrono::time_point< std::chrono::high_resolution_clock > time,
                    std::chrono::microseconds duration) noexcept;

    std::chrono::microseconds GetDuration() const noexcept;

private:

    std::chrono::microseconds mDuration;
};


class ProfileWaypointEvent
    : public ProfileEndEvent
{
public:

    ProfileWaypointEvent(const std::string &name, const std::string &comment,
                         std::chrono::time_point< std::chrono::high_resolution_clock > time,
                         std::chrono::microseconds duration) noexcept;


    std::string GetCommment() const noexcept;

private:

    std::string mComment;
};


class ProfileUpdateEvent
    : public IEvent
{
public:

    ProfileUpdateEvent(std::chrono::time_point< std::chrono::high_resolution_clock > time,
                       std::chrono::microseconds duration) noexcept;

    std::chrono::time_point< std::chrono::high_resolution_clock > GetTime() const noexcept;

    std::chrono::microseconds GetDuration() const noexcept;

private:

    std::chrono::microseconds mDuration;
    std::chrono::time_point< std::chrono::high_resolution_clock > mTime;
};

#endif