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
#ifndef __ENGINE_THREADPTR_H__
#define __ENGINE_THREADPTR_H__

#include "threading/abstract/IThreadPtr.h"
#include "threading/threadID.h"

#include "manager/threadingVariableManager.h"
#include "manager/scheduleManager.h"
#include "manager/systemManager.h"

template< typename tT >
class ThreadPtr
    : public IThreadPtr,
      NonAssignable< ThreadPtr< tT > >
{
public:

    ThreadPtr() noexcept
        : mPtr( nullptr ),
          mNewPtr( nullptr ),
          mThreadID( Thread::InvalidID )
    {
        mScheduled.clear( boost::memory_order_relaxed );
    }

    explicit ThreadPtr( tT *ptr ) noexcept
        : mPtr( ptr ),
          mNewPtr( nullptr ),
          mThreadID( Thread::InvalidID )
    {
        mScheduled.clear( boost::memory_order_relaxed );
    }

    virtual void OnSynchronise() override
    {
        mScheduled.clear( boost::memory_order_release );

        if ( mPtr )
        {
            Dispose( mPtr );
        }

        mPtr = mNewPtr;
        mThreadID = Thread::InvalidID;
    }

    tT *Get() const
    {
        if ( mThreadID == SystemManager::Get()->GetManagers()->schedule->GetCurrentThreadID() )
        {
            return mNewPtr;
        }

        return mPtr;
    }

    operator tT *() const
    {
        return Get();
    }

    tT &operator*() const
    {
        return *Get();
    }

    tT *operator->() const
    {
        return Get();
    }

    tT *UnsynchronisedGet() const
    {
        return mPtr;
    }

    bool Set( tT *const ptr )
    {
        if ( !mScheduled.test_and_set( boost::memory_order_acquire ) )
        {
            ForcedSynchronisedSet( ptr );

            return true;
        }

        return false;
    }

    void ForcedSynchronisedSet( tT *const ptr )
    {
        ManagerHolder *managerHolder = SystemManager::Get()->GetManagers();

        mNewPtr = ptr;
        mThreadID = managerHolder->schedule->GetCurrentThreadID();
        managerHolder->threadingVariable->ScheduleThreadPtr( this );
    }

    void ForcedUnsynchronisedSet( tT *const ptr )
    {
        mPtr = ptr;
        mNewPtr = ptr;
        OnSynchronise();
    }

protected:

    tT *mPtr;
    tT *mNewPtr;

    boost::atomic_flag mScheduled;

    U8 mThreadID;

    virtual void Dispose( tT * )
    {
    }
};


#endif