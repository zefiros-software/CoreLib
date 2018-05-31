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
#ifndef __ENGINE_LOCAL_H__
#define __ENGINE_LOCAL_H__

#include "manager/systemManager.h"
#include "manager/poolManager.h"

/// @addtogroup Pools

/**
 * Retrieves and disposes a pool object in local scope using the RAII patttern.
 *
 * @tParam  The type we want to pool.
 *
 * @sa  NonCopyable
 */

template < typename tT, typename tBase = tT >
class Local
    : NonCopyable < Local< tT, tBase >>
{
public:

    /**
     * Creates an object using a pool from the given managerHolder with the
     * given namespace.
     *
     * @param [in,out]  managerHolder   If non-null, manager for holder.
     * @param   ns                      (optional) the namespace.
     */

    explicit Local(ManagerHolder *const managerHolder, Namespace ns = 0u)
    {
        mPool = managerHolder->pool->Get< tT >(ns);
        mPooledObject = mPool->Get();
    }

    /**
     * Creates an object using a pool from the given pool with the
     * given namespace.
     *
     * @param [in,out]  pool    If non-null, the pool.
     */

    explicit Local(AbstractObjectPool< tBase > *const pool)
    {
        mPool = pool;
        mPooledObject = mPool->Get();
    }

    /**
     * Creates an object using a pool from the given pool using
     * the given type from the correct namespace.
     *
     * @param   ns  (optional) the namespace.
     */

    explicit Local(Namespace ns = 0u)
    {
        mPool = SystemManager::Get()->GetManagers()->pool->Get< tT >(ns);
        mPooledObject = mPool->Get();
    }

    /**
     * If there is a pooled object, we dispose it to the correct pool;
     */

    ~Local()
    {
        if (mPooledObject)
        {
            Dispose();
        }
    }

    /// @name Returning
    /// @{

    /**
     * Disposes our stored object to the correct object pool. Also sets our currently
     * pooled object to nullptr.
     */

    void Dispose()
    {
        mPool->Dispose(mPooledObject);
        mPooledObject = nullptr;
    }

    // @}

    /// @name Retrieval
    /// @{

    /**
     * Gets the object this class holds.
     *
     * @return  The object.
     */

    tBase *Get() noexcept
    {
        return mPooledObject;
    }

    /**
     * Gets the object this class holds.
     *
     * @return  The object.
     */

    const tBase *Get() const noexcept
    {
        return mPooledObject;
    }

    /**
     * Gets the object this class holds.
     *
     * @return  The object.
     */

    operator tBase *() noexcept
    {
        return mPooledObject;
    }

    /**
     * Gets the object this class holds.
     *
     * @return  The object.
     */

    tBase *operator*() noexcept
    {
        return mPooledObject;
    }

    /**
     * Gets the object this class holds.
     *
     * @return  The object.
     */

    tBase *operator->() noexcept
    {
        return mPooledObject;
    }

    /**
     * Gets the object this class holds.
     *
     * @return  The object.
     */

    const tBase *operator->() const noexcept
    {
        return mPooledObject;
    }

    /// @}

    /// @name Recreation
    /// @{

    /**
     * Recreates the stored object.
     */

    void Recreate()
    {
        tBase *newObject = mPool->Get();

        Dispose();

        mPooledObject = newObject;
    }

    /// @}

private:

    /// Stores our pooled object
    tBase *mPooledObject;

    /// The object pool
    AbstractObjectPool< tBase > *mPool;

};


/// @}

#endif