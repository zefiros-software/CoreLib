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
#ifndef __NAMESPACESTORAGE_H__
#define __NAMESPACESTORAGE_H__

#include "common/namespace.h"

#include <unordered_map>
#include <unordered_set>
#include <mutex>

/// @addtogroup Datastructures
/// @{

/**
 * This class functions as a dictionary, which keeps its key-value pairs separated by a so
 * called Namespace. This namespace is in fact an integer. This mechanism allows us to remove
 * all key-value pairs based on their namespace, or even use the same key multiple times with
 * different namespaces. The different namespaces are thus strictly separated, but only for the
 * 16 least significant bytes.
 *
 * The namespace consists of two U16 integers, one of which is used by the plugin and one of
 * which is used by the addin. This idea allows us to seperate key- value pairs on the plugin
 * level, after which they are again seperated on the addin level. When we delete the keys on
 * the plugin level, all the keys on the addin level, which belong to that plugin, will also be
 * deleted. However, when we delete the key-value pairs on the addin level, only the key-value
 * pairs of that addin will be deleted.
 *
 * This concept may seem a little weird at first, however a system like this, can and should be
 * used for example for dynamic reloading of plugins. When we reload a plugin we also should
 * reload the addins. By deleting the plugins values, we also delete the addins values, which
 * makes management a little easier.
 *
 * Example:
 * @code
 *      NamedStorage< U32, U32 > storage;
 *      storage.Add( new int( 100 ), 42 );
 *
 *      // the keys are stored in a separate namespace.
 *      storage.Add( new int( 100 ), 42, 1u );
 *      storage.Add( new int( 100 ), 10, 1u );
 *      storage.Add( new int( 100 ), 10, Namespace( 1, 1) );
 *
 *      storage.HasName( 42 ); // will return true
 *      storage.HasName( 10 ); // will return false
 *      storage.HasName( 10, 1u ); // will return true
 *
 *      storage.RemoveObjectsByNamespace( 1u );
 *      storage.HasName( 10, 1u ); // will return false
 *      storage.HasName( 10, Namespace( 1, 1) ); // will return false
 * @endcode
 *
 * @note    The class only stores objects created with new, and after storing it, it will take
 *          ownership of the objects.
 *
 * @warning The addin namespace ID '0' will not be recognised as an addin, but as the whole
 *          plugin namespace. This is because of how the Namespace class is designed to work.
 *
 * @threadsafe.
 *
 * @tparam  Name        Type of the dictionary key.
 * @tparam  tStoredType  Type of the stored type.
 */

template< typename tName, typename tStoredType >
class NamespaceStorage
{
public:

    /**
     * Deconstructor, releases all our used memory.
     */

    ~NamespaceStorage() noexcept
    {
        // On deconstruction we should delete all the objects we have taken ownership over.
        Clear();
    }

    /// @name Modifiers
    /// @{

    /**
     * Adds a value object under the given key, separated by the given namespace. Ownership will be
     * taken over the added object. You cannot add a different object twice under the same name in
     * the same namespace, only the first addition will be taken care of whereas the later additions
     * will be ignored.
     *
     * @note    The value object should be created by the new operator, since the memory will be
     *          released using the operator delete.
     *
     * @threadsafe.
     *
     * @param [in,out]  object  The value object.
     * @param   name            The key to insert the object on.
     * @param   ns              (optional) The namespace.
     *
     * @return  true if it succeeds, false if it fails.
     */

    bool Add( tStoredType *object, const tName &name, const Namespace ns = 0u )
    {
        // Separate the namespace on addin level
        if ( ns.IsAddin() )
        {
            std::lock_guard< std::recursive_mutex > lock( mMutex );

            std::unordered_map< tName, tStoredType * > &nameMap = mAddinObjects[ ns ];

            const auto objectIt = nameMap.find( name );

            if ( objectIt == nameMap.end() )
            {
                nameMap[ name ] = object;
                mPluginAddins[ ns.GetPlugin() ].insert( ns.GetAddin() );

                return true;
            }
        }
        else
        {
            std::lock_guard< std::recursive_mutex > lock( mMutex );

            std::unordered_map< tName, tStoredType * > &nameMap = mPluginObjects[ ns.GetPlugin() ];

            const auto objectIt = nameMap.find( name );

            if ( objectIt == nameMap.end() )
            {
                nameMap[ name ] = object;

                return true;
            }
        }

        return false;
    }

