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
#ifndef __ENGINE_MTQUEUE_H__
#define __ENGINE_MTQUEUE_H__

#include <condition_variable>
#include <queue>
#include <mutex>

template < class Item >
class MtQueue
{
public:

    void Push(const Item &item)
    {
        std::unique_lock< std::mutex > mlocalLock(mLock);
        mInternalQueue.push(item);
        mlocalLock.unlock();

        //signal that there is new data :D
        mNofity.notify_one();
    }

    Item WaitAndPop()
    {
        std::unique_lock< std::mutex > mlocalLock(mLock);

        //wait while its empty
        while (mInternalQueue.empty())
        {
            mNofity.wait(mlocalLock);
        }

        //get the item
        Item item = mInternalQueue.front();
        mInternalQueue.pop();

        return item;
    }

    bool TryPop(Item &item)
    {
        std::unique_lock< std::mutex > mlocalLock(mLock);

        if (mInternalQueue.empty())
        {
            return false;
        }

        item = mInternalQueue.front();
        mInternalQueue.pop();

        return true;
    }

    size_t Size() const noexcept
    {
        std::unique_lock< std::mutex > mlocalLock(mLock);

        return mInternalQueue.size();
    }

    bool Empty() const noexcept
    {
        std::unique_lock< std::mutex > mlocalLock(mLock);

        return mInternalQueue.empty();
    }

private:

    std::condition_variable mNofity;
    std::queue< Item > mInternalQueue;

    mutable std::mutex mLock;
};

#endif