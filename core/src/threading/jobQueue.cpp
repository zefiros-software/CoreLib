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

#include "threading/jobQueue.h"

#include <mutex>

JobQueue::JobQueue() noexcept
{

}

JobQueue::JobQueue( const JobQueue &&queue ) noexcept
    : mJobQueue( queue.mJobQueue ),
      mUpdateQueue( queue.mUpdateQueue )
{

}

JobQueue::~JobQueue() noexcept
{
    while ( !mUpdateQueue.empty() )
    {
        mUpdateQueue.back()->OnJobFinished();
        mUpdateQueue.pop();
    }
}

void JobQueue::Push( IThreadExecutable *const job )
{
    std::lock_guard< SpinLock > lock( mPushLock );

    mUpdateQueue.push( job );
}

void JobQueue::Flush() noexcept
{
    while ( !mJobQueue.empty() )
    {
        mJobQueue.pop();
    }

    while ( !mUpdateQueue.empty() )
    {
        mJobQueue.push( mUpdateQueue.front() );

        mUpdateQueue.pop();
    }
}

IThreadExecutable *JobQueue::Pop()
{
    IThreadExecutable *job = nullptr;

    mPopLock.lock();

    if ( !mJobQueue.empty() )
    {
        job = mJobQueue.front();
        mJobQueue.pop();
    }

    mPopLock.unlock();

    return job;
}

size_t JobQueue::Size() noexcept
{
    mPopLock.lock();
    size_t size = mJobQueue.size();
    mPopLock.unlock();

    return size;
}
