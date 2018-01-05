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
#ifndef __ENGINE_POOLABLEINSTANTIATOR_H__
#define __ENGINE_POOLABLEINSTANTIATOR_H__

#include "memory/abstract/abstractPoolableInstantiator.h"

#include "common/util.h"

/// @addtogroup Instantiators
/// @{

/**
 * A pool instantiator type, that should be used by pools since it defines
 * how objects should be created, initialised, deleted and released.
 *
 * @note    The instantiated type should have a OnInit() event for
 *          initialisation of the object, and a OnRelease() event
 *          for the releasing of the object.
 *
 * @tParam tT       The instantiated type.
 * @tParam tBase    The type of the base class the instantiated type
 *                  should derive from.
 *
 * @sa  AbstractTInstantiator<tT>
 */

template< typename tT, typename tBase = tT >
class PoolableInstantiator
    : public AbstractPoolableInstantiator< tBase >
{
public:

    PoolableInstantiator() noexcept
    {
        static_assert(Util::IsChildParent< tT, tBase >::value,
                      "PoolableInstantiator::PoolableInstantiator():\n\tThe child type should derive from the base type.");
    }

    /// @name Creation
    /// @{

    /**
     * Creates the instance.
     *
     * @return  The new instance.
     */

    virtual tBase *Create() override
    {
        return new tT;
    }

    /// @}

    /// @name Deletion
    /// @{

    /**
     * Destroys the object, by releasing the memory.
     *
     * @param [in,out]  object  The object.
     */

    virtual void Destroy(tBase *object) override
    {
        delete object;
    }

    /// @}

    /// @name Initialisation
    /// @{

    /**
     * Initialises the object, the object should have an
     * OnInit() event to handle initialisation.
     *
     * @param [in,out]  object  The object.
     */

    virtual void Initialise(tBase *const object) override
    {
        object->OnInit();
    }

    /// @}

    /// @name Releasing
    /// @{

    /**
     * Releases the object by calling the OnRelease()
     * event on the object.
     *
     * @param [in,out]  object  The object.
     */

    virtual void Release(tBase *const object) override
    {
        object->OnRelease();
    }

    /// @}

    virtual AbstractInstantiator *Copy() override
    {
        return new PoolableInstantiator< tT, tBase >();
    }

};

/// @}

#endif