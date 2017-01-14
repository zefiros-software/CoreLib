/**
 * @cond ___LICENSE___
 *
 * Copyright (c) 2017 Koen Visscher, Paul Visscher and individual contributors.
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
#include "memory/allocators/blockAllocator.h"


BlockAllocator::BlockAllocator( size_t blockSize )
    : mBlocks( 1, StackAllocator( blockSize ) ),
      mBlockSize( blockSize ),
      mBlockPosition( 0 )
{

}

void BlockAllocator::BlockAdvance( size_t bytes )
{
    //determine if we can accomodate this item in the current block
    if ( !mBlocks[mBlockPosition].FitsInStack( bytes ) )
    {
        //advance the block
        mBlockPosition++;
        mBlocks.push_back( StackAllocator( mBlockSize ) );
    }
}

BlockAllocator::BlockLocation BlockAllocator::GenerateBlockLocation( StackAllocator::StackLocation stackLoc ) const
{
    //patch the location to deal with the block advance
    return stackLoc + ( mBlockPosition * mBlockSize );
}
