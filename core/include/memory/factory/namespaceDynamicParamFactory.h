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
#ifndef __ENGINE_NAMESPACEDYNAMICPARAMFACTORY_H__
#define __ENGINE_NAMESPACEDYNAMICPARAMFACTORY_H__

#include "memory/factory/namespaceDynamicFactory.h"

#include "memory/instantiator/paramInstantiator.h"

/// @addtogroup Factories
/// @{

/**
 * Namespace dynamic parameter factory.
 *
 * @partthreadsafe
 *
 * @tparam  tName    Type of the name.
 * @tparam  tBase    Type of the base.
 * @tparam  tParam   Type of the creation parameter.
 * @tparam  tInstantiator  Type of the base instantiators, used by the factories.
 *
 * @sa  NamespaceDynamicFactory<tName, tBase, tInstantiator>
 */

template < typename tName, typename tBase, typename tParam, typename tInstantiator = AbstractParamInstantiator< tBase, tParam >  >
class NamespaceDynamicParamFactory
    : public NamespaceDynamicFactory< tName, tBase, tInstantiator >
{
public:

    /// @name Modifiers
    /// @{

    /**
     * @sa NamespaceDynamicFactory::Register
     */

    template< typename tT >
    bool Register( const tName &name, Namespace ns = 0u )
    {
        return this->mFactories.Add( new ParamInstantiator< tT, tParam, tBase >, name, ns );
    }

    /// @}

    /// @name Object creation
    /// @{


    /**
     * Creates an instance using the factory identified by name, and constructed
     * with the given parameter.
     *
     * @partthreadsafe{ the stored factory is threadsafe and not modified }
     *
     * @param   name        The name.
     * @param   parameter   The parameter.
     *
     * @return  The new instance.
     */

    tBase *CreateInstance( const tName &name, const tParam &parameter ) const
    {
        tInstantiator *const factory = this->mFactories.Get( name );

        if ( factory )
        {
            return factory->CreateInstance( parameter );
        }

        return nullptr;
    }

    /// @}
};

/// @}

#endif