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
#ifndef __ENGINE_ABSTRACTPARAMINSTANTIATOR_H__
#define __ENGINE_ABSTRACTPARAMINSTANTIATOR_H__

#include "memory/abstract/abstractTInstantiator.h"

/// @addtogroup Instantiators
/// @{

/**
 * Abstract parameter instantiator, defines an interface that every instantatior, that require
 * parameters to instantiate, should derive from.
 *
 * @sa  AbstractTInstantiator<Base>
 *
 * @tparam   Base   The base class the instantiator creates objects for.
 * @tparam   Param  The type of the parameter, that is required to instantiate.
 */

template< typename tBase, typename Param >
class AbstractParamInstantiator
    : public AbstractTInstantiator< tBase >
{
public:

    /// @name Creation
    /// @{

    /**
     * Creates an instance, using the given parameter.
     *
     * @param   param   The parameter.
     *
     * @return  The new instance.
     */

    virtual tBase *CreateInstance( const Param &param ) = 0;

    /// @}

private:

    virtual tBase *CreateInstance() override
    {
        // just do something, we wont use it anyway
        return nullptr;
    }
};

/// @}

#endif