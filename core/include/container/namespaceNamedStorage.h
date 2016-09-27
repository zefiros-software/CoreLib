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
#ifndef __ENGINE_NAMESPACENAMEDSTORAGE_H__
#define __ENGINE_NAMESPACENAMEDSTORAGE_H__

#include "common/namespace.h"

#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <mutex>

/// @addtogroup Datastructures
/// @{

/**
 * This class is closely related to the NamespaceStorage class, however there are some very
 * important differences. First of all we enforce the keys (Name) to be unique over all
 * namespaces, in other words, we can only have one of the same key. Second, we store more
 * metadata of the objects, so we can ask the names used per namespace. Again the objects are
 * separated by their namespace, and we can delete them based on their namespace.
 *
 * Also this class will take ownership over the inserted objects, thus requiring added objects
 * to be created with the new operator.
 *
 * @warning The addin namespace ID '0' will not be recognised as an addin, but as the whole
 *          plugin namespace. This is because of how the Namespace class is designed to work.
 *
 * @see NamespaceStorage
 *
 * @threadsafe.
 *
 * @tparam  Name        Type of the dictionary key.
 * @tparam  StoredType  Type of the stored type.
 */

template < typename tName, typename tStoredType >
class NamespaceNamedStorage
{
public:

    /**
     * Destructor, releases all our used memory.
     */

    ~NamespaceNamedStorage() noexcept
    {
        Clear();
    }

    /// @name Modifiers
    /// @{

    /**
     * Adds a value object under the given unique key, separated by the given namespace. Ownership
     * will be taken over the added object. You cannot add a different object twice under the same
     * name, only the first addition will be taken care of whereas the later additions will be
     * ignored.
     *
     * @threadsafe.
     *
     * @param [in,out]  object  The value object.
     * @param   name            The name.
     * @param   ns              (optional) the ns.
     *
     * @return  true if it succeeds, false if it fails.
     */

