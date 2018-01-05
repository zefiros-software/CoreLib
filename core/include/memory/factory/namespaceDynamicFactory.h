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
#ifndef __ENGINE_NAMESPACEDYNAMICFACTORY_H__
#define __ENGINE_NAMESPACEDYNAMICFACTORY_H__

#include "memory/instantiator/instantiator.h"

#include "container/namespaceNamedStorage.h"

/// @addtogroup Factories
/// @{

/**
 * A factory that works on the same principles as the DynamicFactory.
 * However the factories are now seperated by their respective namespaces.
 * Note that the given keys should be unique over all namespaces.
 *
 * @note This is a simple wrapper around the NamespaceNamedStorage class
 *       so we can use it as a factory storage.
 *
 * @partthreadsafe
 *
 * @tparam  tName    Type of the name.
 * @tparam  tBase    Type of the base.
 * @tparam  tInstantiator  Type of the base instantiators, used by the factories.
 *
 * @sa NamespaceNamedStorage
 */

template < typename tName, typename tBase, typename tInstantiator = AbstractTInstantiator< tBase > >
class NamespaceDynamicFactory
{
public:

    /// @name Object creation
    /// @{

    /**
     * Creates an instance, using the factory under the given name.
     *
     * @partthreadsafe{ the stored factory is threadsafe and not modified }
     *
     * @param   name    The name.
     *
     * @return  The new instance, NULL if no factory is found, or if the creation fails.
     */

    tBase *Create(const tName &name) const
    {
        tInstantiator *const factory = mFactories.Get(name);

        if (factory)
        {
            return factory->Create();
        }

        return nullptr;
    }

    /// @}

    /// @name Lookup
    /// @{

    /**
     * Gets a factory under the given name.
     *
     * @threadsafe.
     *
     * @param   name    The name.
     *
     * @return  null if it fails, else the factory.
     */

    tInstantiator *Get(const tName &name) const
    {
        return mFactories.Get(name);
    }

    /**
     * Gets the registered names under the given namespace.
     *
     * @threadsafe
     *
     * @param   ns  The namespace.
     *
     * @return  The registered names by namespace.
     */

    std::vector< tName > GetByNamespace(Namespace ns) const
    {
        return mFactories.GetNames(ns);
    }

    /// @}

    /// @name Modifiers
    /// @{

    /**
     * Registers what instantiator to use when we create an object using 'name'. Under the given
     * namespace.
     *
     * @threadsafe.
     *
     * @tparam  tT          Type of the instantiated object.
     * @tparam  tBase       Type of the base.
     *
     * @param   name    The name.
     * @param   ns      (optional) the namespace.
     *
     * @return  true if it succeeds, false if it fails.
     *
     * ### param [in,out]   factory If non-null, the factory.
     */

    template< typename tT >
    bool Register(const tName &name, Namespace ns = 0u)
    {
        return mFactories.Add(new Instantiator< tT, tBase >, name, ns);
    }

    /**
     * Registers what instantiator to use when we create an object using 'name'.
     * Under the given namespace.
     *
     * @threadsafe
     *
     * @param [in,out]  factory If non-null, the factory.
     * @param   name            The name.
     * @param   ns              (optional) the namespace.
     *
     * @return  true if it succeeds, false if it fails.
     */

    bool RegisterExt(tInstantiator *factory, const tName &name, Namespace ns = 0u)
    {
        return mFactories.Add(factory, name, ns);
    }

    /**
     * Unregisters the factories under the given namespace.
     *
     * @threadsafe
     *
     * @param   ns  The ns.
     */

    void Clear(Namespace ns)
    {
        mFactories.Clear(ns);
    }

    /**
     * Unregisters the classes.
     *
     * @threadsafe
     */

    void Clear()
    {
        mFactories.Clear();
    }

    /**
     * Unregisters the class under the given name.
     *
     * @threadsafe
     *
     * @param   name    The name.
     */

    void Remove(const tName &name)
    {
        mFactories.Remove(name);
    }

    /// @}

    /// @name Verification
    /// @{

    /**
     * Query if 'name' is registered.
     *
     * @threadsafe
     *
     * @param   name    The name.
     *
     * @return  true if registered, false if not.
     */

    bool Has(const tName &name) const
    {
        return mFactories.Has(name);
    }

    /// @}

protected:

    /// Holds the actual factories
    NamespaceNamedStorage< tName, tInstantiator > mFactories;
};

/// @}

#endif