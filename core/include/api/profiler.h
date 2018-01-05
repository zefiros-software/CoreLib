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
#ifndef __ENGINE_PROFILER_H__
#define __ENGINE_PROFILER_H__

#include "manager/profilerManager.h"

#include "api/expose.h"

/**
 * Prefer the macros over the functions, so we can
 * filter out the profiling when we actually ship a
 * game, and limit wasted time.
 */
#ifndef ENGINE_SHIPVERSION
#   define ProfileStart( name ) Profiler::Start( name )
#   define ProfileWaypoint( name, comment ) Profiler::Waypoint( name, comment )
#   define ProfileEnd( name ) Profiler::End( name )
#else
#   define ProfileStart( name )
#   define ProfileWaypoint( name, comment )
#   define ProfileEnd( name )
#endif

namespace Profiler
{
    EXPOSE_API(profile, Start);

    EXPOSE_API(profile, Waypoint);

    EXPOSE_API(profile, End);
}

#endif