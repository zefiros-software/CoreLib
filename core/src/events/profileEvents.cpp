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

#include "events/profileEvents.h"

ProfileEvent::ProfileEvent( const std::string &name,
                            std::chrono::time_point< std::chrono::high_resolution_clock > time ) noexcept
    : mName( name ),
      mTime( time )
{

}

std::string ProfileEvent::GetName() const noexcept
{
    return mName;
}

std::chrono::time_point< std::chrono::high_resolution_clock > ProfileEvent::GetTime() const noexcept
{
    return mTime;
}

ProfileStartEvent::ProfileStartEvent( const std::string &name,
                                      std::chrono::time_point< std::chrono::high_resolution_clock > time ) noexcept
    : ProfileEvent( name, time )
{

}

ProfileEndEvent::ProfileEndEvent( const std::string &name,
                                  std::chrono::time_point< std::chrono::high_resolution_clock > time,
                                  std::chrono::microseconds duration ) noexcept
    : ProfileEvent( name, time ),
      mDuration( duration )
{

}

std::chrono::microseconds ProfileEndEvent::GetDuration() const noexcept
{
    return mDuration;
}

ProfileWaypointEvent::ProfileWaypointEvent( const std::string &name, const std::string &comment,
                                            std::chrono::time_point< std::chrono::high_resolution_clock > time,
                                            std::chrono::microseconds duration ) noexcept
    : ProfileEndEvent( name, time, duration ),
      mComment( comment )
{

}

std::string ProfileWaypointEvent::GetCommment() const noexcept
{
    return mComment;
}

ProfileUpdateEvent::ProfileUpdateEvent( std::chrono::time_point< std::chrono::high_resolution_clock > time,
                                        std::chrono::microseconds duration ) noexcept
    : mDuration( duration ),
      mTime( time )
{

}

std::chrono::time_point< std::chrono::high_resolution_clock > ProfileUpdateEvent::GetTime() const noexcept
{
    return mTime;
}

std::chrono::microseconds ProfileUpdateEvent::GetDuration() const noexcept
{
    return mDuration;
}
