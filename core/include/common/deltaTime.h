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

#pragma once
#ifndef __ENGINE_DELTATIME_H__
#define __ENGINE_DELTATIME_H__

#include "external/chrono.h"

#include <deque>

/// @addtogroup docCommon
/// @{

/// @addtogroup docCommon_Time
/// @{

/**
 * An object with the ability to calculate the delta time since the last time Update() was called.
 */

class DeltaTime
{
public:

    /**
     * Sets the members to their default values.
     *
     * @param   maxsize The maximum records kept for the delta time easing algorithm, this value should satisfy @f$ 1
     *                  \leq n \leq 255 @f$. We default it to 60, so we get the average over the last second of game time,
     *                  when running a game on 60 fps.
     */

    DeltaTime( U8 maxsize = 60 ) noexcept;

    /// @name Modifiers
    /// @{

    /**
     * Updates both the delta time and the eased delta time, using the last time they were updated.
     *
     * @post
     * * Both the 'GetDeltaTime()' and 'GetEasedDeltaTime()' will be updated to represent the new values since the
     *   last update.
     * * 'GetElaspedMilliseconds()' will now return the time since this call.
     *
     * @details
     * @examples
     *
     * @snippet testDeltaTime.cpp Update
     */

    void Update() noexcept;

    /// @}

    /// @name Queries
    /// @{

    /**
     * Gets the raw delta time between the last requested Update()'s. This function will never return 0.0f, since this
     * may cause divisions by zero. Instead we will return Mathf::GetEpsilon() if this would be the case.
     *
     * @return The delta time in seconds.
     *
     * @details
     * @examples
     *
     * @snippet testDeltaTime.cpp Get
     */

    F32 GetDeltaTime() const noexcept;

    /**
     * Gets elapsed time in miliseconds since the last Update(), so when called twice, the value of the second call
     * would be greater or equal than the first call. This function will never return 0.0f, since this may cause
     * divisions by zero. Instead we will return Mathf::GetEpsilon() if this would be the case.
     *
     * @return The elapsed time in milliseconds.
     *
     * @details
     * @examples
     * * Does not return 0.0f:
     * @snippet testDeltaTime.cpp GetMS
     * * Does give the actual time since the last Update() call.
     * @snippet testDeltaTime.cpp UpdateMS
     */

    F32 GetElapsedMilliseconds() const noexcept;

    /**
     * Gets the eased delta time between the last requested Update()'s, by using a moving average.
     *
     * The moving average follows the following formula:
     *
     * @f$ \Delta t_{\text{eased}} = \frac{1}{n}\displaystyle \sum\limits_{i=0}^{ \xi } \Delta t_{n - i} @f$
     *
     * With the following conditions:
     *  - @f$ \sum\limits_{i=0}^\xi \Delta t_{n - i} \approx 1.0 @f$
     *  - @f$ \xi \ge 1 @f$
     *
     * Where:
     *  - @f$ n @f$     The amount of delta time records kept.
     *  - @f$ m @f$     The maximum record count, as given in the constructor.
     *  - @f$ \xi  @f$  Is @f$ \min\{n,m\} @f$.
     *
     * Or in plain english:
     *      We calculate the average of the last kept records limited by either the maximum record count or the amount
     *      of records needed so that the sum of the records approximates 1.
     *
     * @return The eased delta time.
     *
     * @details
     * @examples
     *
     * @snippet testDeltaTime.cpp Get
     */

    F32 GetEasedDeltaTime() const noexcept;

    /// @}

private:

    /// Holds a record of the previous calculated delta times
    std::deque< F32 > mPreviousDeltaTimes;

    /// Holds the current time in microseconds
    boost::chrono::time_point< boost::chrono::high_resolution_clock > mTime;

    /// The total sum of the recorded delta times
    F32 mTotalDeltaTimeValue;

    /// Holds the eased delta time, this variable is used as a cache
    /// so we don't have to recalculate the delta time on every request
    F32 mEasedDeltaTime;

    /// Holds the delta time, this variable is used as a cache
    /// so we don't have to recalculate the delta time on every request
    F32 mDeltaTime;

    /// Holds the maximum size of the moving average queue
    U8 mMaxSize;

    /**
     * Updates the delta time variable.
     */

    void UpdateDeltaTime() noexcept;

    /**
     * Updates the eased delta time variable.
     */

    void UpdateEasedDeltaTime() noexcept;

};

/// @}

/// @}

#endif