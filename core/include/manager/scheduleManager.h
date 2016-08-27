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
#ifndef __ENGINE_SCHEDULEMANAGER_H__
#define __ENGINE_SCHEDULEMANAGER_H__

#include "threading/threadPool.h"
#include "threading/jobQueue.h"
#include "threading/barrier.h"
#include "threading/worker.h"

#include "manager/abstract/abstractManager.h"

#include <future>

class ScheduleManager
    : public AbstractManager
{
public:

    ScheduleManager();

    virtual void OnPreInit() override;

    virtual void OnRelease() override;

    virtual void OnUpdate() override;

    virtual void OnSynchronise() override;

    virtual void OnProcessEvents() override;

    void RunEventJobs();

    bool RegisterJob( IThreadExecutable *const job, IThreadExecutable::Type type = IThreadExecutable::Type::Worker );

    bool RegisterJob( IThreadExecutable *job, U32 threadGroupID );

    bool RegisterSynchronisationJob( IThreadExecutable *job, U32 threadGroupID );

    void RunMainJobs();

    void RunWorkerJobs();

    void RunSynchronisationJobs();

    void RunLoaderJobs();

    void RunWorkerThreadGroupJobs();

    void RunSynchronisationThreadGroupJobs();

    void RunMainWorkerQueue( JobQueue *queue );

    static ThreadID GetCurrentThreadID();

    static void SetCurrentThreadID( const ThreadID threadID );

    static U32 GetMainThreadID();

    static U32 GetThreadCount();

    template< typename tT >
    void SetGlobalThreadID()
    {
        // one for the main thread
        const ThreadID threadCount = static_cast< ThreadID >( GetThreadCount() + 1 );
        SpinBarrier barrier( threadCount );
        JobQueue queue;

        std::vector< GlobalThreadIDJob< tT > > threadJobs( threadCount, GlobalThreadIDJob< tT >( this, &barrier ) );

        for ( GlobalThreadIDJob< tT > &job : threadJobs )
        {
            queue.Push( &job );
        }

        queue.Flush();
        mThreadPool.Run( &queue );

        RunMainWorkerQueue( &queue );

        mThreadPool.JoinAll();
    }

private:

    template< typename tT >
    class GlobalThreadIDJob
        : public IThreadExecutable
    {
    public:

        explicit GlobalThreadIDJob( ScheduleManager *scheduleManager, SpinBarrier *barrier )
            : mScheduleManager( scheduleManager ),
              mBarrier( barrier )
        {

        }

        virtual void OnStartJob( ThreadID threadID ) override
        {
            mScheduleManager->SetCurrentThreadID( threadID );
        }

        virtual void OnRunJob() override
        {
            // one ID per thread :)
            mBarrier->Wait();
        }

    private:

        ScheduleManager *mScheduleManager;
        SpinBarrier *mBarrier;
    };

    ThreadPool mThreadPool;

    std::unordered_map< U32, JobQueue > mSyncThreadGroups;
    std::unordered_map< U32, JobQueue > mWorkerThreadGroups;

    JobQueue mWorkerQueue;
    JobQueue mMainThreadQueue;
    JobQueue mLoaderQueue;

    JobQueue mSyncQueue;
    JobQueue mEventQueue;

    JobQueue *mLoaderHook;
    Worker mLoaderWorker;

    std::future< void > mLoaderThread;
    bool mLoaderIsRunning;

    void RunThreadGroups( std::unordered_map< U32, JobQueue > &queues );

};



#endif