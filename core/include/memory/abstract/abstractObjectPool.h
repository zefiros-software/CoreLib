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
#ifndef __ENGINE_ABSTRACTOBJECTPOOL_H__
#define __ENGINE_ABSTRACTOBJECTPOOL_H__

#include "memory/abstract/abstractPool.h"

#include <stddef.h>

/// @addtogroup
/// @{

/**
 * Abstract object pool, an interface from which every ObjectPool
 * should derive.
 *
 * @sparam Base The base class the object pool will create objects for.
 * @sa  AbstractPool
 */

template< typename tBase >
class AbstractObjectPool
    : public AbstractPool
{
public:

    /// @name Retrieve Objects
    /// @{

    /**
     * Gets an object from the object pool.
     *
     * @return  The object.
     */

    virtual tBase *Get() = 0;

    /**
     * Gets an object from the object pool without initialising.
     *
     * @return  The object.
     */

    virtual tBase *FastGet()
    {
        return Get();
    }

    /// @}

    /// @name Return Objects
    /// @{

    /**
     * Returns the object to the pool and optionally releases the managed resources, when the object
     * pool wants to do so.
     *
     * @param [in,out]  object  The object.
     */

    virtual void Dispose( tBase *object ) = 0;

    /// @}

    /// @name Verification
    /// @{

    /**
     * Gets the amount of borrowed objects from this pool.
     *
     * @return  The borrowed count.
    */

    virtual size_t GetBorrowedCount() const noexcept = 0;

    /**
     * Gets the amount of returned objects from this pool.
     *
     * @return  The returned count.
     */

    virtual size_t GetReturnedCount() const noexcept = 0;

    /// @}
};

/// @}

#endif