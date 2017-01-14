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
#ifndef __ENGINE_INSTANTIATOR_H__
#define __ENGINE_INSTANTIATOR_H__

#include "memory/abstract/abstractTInstantiator.h"

#include "common/util.h"

/// @addtogroup Instantiators
/// @{

/**
 * Instantiator used to instantiate objects of a certain type derived from a Base class.
 *
 * @tparam tT       The type of the instantiated class.
 * @tparam tBase    The base class the instantiated type is derived from.
 *
 * @sa  AbstractTInstantiator<tBase>
 */

template < typename tT, typename tBase = tT >
class Instantiator
    : public AbstractTInstantiator< tBase >
{
public:

    Instantiator() noexcept
    {
        static_assert( Util::IsChildParent< tT, tBase >::value,
                       "Instantiator::Instantiator():\n\tThe child type should derive from the base type." );
    }

    /// @name Object creation
    /// @{

    virtual tBase *Create() override
    {
        return new tT;
    }

    /// @}

    virtual AbstractInstantiator *Copy() override
    {
        return new Instantiator< tT, tBase >();
    }

};

/// @}

#endif