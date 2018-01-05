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
#ifndef __ENGINE_STACKALLOCATOR_H__
#define __ENGINE_STACKALLOCATOR_H__

#include "common/types.h"

#include <cstring>
#include <vector>

class StackAllocator
{
public:

    typedef ptrdiff_t StackLocation;

    StackAllocator();

    StackAllocator(size_t size);

    template< class tItem >
    StackLocation Move(const tItem &item)
    {
        StackLocation loc = Alloc< tItem >();

        U8 *ptr = Extract<U8>(loc);

        memcpy(ptr, &item, sizeof(tItem));

        return loc;
    }

    template< class tItem >
    StackLocation Alloc()
    {
        size_t size = sizeof(tItem);

        return InternalAlloc(size);
    }

    template< class tItem >
    const tItem *Extract(StackLocation location) const
    {
        return reinterpret_cast<const tItem *>(mStack.data() + location);
    }

    template< class tItem >
    tItem *Extract(StackLocation location)
    {
        return reinterpret_cast<tItem *>(mStack.data() + location);
    }

    bool FitsInStack(size_t bytes) const;

    void Clear();

private:

    StackLocation InternalAlloc(size_t allocSize);

    std::vector< U8 > mStack;

    size_t mCursor;
};

#endif