    /**
     * Removes all the stored objects and frees their used memory.
     *
     * @threadsafe
     */

    void Clear()
    {
        std::lock_guard< std::recursive_mutex > lock( mMutex );

        for ( const auto &nameit : mPluginObjects )
        {
            const std::unordered_map< tName, tStoredType * > &nameMap = nameit.second;

            for ( const auto &it : nameMap )
            {
                delete it.second;
            }
        }

        for ( const auto &nameit : mAddinObjects )
        {
            const std::unordered_map< tName, tStoredType * > &nameMap = nameit.second;

            for ( const auto &it : nameMap )
            {
                delete it.second;
            }
        }

        mPluginObjects.clear();
        mAddinObjects.clear();
        mPluginAddins.clear();
    }

    /**
     * Removes and frees the memory of all the objects in the given namespace.
     *
     * @threadsafe.
     *
     * @param   ns  The namespace.
     */

    void Clear( const Namespace ns )
    {
        if ( ns.IsAddin() )
        {
            RemoveObjectsByAddinNamespace( ns );
        }
        else
        {
            RemoveObjectsByPluginNamespace( ns.GetPlugin() );
        }
    }

    /**
     * Removes the object by name, under the given namespace.
     *
     * @threadsafe.
     *
     * @param   name    The name.
     * @param   ns      (optional) The namespace.
     */

    void Remove( const tName &name, const Namespace ns = 0u )
    {
        std::unordered_map< tName, tStoredType * > *nameMap = nullptr;

        std::lock_guard< std::recursive_mutex > lock( mMutex );

        if ( ns.IsAddin() )
        {
            const auto addinIt = mAddinObjects.find( ns );

            if ( addinIt != mAddinObjects.end() )
            {
                nameMap = &addinIt->second;
            }
        }
        else
        {
            auto pluginIt = mPluginObjects.find( ns.GetPlugin() );

            if ( pluginIt != mPluginObjects.end() )
            {
                nameMap = &pluginIt->second;
            }
        }


        if ( nameMap )
        {
            const auto objectIt = nameMap->find( name );

            if ( objectIt != nameMap->end() )
            {
                delete objectIt->second;

                nameMap->erase( objectIt );
            }
        }
    }

    /// @}

    /// @name Element access
    /// @{

    /**
     * Gets the object, when it exists, under the given key and namespace.
     *
     * @threadsafe
     *
     * @param   name    The key under which the object is stored.
     * @param   ns      (optional) the namespace.
     *
     * @return  null if it fails, else returns the stored object.
     */

    tStoredType *Get( const tName &name, const Namespace ns = 0u ) const
    {
        const std::unordered_map< tName, tStoredType * > *nameMap = nullptr;

        std::lock_guard< std::recursive_mutex > lock( mMutex );

        if ( ns.IsAddin() )
        {
            const auto nameIt = mAddinObjects.find( ns );

            if ( nameIt != mAddinObjects.end() )
            {
                nameMap = &nameIt->second;
            }
        }
        else
        {
            const auto nameIt = mPluginObjects.find( ns.GetPlugin() );

            if ( nameIt != mPluginObjects.end() )
            {
                nameMap = &nameIt->second;
            }
        }

        if ( nameMap )
        {
            const auto objectIt = nameMap->find( name );

            if ( objectIt != nameMap->end() )
            {
                return objectIt->second;
            }
        }

        return nullptr;
    }

    /// @}

    /// @name Element verification
    /// @{

    /**
     * Query whether the given key has an object stored under the given namespace.
     *
     * @threadsafe
     *
     * @param   name    The key.
     * @param   ns      (optional) the namespace.
     *
     * @return  true if it has an object stored under this name, false if not.
     */

