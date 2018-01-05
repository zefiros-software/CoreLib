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
#ifndef __ENGINE_MEMORYMANAGER_H__
#define __ENGINE_MEMORYMANAGER_H__

#include "manager/abstract/abstractManager.h"

#include "memory/allocators/blockAllocator.h"

#include "threading/threadID.h"

#include "config.h"

class MemoryManager
    :  public AbstractManager
{
public:

    struct ThreadAllocators
    {
        ThreadAllocators();

        void Clear();

        BlockAllocator blockAlloc;
    };

    virtual void OnUpdate() override;

    template< class T >
    BlockAllocator::BlockLocation TempAlloc(ThreadID tid)
    {
        return mTempThreadAllocators[tid].blockAlloc.Alloc< T >();
    }

    template< class T >
    BlockAllocator::BlockLocation TempMove(ThreadID tid, const T &item)
    {
        return mTempThreadAllocators[tid].blockAlloc.Move< T >(item);
    }

    template< class T >
    T *ExtractTemp(ThreadID tid, BlockAllocator::BlockLocation loc)
    {
        return mTempThreadAllocators[tid].blockAlloc.Extract< T >(loc);
    }


private:

    ThreadAllocators mTempThreadAllocators[PROGRAM_MAX_THREADS];
};

#endif