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

#include "manager/scheduleManager.h"
#include "manager/eventManager.h"

#include "events/threadEvents.h"

#include "preproc/env.h"

#include "config.h"

#if OS_IS_WINDOWS
__declspec( thread ) ThreadID gThreadID = Thread::InvalidID;
#else
__thread ThreadID gThreadID = Thread::InvalidID;
#endif

ScheduleManager::ScheduleManager()
    :  mThreadPool( GetThreadCount(), 1 ),
       mLoaderHook( &mLoaderQueue ),
       mLoaderWorker( &mLoaderHook ),
       mLoaderIsRunning( false )
{
}

void ScheduleManager::OnPreInit()
{
    mThreadPool.Init();
    SetCurrentThreadID( Thread::MainThreadID );
}

void ScheduleManager::OnRelease()
{
    mThreadPool.JoinAll();

    if ( mLoaderThread.valid() )
    {
        mLoaderThread.wait();
    }
}

void ScheduleManager::OnUpdate()
{
    RunLoaderJobs();

    RunWorkerJobs();

    RunMainJobs();

    // Let the main thread help
    RunMainWorkerQueue( &mWorkerQueue );

    mThreadPool.JoinAll();

    RunWorkerThreadGroupJobs();

    // workers are clock synchronised so join them
    mThreadPool.JoinAll();

    GetManagers()->event->Post( ThreadingEvent( false ) );
}

void ScheduleManager::OnSynchronise()
{
    RunSynchronisationJobs();

    mThreadPool.JoinAll();

    RunSynchronisationThreadGroupJobs();

    mThreadPool.JoinAll();
}

void ScheduleManager::OnProcessEvents()
{
    RunEventJobs();
}

bool ScheduleManager::RegisterJob( IThreadExecutable *const job, IThreadExecutable::Type type )
{
    switch ( type )
    {
    case IThreadExecutable::Type::Worker:
        mWorkerQueue.Push( job );
        break;

    case IThreadExecutable::Type::Loader:
        mLoaderQueue.Push( job );
        break;

    case IThreadExecutable::Type::Main:
        mMainThreadQueue.Push( job );
        break;

    case IThreadExecutable::Type::Synchronisation:
        mSyncQueue.Push( job );
        break;

    case IThreadExecutable::Type::Event:
        mEventQueue.Push( job );
        break;

    default:
        return false;
    }

    return true;
}

bool ScheduleManager::RegisterJob( IThreadExecutable *job, U32 threadGroupID )
{
    auto it = mWorkerThreadGroups.find( threadGroupID );

    if ( it == mWorkerThreadGroups.end() )
    {
        JobQueue queue;
        queue.Push( job );
        mWorkerThreadGroups.emplace( threadGroupID, std::move( queue ) );
    }
    else
    {
        it->second.Push( job );
    }

    return true;
}

bool ScheduleManager::RegisterSynchronisationJob( IThreadExecutable *job, U32 threadGroupID )
{
    auto it = mSyncThreadGroups.find( threadGroupID );

    if ( it == mSyncThreadGroups.end() )
    {
        JobQueue queue;
        queue.Push( job );
        mSyncThreadGroups.emplace( threadGroupID, std::move( queue ) );
    }
    else
    {
        it->second.Push( job );
    }

    return true;
}

void ScheduleManager::RunLoaderJobs()
{
    if ( !mLoaderIsRunning )
    {
        mLoaderQueue.Flush();

        if ( mLoaderQueue.Size() > 0 )
        {
            mLoaderIsRunning = true;

            try
            {
                mLoaderThread = std::async( std::launch::async, &Worker::RunJobs, mLoaderWorker, Thread::LoaderID );
            }
            catch ( const std::error_code & )
            {
                mLoaderWorker.RunJobs( Thread::FailedLoaderID );
                mLoaderIsRunning = false;
            }
        }

    }

    if ( mLoaderThread.valid() )
    {
        switch ( mLoaderThread.wait_for( std::chrono::seconds( 0 ) ) )
        {
        case std::future_status::ready:
            mLoaderIsRunning = false;
            break;

        case std::future_status::timeout:
            // Still computing
            break;

        case std::future_status::deferred:
            // Not yet started
            break;
        }
    }
    else
    {
        mLoaderIsRunning = false;
    }
}

void ScheduleManager::RunWorkerThreadGroupJobs()
{
    RunThreadGroups( mWorkerThreadGroups );
}

void ScheduleManager::RunSynchronisationThreadGroupJobs()
{
    RunThreadGroups( mSyncThreadGroups );
}

U32 ScheduleManager::GetThreadCount()
{
    return std::min< U32 >( std::max< U32 >( std::thread::hardware_concurrency(), 2 ) - 1, PROGRAM_MAX_THREADS );
}

void ScheduleManager::RunMainJobs()
{
    mMainThreadQueue.Flush();

    RunMainWorkerQueue( &mMainThreadQueue );
}

void ScheduleManager::RunWorkerJobs()
{
    mWorkerQueue.Flush();
    mThreadPool.Run( &mWorkerQueue );

    GetManagers()->event->Post( ThreadingEvent( true ) );
}

void ScheduleManager::RunEventJobs()
{
    mEventQueue.Flush();

    RunMainWorkerQueue( &mEventQueue );
}

void ScheduleManager::RunSynchronisationJobs()
{
    mSyncQueue.Flush();
    mThreadPool.Run( &mSyncQueue );

    RunMainWorkerQueue( &mSyncQueue );
}

U32 ScheduleManager::GetMainThreadID()
{
    return Thread::MainThreadID;
}

void ScheduleManager::RunMainWorkerQueue( JobQueue *queue ) const
{
    Worker mainThreadWorker( &queue );
    mainThreadWorker.RunJobs( Thread::MainThreadID );
}

ThreadID ScheduleManager::GetCurrentThreadID()
{
    return gThreadID;
}

void ScheduleManager::SetCurrentThreadID( const ThreadID threadID )
{
    gThreadID = threadID;
}

void ScheduleManager::RunThreadGroups( std::unordered_map< U32, JobQueue > &queues )
{
    for ( auto &job : queues )
    {
        JobQueue *queue = &job.second;

        queue->Flush();

        if ( queue->Size() > 0 )
        {
            mThreadPool.Run( queue );
            RunMainWorkerQueue( queue );

            mThreadPool.JoinAll();
        }
    }

    queues.clear();
}
