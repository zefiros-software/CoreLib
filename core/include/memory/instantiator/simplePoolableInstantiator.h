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

#pragma once
#ifndef __ENGINE_SIMPLEPOOLABLEINSTANTIATOR_H__
#define __ENGINE_SIMPLEPOOLABLEINSTANTIATOR_H__

#include "memory/abstract/abstractPoolableInstantiator.h"

#include "common/util.h"

/// @addtogroup Instantiators
/// @{

/**
 * The simple poolable instantiator defines the instantiator as a basic factory,
 * where no initialisation nor release event will be called on the object. This
 * can be handy when we want to pool primitive types for example.
 *
 * @tParam tT       The instantiated type.
 * @tParam tBase    The type of the base class the instantiated type
 *                  should derive from.
 *
 * @sa  AbstractPoolableInstantiator<tT>
 */

template< typename tT, typename tBase = tT >
class SimplePoolableInstantiator
    : public AbstractPoolableInstantiator< tBase >
{
public:

    SimplePoolableInstantiator() noexcept
    {
        static_assert( Util::IsChildParent< tT, tBase >::value,
                       "SimplePoolableInstantiator::SimplePoolableInstantiator():\n\tThe child type should derive from the base type." );
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

    virtual void Destroy( tBase *object ) override
    {
        delete object;
    }

    /// @}

    /// @name Initialisation
    /// @{

    virtual void Initialise( tBase *const ) override
    {
    }

    /// @}

    /// @name Releasing
    /// @{

    virtual void Release( tBase *const ) override
    {
    }

    /// @}

    virtual AbstractInstantiator *Copy() override
    {
        return new SimplePoolableInstantiator< tT, tBase >();
    }
};

/// @}

#endif