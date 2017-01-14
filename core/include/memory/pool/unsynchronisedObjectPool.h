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
#ifndef __ENGINE_UNSYNCHRONISEDOBJECTPOOL_H__
#define __ENGINE_UNSYNCHRONISEDOBJECTPOOL_H__


#include "memory/instantiator/poolableInstantiator.h"
#include "memory/abstract/abstractObjectPool.h"

#include <assert.h>

/// @addtogroup Pools
/// @{

/**
 * An object pool intends to keep objects alive and reuse them later on
 * to prevent memory fragmentation and improve caching. The class allows
 * for custom object creation/deletion by defining the instantiator type. Also
 * one should always return all objects to the pool it borrowed from
 * to prevent memory leaks.
 *
 * The pool has a preset capacity, which is the amount of objects it can hold
 * without the objects being used. When we overflow this amount, the objects
 * will be deleted. When we have objects stored, we use those objects instead
 * of creating new objects.
 *
 * This is the unsynchronised version of @see ObjectPool.
 *
 * @tparam tT               The instantiated type.
 * @tparam tBase            The derived type.
 * @tparam tInstantiator    The type of instantiator we use to create our objects.
 *
 * @sa  AbstractObjectPool<tBase>
 */

template< typename tT, typename tBase = tT, typename tInstantiator = PoolableInstantiator< tT, tBase > >
class UnsynchronisedObjectPool
    : public AbstractObjectPool< tBase >
{
public:

    /**
     * Uses the given instantiator to create its objects.
     *
     * @param [in,out]  instantiator    If non-null, the instantiator.
     * @param   capacity                (optional) The maximum of kept alive objects.
     *
     * @note    Takes ownership of the instantiator.
     */

    explicit UnsynchronisedObjectPool( AbstractPoolableInstantiator< tBase > *instantiator, size_t capacity = 500 )
        : mCapacity( capacity ),
          mBorrowedObjectsCount( 0 ),
          mReturnedObjectsCount( 0 ),
          mInstantiator( instantiator )
    {
        static_assert( Util::IsChildParent< tT, tBase >::value,
                       "UnsynchronisedObjectPool::UnsynchronisedObjectPool():\n\tThe child type should derive from the base type." );
    }

    /**
     * Creates its own instantiator.
     *
     * @param   capacity    (optional) The maximum of kept alive objects.
     */

    explicit UnsynchronisedObjectPool( size_t capacity = 500 )
        : mCapacity( capacity ),
          mBorrowedObjectsCount( 0 ),
          mReturnedObjectsCount( 0 ),
          mInstantiator( new tInstantiator )
    {
        static_assert( Util::IsChildParent< tT, tBase >::value,
                       "ObjectPool::ObjectPool():\n\tThe child type should derive from the base type." );
    }

    /**
     * Destroys all the unused objects. Checks whether we returned all our objects and delete the instantiator.
     */

    virtual ~UnsynchronisedObjectPool()
    {
        for ( auto it = mPool.begin(); it != mPool.end(); ++it )
        {
            mInstantiator->Destroy( *it );
        }

        assert( GetBorrowedCount() == GetReturnedCount() );

        SAFE_DELETE( mInstantiator );
    }

    /// @name Retrieve Objects
    /// @{

    /**
     * Gets an object from the object pool.
     *
     * @return  The object.
     */

    virtual tBase *Get() override
    {
        tBase *const object = FastGet();

        mInstantiator->Initialise( object );

        return object;
    }

    /**
     * Gets an object from the object pool without initialising.
     *
     * @return  The object.
     */

    virtual tBase *FastGet() override
    {
        ++mBorrowedObjectsCount;

        return Create();
    }

    /// @}

    /// @name Return Objects
    /// @{

    /**
     * Returns the object to the pool and optionally releases the managed resources, when the object
     * pool wants to do so. Also calls Release() on the instantiator
     *
     * @param [in,out]  object  The object.
     */

    virtual void Dispose( tBase *object ) override
    {
        mInstantiator->Release( object );

        ++mReturnedObjectsCount;

        if ( mPool.size() < mCapacity )
        {
            mPool.push_back( object );
        }
        else
        {
            mInstantiator->Destroy( object );
        }
    }

    /**
     * Returns the object to the pool and optionally releases the managed resources, when the object
     * pool wants to do so.
     *
     * @param [in,out]  object  The object.
     */

    void FastDispose( tBase *object )
    {
        ++mReturnedObjectsCount;

        if ( mPool.size() < mCapacity )
        {
            mPool.push_back( object );
        }
        else
        {
            mInstantiator->Destroy( object );
        }
    }

    /// @}

    /// @name Verification
    /// @{

    /**
     * Gets the amount of borrowed objects from this pool.
     *
     * @return The borrowed count.
     */

    virtual size_t GetBorrowedCount() const noexcept override
    {
        return mBorrowedObjectsCount;
    }

    /**
     * Gets the amount of returned objects from this pool.
     *
     * @return  The returned count.
     */

    virtual size_t GetReturnedCount() const noexcept override
    {
        return mReturnedObjectsCount;
    }

    /// @}

private:

    /// The unused stored objects
    std::vector< tBase * > mPool;

    /// The maximum amount of objects in our pool
    size_t mCapacity;

    /// The amount of objects borrowed from the pool
    size_t mBorrowedObjectsCount;

    /// The amount of returned objects
    size_t mReturnedObjectsCount;

    /// The instantiator we use to create and delete objects.
    AbstractPoolableInstantiator< tBase > *mInstantiator;

    /**
     * Checks whether we can retrieve an object from our pool,
     * or whether we need to create a new one.
     *
     * @return The object.
     */

    tBase *Create()
    {
        tBase *object;

        if ( !mPool.empty() )
        {
            object = mPool.back();
            mPool.pop_back();
        }
        else
        {
            object = mInstantiator->Create();
        }

        return object;
    }
};

/// @}

#endif