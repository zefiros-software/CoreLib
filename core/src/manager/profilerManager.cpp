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

#include "manager/profilerManager.h"
#include "manager/eventManager.h"

#include "events/profileEvents.h"

#include "api/console.h"

#include "preproc/env.h"

ProfilerManager::ProfilerManager()
{
    StartMemoryLeakDetection();

    mLastUpdate = std::chrono::high_resolution_clock::now();
}


void ProfilerManager::OnUpdate()
{
    std::chrono::time_point< std::chrono::high_resolution_clock > time = std::chrono::high_resolution_clock::now();

    GetManagers()->event->Post( ProfileUpdateEvent( time,
                                                    std::chrono::duration_cast< std::chrono::microseconds >( mLastUpdate - time ) ) );

    mLastUpdate = time;
}

void ProfilerManager::Start( const std::string &name )
{
    std::chrono::time_point< std::chrono::high_resolution_clock > time = std::chrono::high_resolution_clock::now();
    {
        std::lock_guard< std::mutex > lock( mMutex );

        auto it = mTimings.find( name );

        if ( it == mTimings.end() )
        {
            mTimings[ name ] = time;
        }
        else
        {
            Console::Warningf( LOG( "Profile request '%s' already started." ), name );
        }
    }

    GetManagers()->event->Post( ProfileStartEvent( name, time ) );
}

void ProfilerManager::Waypoint( const std::string &name, const std::string &comment )
{
    std::chrono::microseconds duration;
    std::chrono::time_point< std::chrono::high_resolution_clock > time = std::chrono::high_resolution_clock::now();
    {
        std::lock_guard< std::mutex > lock( mMutex );

        auto it = mTimings.find( name );

        if ( it != mTimings.end() )
        {
            duration = std::chrono::duration_cast<std::chrono::microseconds>( time - it->second );

            GetManagers()->event->Post( ProfileWaypointEvent( name, comment, time, duration ) );
        }
        else
        {
            Console::Warningf( LOG( "Trying to waypoint an unknown profile request '%s'." ), name );
        }
    }
}

void ProfilerManager::End( const std::string &name )
{
    std::chrono::microseconds duration;
    std::chrono::time_point< std::chrono::high_resolution_clock > time = std::chrono::high_resolution_clock::now();

    {
        std::lock_guard< std::mutex > lock( mMutex );

        auto it = mTimings.find( name );

        if ( it != mTimings.end() )
        {
            duration = std::chrono::duration_cast< std::chrono::microseconds >( time - it->second );
            mTimings.erase( it );

            GetManagers()->event->Post( ProfileEndEvent( name, time, duration ) );
        }
        else
        {
            Console::Warningf( LOG( "Trying to end unknown profile request '%s'." ), name );
        }
    }
}

void ProfilerManager::StartMemoryLeakDetection()
{
#if defined(COMP_IS_MSVC) && IS_DEBUG

    _CrtSetDbgFlag( _CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF );
    _CrtSetReportMode( _CRT_ASSERT, _CRTDBG_MODE_FILE );
    _CrtSetReportFile( _CRT_ASSERT, _CRTDBG_FILE_STDERR );
    //_crtBreakAlloc = 0;

#endif

}