    bool Add( tStoredType *object, const tName &name, Namespace ns = 0u )
    {
        std::lock_guard< std::recursive_mutex > lock( mMutex );

        auto objectIt = mObjects.find( name );

        if ( objectIt == mObjects.end() )
        {

            if ( ns.IsAddin() )
            {
                const Namespace addinNs = ns.GetAddin();
                std::unordered_set< tName > &nameSet = mAddinObjects[ ns ];
                mPluginAddinsNamespaces[ ns.GetPlugin() ].insert( addinNs );

                nameSet.insert( name );

                mObjects[ name ] = object;
                mObjectName[ name ] = ns;
            }
            else
            {
                const Namespace pluginNs = ns.GetPlugin();

                std::unordered_set< tName > &nameSet = mPluginObjects[ pluginNs ];

                nameSet.insert( name );

                mObjects[ name ] = object;
                mObjectName[ name ] = ns;
            }

            return true;
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

        for ( const auto &it : mObjects )
        {
            delete it.second;
        }

        mObjects.clear();
        mAddinObjects.clear();
        mPluginObjects.clear();
        mPluginAddinsNamespaces.clear();
        mObjectName.clear();
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
     * Removes the object by name. We don't have to know the actual namespace
     * off the stored object, since the name is unique.
     *
     * @threadsafe
     *
     * @param   name    The name.
     */

    void Remove( const tName &name )
    {
        std::lock_guard< std::recursive_mutex > lock( mMutex );

        auto nameIt = mObjectName.find( name );

        if ( nameIt != mObjectName.end() )
        {
            const Namespace ns( nameIt->second );

            std::unordered_set< tName > *nameSet = nullptr;

            if ( ns.IsAddin() )
            {
                auto addinIt = mAddinObjects.find( ns );

                if ( addinIt != mAddinObjects.end() )
                {
                    nameSet = &addinIt->second;
                }
            }
            else
            {
                auto pluginIt = mPluginObjects.find( ns.GetPlugin() );

                if ( pluginIt != mPluginObjects.end() )
                {
                    nameSet = &pluginIt->second;
                }
            }

            if ( nameSet )
            {
                auto objectIt = nameSet->find( name );

                if ( objectIt != nameSet->end() )
                {
                    nameSet->erase( objectIt );
                }
            }
        }

        RemoveObjectName( name );
    }

    /// @}

    /// @name Element access
    /// @{

    /**
     * Gets the object, when it exists, under the given key.
     *
     * @threadsafe.
     *
     * @param   name    The key under which the object is stored.
     *
     * @return  nullptr if it fails, else returns the stored object.
     */

    tStoredType *Get( const tName &name ) const
    {
        std::lock_guard< std::recursive_mutex > lock( mMutex );

        auto objectIt = mObjects.find( name );

        if ( objectIt != mObjects.end() )
        {
            return objectIt->second;
        }

        return nullptr;
    }

    /// @}

    /// @name Metadata
    /// @{

    /**
     * Gets the names used by the given namespace.
     *
     * @threadsafe
     *
     * @param   ns  The namespace.
     *
     * @return  The names used by this namespace.
     */

    std::vector< tName > GetNames( const Namespace ns ) const
    {
        std::vector< tName > objects;

        if ( ns.IsAddin() )
        {
            objects = GetNamesByAddinNamespace( ns );
        }
        else
        {
            objects = GetNamesByPluginNamespace( ns.GetPlugin() );
        }

        return objects;
    }

    /// @}

    /// @name Element verification
    /// @{

    /**
     * Query whether the given key has an object stored.
     *
     * @threadsafe
     *
     * @param   name    The name.
     *
     * @return  true if it has an object stored under this name, false if not.
     */

    bool Has( const tName &name ) const
    {
        std::lock_guard< std::recursive_mutex > lock( mMutex );

        return mObjects.find( name ) != mObjects.end();
    }

    /// @}

private:

    /// Holds the objects stored under the full addin namespace.
    std::unordered_map< Namespace, std::unordered_set< tName > > mAddinObjects;

    /// Holds the objects stored directly under the plugin namespace.
    std::unordered_map< Namespace, std::unordered_set< tName > > mPluginObjects;

    /// Holds the addin namespaces per plugin namespace, so we know what plugins have what addins.
    std::unordered_map< Namespace, std::unordered_set< Namespace > > mPluginAddinsNamespaces;

    /// Holds the objects stored under their names
    std::unordered_map< tName, tStoredType * > mObjects;

    /// Holds the namespace of the names
    std::unordered_map< tName, Namespace > mObjectName;

    mutable std::recursive_mutex mMutex;

    /**
     * Removes the object described by the name.
     *
     * @param   name    The name.
     */

    void RemoveObjectName( const tName &name )
    {
        auto objectIt = mObjects.find( name );

        if ( objectIt != mObjects.end() )
        {
            delete objectIt->second;

            mObjects.erase( objectIt );
        }

        auto nameIit = mObjectName.find( name );

        if ( nameIit != mObjectName.end() )
        {
            mObjectName.erase( nameIit );
        }
    }

    /**
     * Removes the objects under the given addin namespace.
     *
     * @threadsafe
     *
     * @param   ns  The addin namespace.
     */

    void RemoveObjectsByAddinNamespace( Namespace addinNs )
    {
        std::lock_guard< std::recursive_mutex > lock( mMutex );

        auto nameIt = mAddinObjects.find( addinNs );

        if ( nameIt != mAddinObjects.end() )
        {
            const std::unordered_set< tName > &identifiers = nameIt->second;

            for ( const auto id : identifiers )
            {
                RemoveObjectName( id );
            }

            mAddinObjects.erase( nameIt );
        }

        auto addinIt = mPluginAddinsNamespaces.find( addinNs.GetPlugin() );

        if ( addinIt != mPluginAddinsNamespaces.end() )
        {
            std::unordered_set< Namespace > &addins = addinIt->second;

            const auto it = addins.find( addinNs.GetAddin() );

            if ( it != addins.end() )
            {
                addins.erase( it );
            }
        }
    }

    /**
     * Removes the objects under the given plugin namespace.
     *
     * @threadsafe
     *
     * @param   pluginNs    The plugin namespace.
     */

    void RemoveObjectsByPluginNamespace( Namespace pluginNs )
    {
        std::lock_guard< std::recursive_mutex > lock( mMutex );

        auto nameIt = mPluginObjects.find( pluginNs );

        if ( nameIt != mPluginObjects.end() )
        {
            const std::unordered_set< tName > &identifiers = nameIt->second;

            for ( const auto id : identifiers )
            {
                RemoveObjectName( id );
            }

            mPluginObjects.erase( nameIt );
        }

        auto addinIt = mPluginAddinsNamespaces.find( pluginNs );

        if ( addinIt != mPluginAddinsNamespaces.end() )
        {
            const std::unordered_set< Namespace > addinIDs = addinIt->second;

            for ( auto addinNs : addinIDs )
            {
                RemoveObjectsByAddinNamespace( Namespace( pluginNs, addinNs ) );
            }

            mPluginAddinsNamespaces.erase( addinIt );
        }
    }

    /**
     * Gets the names used by the given addin namespace.
     *
     * @threadsafe
     *
     * @param   addinNs The addin nsnamespace
     *
     * @return  The names used by the addin namespace.
     */

    std::vector< tName > GetNamesByAddinNamespace( Namespace addinNs ) const
    {
        std::vector< tName > objects;

        std::lock_guard< std::recursive_mutex > lock( mMutex );

        auto pluginIt = mAddinObjects.find( addinNs );

        if ( pluginIt != mAddinObjects.end() )
        {
            const std::unordered_set< tName > &identifiers = pluginIt->second;

            for ( const auto id : identifiers )
            {
                objects.push_back( id );
            }
        }

        return objects;
    }

    /**
     * Gets the names used by the plugin namespace.
     *
     * @threadsafe.
     *
     * @param   pluginNs    The plugin namespace.
     *
     * @return  The names used by the plugin namespace.
     */

    std::vector< tName > GetNamesByPluginNamespace( Namespace pluginNs ) const
    {
        std::vector< tName > objects;

        std::lock_guard< std::recursive_mutex > lock( mMutex );

        auto nameIt = mPluginObjects.find( pluginNs );

        if ( nameIt != mPluginObjects.end() )
        {
            const std::unordered_set< tName > &identifiers = nameIt->second;

            for ( const auto id : identifiers )
            {
                objects.push_back( id );
            }
        }

        auto addinIt = mPluginAddinsNamespaces.find( pluginNs );

        if ( addinIt != mPluginAddinsNamespaces.end() )
        {
            const std::unordered_set< Namespace > &addinIDs = addinIt->second;

            for ( const auto id : addinIDs )
            {
                const std::vector< tName > newObjects = GetNamesByAddinNamespace( Namespace( pluginNs, id ) );
                objects.insert( objects.end(), newObjects.begin(), newObjects.end() );
            }
        }

        return objects;
    }
};

/// @}

#endif