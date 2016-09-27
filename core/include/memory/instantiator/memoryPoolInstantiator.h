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
#ifndef __ENGINE_MEMORYPOOLINSTANTIATOR_H__
#define __ENGINE_MEMORYPOOLINSTANTIATOR_H__

#include "memory/abstract/abstractMemoryPoolInstantiator.h"

#include "threading/spinlock.h"

#include "common/util.h"

#include <assert.h>
#include <vector>
#include <mutex>

/// @addtogroup Instantiators
/// @{

/**
 * An instantiator that stores the objects in contiguous memory for better caching and also
 * functions as a memory pool.
 *
 * @threadsafe
 *
 * @sa  AbstractMemoryPooledInstantiator<tBase>
 * @sa  NonAssignable
 *
 * @tparam  tT      The instantiated object type.
 * @tparam  tBase   The base type the instantiated type should derive from.
 */

template< typename tT, typename tBase = tT >
class MemoryPoolInstantiator
    : public AbstractMemoryPoolInstantiator< tBase >,
      NonAssignable < MemoryPoolInstantiator< tT, tBase > >
{
public:

    /**
     * Constructs the instantiator and fills the first block.
     *
     * @threadsafe
     *
     * @param   blocksize   (optional) the amount of objects per block.
     * @param   maxBlocks   (optional) the maximum amount of blocks.
     */

    explicit MemoryPoolInstantiator( size_t blocksize = 1000, size_t maxBlocks = 1000 )
        : mBlockSize( blocksize ),
          mMaxBlocks( maxBlocks )
    {
        static_assert( Util::IsChildParent< tT, tBase >::value,
                       "MemoryPoolInstantiator::MemoryPoolInstantiator():\n\tThe child type should derive from the base type." );

        AddMemoryBlockArray();
    }

    /**
     * Frees all the memory used by the memory pool.
     *
     * @threadsafe
     *
     * @warning All objects should be returned to the pool
     *          to avoid memory leaks.
     */

    virtual ~MemoryPoolInstantiator()
    {
        assert( mBlockSize == 0 || mAvailablePtrs.size() / ( mBlockSize * mMemoryBlocks.size() ) == 1 );

        mAvailablePtrs.clear();

        for ( auto it = mMemoryBlocks.begin(), end = mMemoryBlocks.end(); it != end; ++it )
        {
            delete[] *it;
        }
    }

    /// @name Object creation
    /// @{

    /**
     * Creates the instance. When there are still objects available, retrieve them
     * from the pool. Otherwise we either add a new memory block of pointers or
     * we just create an object.
     *
     * @threadsafe
     *
     * @return  The new instance.
     */

    virtual tBase *Create() override
    {
        std::lock_guard< SpinLock > lock( mLock );

        if ( !mAvailablePtrs.empty() )
        {
            return GetObject();
        }
        else if ( mMemoryBlocks.size() <= mMaxBlocks )
        {
            AddMemoryBlockArray();
            return GetObject();
        }
        else
        {
            return new tT;
        }
    }

    /// @}

    /// @name Object deletion
    /// @{

    /**
     * Destroys the object described by object.
     *
     * @threadsafe
     *
     * @param [in,out]  object  If non-null, the object.
     */

    virtual void Destroy( tBase *object ) override
    {
        {
            std::lock_guard< SpinLock > lock( mLock );

            for ( auto it = mMemoryBlocks.begin(), end = mMemoryBlocks.end(); it != end; ++it )
            {
                // Check whether the pointer falls in this memory block
                if ( object >= *it && *it + mBlockSize > object )
                {
                    mAvailablePtrs.push_back( static_cast< tT * >( object ) );
                    return;
                }
            }
        }

        delete object;
    }

    /// @}

    virtual AbstractInstantiator *Copy() override
    {
        return new MemoryPoolInstantiator< tT, tBase >( mBlockSize, mMaxBlocks );
    }

    size_t GetBlockSize() const
    {
        return mBlockSize;
    }

    size_t GetMaxBlocks() const
    {
        return mMaxBlocks;
    }

private:

    MemoryPoolInstantiator( const MemoryPoolInstantiator & );

    /// @name Pool state
    /// @{

    /**
     * Adds a full block of memory to the available object pointers.
     */

    void AddMemoryBlockArray()
    {
        tT *memBlock = new tT[ mBlockSize ];
        mMemoryBlocks.push_back( memBlock );

        mAvailablePtrs.reserve( mAvailablePtrs.size() + mBlockSize );

        for ( size_t i = 0; i < mBlockSize; ++i )
        {
            mAvailablePtrs.push_back( memBlock++ );
        }
    }

    /// @}

    /// @name Object retrieval
    /// @{

    /**
     * Gets an object from the pointer vector.
     *
     * @return  The object.
     */

    tBase *GetObject()
    {
        tT *object = mAvailablePtrs.back();
        mAvailablePtrs.pop_back();
        return object;
    }

    /// @}

    /// Holds all the available pointers to object
    std::vector< tT * > mAvailablePtrs;

    /// Holds the used memory blocks
    std::vector< tT * > mMemoryBlocks;

    mutable SpinLock mLock;

    /// The amount of objects per block
    const size_t mBlockSize;

    /// The maximum of blocks used by this instantiator
    const size_t mMaxBlocks;
};

//// @}

#endif