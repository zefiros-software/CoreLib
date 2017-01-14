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

#pragma once
#ifndef __ENGINE_BLOCKALLOCATOR_H__
#define __ENGINE_BLOCKALLOCATOR_H__

#include "memory/allocators/stackAllocator.h"

#include "common/utilClasses.h"

//Allocate the stack in blocks to ensure that we never have to resize ( invalidate ptr's ) when allocating
class BlockAllocator
    : public NonAssignable< BlockAllocator >
{
public:

    typedef std::ptrdiff_t BlockLocation;

    explicit BlockAllocator( size_t blockSize );

    template< class tT >
    BlockLocation Move( const tT &item )
    {
        BlockAdvance( sizeof( tT ) );

        StackAllocator::StackLocation stackLoc =  mBlocks[mBlockPosition].Move< tT >( item );

        return GenerateBlockLocation( stackLoc );
    }

    template< class tT >
    BlockLocation Alloc()
    {
        BlockAdvance( sizeof( tT ) );

        StackAllocator::StackLocation stackLoc =  mBlocks[mBlockPosition].Alloc< tT >();

        return GenerateBlockLocation( stackLoc );
    }

    template< class tT >
    const tT *Extract( BlockLocation loc ) const
    {
        // seperate in block  index and stack location
        StackAllocator::StackLocation stackLoc = loc % mBlockSize;
        size_t blockIndex = loc / mBlockSize;

        return mBlocks[blockIndex].Extract< tT >( stackLoc );
    }

    template< class tT >
    tT *Extract( BlockLocation loc )
    {
        // seperate in block  index and stack location
        StackAllocator::StackLocation stackLoc = loc % mBlockSize;
        size_t blockIndex = loc / mBlockSize;

        return mBlocks[blockIndex].Extract< tT >( stackLoc );
    }

    void Clear()
    {
        for ( auto it = mBlocks.begin(), itend = mBlocks.end(); it != itend; ++it )
        {
            it->Clear();
        }

        mBlockPosition = 0;
    }

private:

    std::vector< StackAllocator > mBlocks;

    const size_t mBlockSize;

    size_t mBlockPosition;

    void BlockAdvance( size_t bytes );

    BlockLocation GenerateBlockLocation( StackAllocator::StackLocation stackLoc ) const;
};

#endif