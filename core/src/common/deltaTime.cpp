/**
 * @cond ___LICENSE___
 *
 * Copyright (c) 2016 Koen Visscher, Paul Visscher and individual contributors.
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

#include "common/deltaTime.h"

#include "math/mathf.h"

DeltaTime::DeltaTime( U8 maxsize /* = 60 */ ) noexcept
    : mTotalDeltaTimeValue( 0 ),

      // Give the delta times a really small value, to prevent
      // a division by zero.
      mEasedDeltaTime( Mathf::GetEpsilon() ),
      mDeltaTime( Mathf::GetEpsilon() ),
      mMaxSize( maxsize )
{
    assert( maxsize > 1 );

    // Get the current time
    mTime = boost::chrono::high_resolution_clock::now();
}

void DeltaTime::Update()
{
    // We need to update the normal delta time first,
    // since the eased delta time depends on it
    UpdateDeltaTime();
    UpdateEasedDeltaTime();
}

F32 DeltaTime::GetElapsedMilliseconds() const
{
    const boost::chrono::time_point< boost::chrono::high_resolution_clock> now =
        boost::chrono::high_resolution_clock::now();

    boost::chrono::microseconds diff = boost::chrono::duration_cast<boost::chrono::microseconds>( now - mTime );

    const F32 elapsed = static_cast< F32 >( diff.count() / 1000.0f );

    return elapsed != 0.0 ? elapsed : Mathf::GetEpsilon();
}

F32 DeltaTime::GetDeltaTime() const
{
    return mDeltaTime;
}

F32 DeltaTime::GetEasedDeltaTime() const
{
    // If we don't have any records, just return the normal delta time
    if ( mPreviousDeltaTimes.empty() )
    {
        return GetDeltaTime();
    }

    return mEasedDeltaTime;
}

void DeltaTime::UpdateDeltaTime()
{
    const boost::chrono::time_point< boost::chrono::high_resolution_clock > now =
        boost::chrono::high_resolution_clock::now();

    boost::chrono::duration<float> diff = now - mTime;
    // Calculate the delta time and convert it to seconds
    mDeltaTime = diff.count();

    // Prevent delta time from being zero
    mDeltaTime = mDeltaTime != 0.0 ? mDeltaTime : Mathf::GetEpsilon();

    // Update our time, so we can calculate the amount
    // of milliseconds since our last update
    mTime = now;
}

void DeltaTime::UpdateEasedDeltaTime()
{
    mPreviousDeltaTimes.push_back( mDeltaTime );

    mTotalDeltaTimeValue += mDeltaTime;

    // Make sure the total sum of the delta times we recorded is never larger than one.
    // If it is bigger, remove records until the specifications are met.
    // Also make sure we don't store more values than we want to.
    while ( ( mTotalDeltaTimeValue > 1.0f && !mPreviousDeltaTimes.empty() ) ||
            mPreviousDeltaTimes.size() > static_cast<size_t >( mMaxSize ) )
    {
        mTotalDeltaTimeValue -= mPreviousDeltaTimes.front();
        mPreviousDeltaTimes.pop_front();
    }

    // Calculate our eased delta time.
    mEasedDeltaTime = mTotalDeltaTimeValue / mPreviousDeltaTimes.size();
}
