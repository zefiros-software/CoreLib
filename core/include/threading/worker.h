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
#ifndef __ENGINE_WORKER_H__
#define __ENGINE_WORKER_H__

#include "threading/threadID.h"

#include <condition_variable>
#include <atomic>
#include <mutex>

class JobQueue;

class Worker
{
public:

    struct IsNotReady
    {
        IsNotReady(std::atomic< bool > *isrunning, std::atomic< bool > *terminate) noexcept;

        bool operator()() const;

    private:

        std::atomic< bool > *mIsRunning;
        std::atomic< bool > *mTerminate;
    };

    explicit Worker(JobQueue **hook) noexcept;
    Worker(const Worker &) noexcept;

    Worker operator= (const Worker &) const noexcept;

    JobQueue **GetQueueHook() const;

    void OnPooledRun(std::pair< std::condition_variable *, std::mutex * > notification,
                     std::pair< std::condition_variable *, std::mutex * > response, ThreadID threadID);

    void Activate();

    void RunJobs(ThreadID threadID) const;

    bool IsRunning() const;

    void Terminate();

private:

    JobQueue **mQueueHook;

    std::atomic< bool > mIsRunning;
    std::atomic< bool > mTerminate;
};

#endif