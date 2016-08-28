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
#ifndef __ENGINE_PARAMINSTANTIATOR_H__
#define __ENGINE_PARAMINSTANTIATOR_H__

#include "memory/abstract/abstractParamInstantiator.h"

#include "common/util.h"

/// @addtogroup Instantiators
/// @{

/**
 * An instantiator that uses a parameter to create the objects.
 *
 * @tparam tT       The instantiated type.
 * @tparam tParam   The type of the creation parameter.
 * @tparam tBase    The type the instantiated type should derive from.
 *
 * @sa  AbstractParamInstantiator<tBase, tParam>
 */

template< typename tT, typename tParam, typename tBase = tT >
class ParamInstantiator
    : public AbstractParamInstantiator< tBase, tParam >
{
public:

    ParamInstantiator()
    {
        static_assert( Util::IsChildParent< tT, tBase >::value,
                       "ParamInstantiator::ParamInstantiator():\n\tThe child type should derive from the base type." );
    }

    /// @name Object creation
    /// @{

    virtual tBase *CreateInstance( const tParam &param ) override
    {
        return new tT( param );
    }

    /// @}

    virtual AbstractInstantiator *Copy() override
    {
        return new ParamInstantiator< tT, tParam, tBase >();
    }
};

/// @}

#endif