    bool Has( const tName &name, const Namespace ns = 0u ) const
    {
        const std::unordered_map< tName, tStoredType * > *nameMap = nullptr;

        std::lock_guard< std::recursive_mutex > lock( mMutex );

        if ( ns.IsAddin() )
        {
            const auto nameIt = mAddinObjects.find( ns );

            if ( nameIt != mAddinObjects.end() )
            {
                nameMap = &nameIt->second;
            }
        }
        else
        {
            const auto nameIt = mPluginObjects.find( ns.GetPlugin() );

            if ( nameIt != mPluginObjects.end() )
            {
                nameMap = &nameIt->second;
            }
        }

        if ( nameMap )
        {
            return nameMap->find( name ) != nameMap->end();
        }

        return false;
    }

    /**
     * Query if the given namespace is used. Will also return true when only an addin namespace
     * behind the given plugin namespace is used, even when the actual plugin namespace is empty.
     * However the oposite does not hold true, thus a plugin namespace should actually have objects
     * added for this to return true.
     *
     * @threadsafe.
     *
     * @param   ns  The namespace.
     *
     * @return  true if the namespace is used, false if not.
     */

    bool HasNamespace( const Namespace ns ) const
    {
        std::lock_guard< std::recursive_mutex > lock( mMutex );

        if ( ns.IsAddin() )
        {
            return mAddinObjects.find( ns ) != mAddinObjects.end();
        }

        return mPluginObjects.find( ns.GetPlugin() ) != mPluginObjects.end() ||
               mPluginAddins.find( ns.GetPlugin() ) != mPluginAddins.end();
    }

    /// @}

private:

    // Holds the objects stored under the full addin namespace.
    std::unordered_map< Namespace, std::unordered_map< tName, tStoredType * > > mAddinObjects;

    // Holds the objects stored directly under the plugin namespace.
    std::unordered_map< Namespace, std::unordered_map< tName, tStoredType * > > mPluginObjects;

    // Holds the addin namespaces per plugin namespace, so we know what plugins have what addins.
    std::unordered_map< Namespace, std::unordered_set< Namespace > > mPluginAddins;

    // Mutex for our threadsafety.
    mutable std::recursive_mutex mMutex;

    /// @addtogroup Memory Management
    /// @{

    /**
     * Removes and frees the memory of the stored under the given addin namespace.
     *
     * @threadsafe.
     *
     * @param   addinNs The addin namespace.
     */

    void RemoveObjectsByAddinNamespace( const Namespace addinNs )
    {
        std::lock_guard< std::recursive_mutex > lock( mMutex );

        const auto pluginIt = mAddinObjects.find( addinNs );

        if ( pluginIt != mAddinObjects.end() )
        {
            const std::unordered_map< tName, tStoredType * > &objects = pluginIt->second;

            for ( const auto &it : objects )
            {
                delete it.second;
            }

            mAddinObjects.erase( pluginIt );
        }

        const auto addinIt = mPluginAddins.find( addinNs.GetPlugin() );

        if ( addinIt != mPluginAddins.end() )
        {
            std::unordered_set< Namespace > &addins = addinIt->second;

            const auto &it = addins.find( addinNs.GetAddin() );

            if ( it != addins.end() )
            {
                addins.erase( it );
            }
        }
    }

    /**
     * Removes and frees the memory of the objects stored under the given plugin namespace.
     *
     * @threadsafe
     *
     * @param   pluginNs    The plugin namespace.
     */

    void RemoveObjectsByPluginNamespace( const Namespace pluginNs )
    {
        std::lock_guard< std::recursive_mutex > lock( mMutex );

        const auto pluginIt = mPluginObjects.find( pluginNs );

        if ( pluginIt != mPluginObjects.end() )
        {
            const std::unordered_map< tName, tStoredType * > &objects = pluginIt->second;

            for ( const auto &it : objects )
            {
                delete it.second;
            }

            mPluginObjects.erase( pluginIt );
        }

        auto addinIt = mPluginAddins.find( pluginNs );

        if ( addinIt != mPluginAddins.end() )
        {
            std::unordered_set< Namespace > addinIDs = addinIt->second;

            for ( auto addinNs : addinIDs )
            {
                RemoveObjectsByAddinNamespace( Namespace( pluginNs, addinNs ) );
            }

            mPluginAddins.erase( addinIt );
        }
    }

    /// @}
};

/// @}

#endif