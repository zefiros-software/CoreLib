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

#include "manager/scheduleManager.h"
#include "manager/systemManager.h"


bool Worker::IsNotReady::operator()() const
{
    //override when we terminate
    if (mTerminate->load())
    {
        return true;
    }

    return mIsRunning->load();
}

Worker::IsNotReady::IsNotReady(std::atomic< bool > *isrunning, std::atomic< bool > *terminate) noexcept
    : mIsRunning(isrunning),
      mTerminate(terminate)
{

}

Worker::Worker(const Worker &init) noexcept
{
    mQueueHook = init.GetQueueHook();

    mIsRunning.store(false);
    mTerminate.store(false);
}

Worker::Worker(JobQueue **hook) noexcept
    : mQueueHook(hook),
      mIsRunning(false),
      mTerminate(false)
{
}

Worker Worker::operator= (const Worker &init) const noexcept
{
    return Worker(init.GetQueueHook());
}

JobQueue **Worker::GetQueueHook()  const
{
    return mQueueHook;
}

void Worker::OnPooledRun(std::pair< std::condition_variable *, std::mutex * > notification,
                         std::pair< std::condition_variable *, std::mutex * > response, ThreadID threadID)
{
    SystemManager::Get()->GetManagers()->schedule->SetCurrentThreadID(threadID);


    while (!mTerminate.load())
    {
        std::unique_lock<std::mutex> lock(*notification.second);
        notification.first->wait(lock, IsNotReady(&mIsRunning, &mTerminate));
        lock.unlock();

        if (mQueueHook)
        {
            if (*mQueueHook)
            {
                RunJobs(threadID);
            }
        }

        std::unique_lock< std::mutex > isRunningLock(*response.second);
        mIsRunning.store(false);
        isRunningLock.unlock();

        //notify the main thread to continue;
        response.first->notify_one();
    }
}

void Worker::Activate()
{
    mIsRunning.store(true);
}

bool Worker::IsRunning() const
{
    return mIsRunning.load();
}

void Worker::Terminate()
{
    mTerminate.store(true);
}

void Worker::RunJobs(const ThreadID threadID) const
{
    IThreadExecutable *job;

    while ((job = (*mQueueHook)->Pop()) != nullptr)
    {
        // Windows dll unshared global memory issues are resolved by this.
        // Fuck windows and their lazy ass dll implementation. Finding this issue
        // cost me way too much time, and the bloke who removes this line will be sent
        // straight to hell and will be forced to listen "Friday" 24/7 whilst being gutted by
        // Mickey Mouse. Seriously, do not remove this line.
        SystemManager::Get()->GetManagers()->schedule->SetCurrentThreadID(threadID);

        job->OnStartJob(threadID);
        job->OnRunJob();
        job->OnJobFinished();
    }
}
