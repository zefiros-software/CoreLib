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
#ifndef __ENGINE_DYNAMICFACTORY_H__
#define __ENGINE_DYNAMICFACTORY_H__

#include "memory/instantiator/instantiator.h"

#include <unordered_map>

/// @addtogroup Factories
/// @{

/**
 * A dynamic factory allows to create objects derived from a given base class, to
 * be specifically created by a on forehand registered instantiator with a given
 * class name.
 *
 * @tparam  Name    Type of the name, with which the instantiators will be identified.
 * @tparam  Base    Type of the base, from which the instantiated types should derive.
 */

template < typename tName, typename tBase >
class DynamicFactory
{
public:

    /**
     * Destructor, frees all memory used by destroying the instantiators.
     */

    virtual ~DynamicFactory()
    {
        for ( auto it = mInstantiators.begin(); it != mInstantiators.end(); ++it )
        {
            delete it->second;
        }
    }

    /// @name Object creation
    /// @{

    /**
     * Creates an instance, by using the instantiator stored on the given name.
     *
     * @param   name    Identifier for the instantiator.
     *
     * @return  The new instance, NULL if there is no instantiator stored under the given name.
     */

    tBase *CreateInstance( const tName &name ) const
    {
        const auto it = mInstantiators.find( name );

        if ( it != mInstantiators.end() )
        {
            return it->second->CreateInstance();
        }

        return nullptr;
    }

    /// @}

    /// @name Modifiers
    /// @{

    /**
     * Registers an instantiator of the given type under the given name.
     * When we create an instance with this name, this instantiator will be used.
     * You cannot add a different instantiator twice under the same name, only the
     * first instantiator will be considered as valid, while the later will be
     * ignored.
     *
     * @tparam  Instantiated   Type of the instantiated objects, should
     *                         derive from Base.
     *
     * @param   name Identifier for the instantiator.
     *
     * @return  true if it succeeds, false if it fails.
     */

    template < typename Instantiated >
    bool Register( const tName &name )
    {
        Instantiator< Instantiated, tBase > *instantiator = new Instantiator< Instantiated, tBase >;

        if ( !RegisterExt( instantiator, name ) )
        {
            delete instantiator;
            return false;
        }

        return true;
    }

    /**
     * Registers an given instantiator under the given name, this will take ownership of the
     * instantiator, thus the DynamicFactory will handle its memory management. You cannot add a
     * different instantiator twice under the same name, only the first instantiator will be
     * considered as valid, while the later will be ignored.
     *
     * @param [in,out]  factory The factory.
     * @param   name            Identifier for the instantiator.
     *
     * @return  true if it succeeds, false if it fails.
     */

    bool RegisterExt( AbstractTInstantiator< tBase > *factory, const tName &name )
    {
        const auto it = mInstantiators.find( name );

        if ( it == mInstantiators.end() )
        {
            mInstantiators[ name ] = factory;
            return true;
        }

        return false;
    }

    /**
     * Unregisters and free the memory of the instantiator stored under the given name.
     * This function will do nothing if there is no instantiator stored under the name.
     *
     * @param   name    Identifier for the instantiator.
     */

    void Unregister( const tName &name )
    {
        const auto it = mInstantiators.find( name );

        if ( it != mInstantiators.end() )
        {
            delete it->second;

            mInstantiators.erase( it );
        }
    }

    /// @}

    /// @name Verification
    /// @{

    /**
     * Query if 'name' is registered.
     *
     * @param   name    Identifier for the class.
     *
     * @return  true if registered, false if not.
     */

    bool IsRegistered( const tName &name ) const
    {
        return mInstantiators.find( name ) != mInstantiators.end();
    }

    /// @}

private:

    /// Stores the instantiators under their given name.
    std::unordered_map< tName, AbstractTInstantiator< tBase > * > mInstantiators;
};

/// @}

#endif