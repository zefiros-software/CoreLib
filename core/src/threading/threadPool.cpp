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

#include "threading/threadPool.h"
#include "threading/jobQueue.h"
#include "threading/worker.h"


ThreadPool::ThreadPool(const U32 capacity /*= 16*/, const U32 startThreadID /*= 0 */) noexcept
    : mQueueHook(nullptr),
      mLastQueueSize(0),
      mStartThreadID(startThreadID)
{
    mWorkers.resize(capacity, Worker(&mQueueHook));
    mThreads.reserve(capacity);
}

ThreadPool::~ThreadPool() noexcept
{
    std::unique_lock<std::mutex> lock(mMutex);

    //set safe to nullptr
    mQueueHook = nullptr;

    lock.unlock();

    for (auto it = mWorkers.begin(), end = mWorkers.end(); it != end; ++it)
    {
        it->Terminate();
    }

    mNotification.notify_all();

    for (auto it = mThreads.begin(), end = mThreads.end(); it != end; ++it)
    {
        if (it->joinable())
        {
            it->join();
        }
    }
}

void ThreadPool::Init()
{
    U32 threadID = mStartThreadID;

    for (auto it = mWorkers.begin(), end = mWorkers.end(); it != end; ++it, ++threadID)
    {
        try
        {
            mThreads.emplace_back(std::thread(&Worker::OnPooledRun, it,
                                              std::make_pair(&mNotification, &mMutex),
                                              std::make_pair(&mRespond, &mRespondMutex), threadID));
        }
        catch (const std::system_error &)
        {
        }
    }
}

bool ThreadPool::IsRunning() const noexcept
{
    for (auto it = mWorkers.begin(), end = mWorkers.end(); it != end; ++it)
    {
        if ((*it).IsRunning())
        {
            return true;
        }
    }

    return false;
}

void ThreadPool::Run(JobQueue *jobs)
{
    std::unique_lock<std::mutex> lock(mMutex);

    if ((mLastQueueSize = jobs->Size()) > 0)
    {
        mQueueHook = jobs;

        //Do a manual activation to make sure the isRunning is set on time!
        for (auto it = mWorkers.begin(), end = mWorkers.end(); it != end; ++it)
        {
            (*it).Activate();
        }

        mNotification.notify_all();
    }
}

void ThreadPool::JoinAll()
{
    std::unique_lock<std::mutex> lock(mRespondMutex);

    if (mLastQueueSize > 0)
    {
        while (IsRunning())
        {
            mRespond.wait(lock);
        }
    }
}
