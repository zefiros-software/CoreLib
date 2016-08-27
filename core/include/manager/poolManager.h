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
#ifndef __ENGINE_POOLMANAGER_H__
#define __ENGINE_POOLMANAGER_H__

#include "manager/factoryManager.h"

#include "container/namespaceStorage.h"

#include "memory/pool/objectPool.h"

class AbstractPool;

class PoolManager
    : public AbstractManager
{
public:

    virtual void OnRelease() override;

    virtual void OnRelease( Namespace ns ) override;

    template< typename tT >
    void ReleasePool( const Namespace ns = 0u )
    {
        mPools.RemoveObjectByName( typeid( tT ), ns );
    }

    void ReleasePools( const Namespace ns );

    template< typename tT, typename tBase >
    bool AddPoolExt( AbstractObjectPool< tBase > *const pool, const Namespace ns = 0u )
    {
        const std::type_index typeID = typeid( tT );

        if ( !mPools.HasName( typeID, ns ) )
        {
            mPools.Add( pool, typeID, ns );

            return true;
        }
        else
        {
            Console::Errorf( LOG( "Pool already registered." ) );
        }

        // never happens
        return false;
    }

    template< typename tT >
    AbstractObjectPool< tT > *AddPoolExt( const Namespace ns = 0U, size_t capacity = 500 )
    {
        return AddPoolExt< tT, tT >( ns, capacity );
    }

    template< typename tT, typename tBase >
    AbstractObjectPool< tBase > *AddPoolExt( const Namespace ns = 0U, size_t capacity = 500 )
    {
        const std::type_index typeID = typeid( tT );
        ObjectPool< tT, tBase > *pool = nullptr;

        if ( !mPools.HasName( typeID, ns ) )
        {
            pool = new ObjectPool< tT, tBase >( GetManagers()->factory->GetFactory< tT >(), capacity );
            mPools.Add( pool, typeID, ns );
        }

        return pool;
    }

    template< typename tT >
    AbstractObjectPool< tT > *AddPool( const Namespace ns = 0U, size_t capacity = 500 )
    {
        return AddPool< tT, tT>( ns, capacity );
    }

    template< typename tT, typename tBase >
    AbstractObjectPool< tBase > *AddPool( const Namespace ns = 0U, size_t capacity = 500 )
    {
        return AddPool< tT, tBase, PoolableInstantiator< tT, tBase > >( ns, capacity );
    }

    template< typename tT, typename tBase, typename tInstantiator >
    AbstractObjectPool< tBase > *AddPool( const Namespace ns = 0U, size_t capacity = 500 )
    {
        const std::type_index typeID = typeid( tT );
        ObjectPool< tT, tBase, tInstantiator > *pool = nullptr;

        if ( !mPools.HasName( typeID, ns ) )
        {
            pool = new ObjectPool< tT, tBase, tInstantiator >( capacity );
            mPools.Add( pool, typeID, ns );
        }

        return pool;
    }

    template< typename tT >
    AbstractObjectPool< tT > *GetPool( const Namespace ns = 0u ) const
    {
        return GetPool< tT, tT >( ns );
    }

    template< typename tT, typename tBase >
    AbstractObjectPool< tBase > *GetPool( const Namespace ns = 0u ) const
    {
        return static_cast< AbstractObjectPool< tBase > * >( mPools.Get( typeid( tT ), ns ) );
    }

    bool HasIdentifiedPool( const Namespace ns );

    template< typename tT >
    bool HasPool( const Namespace ns = 0u )
    {
        return mPools.HasName( typeid( tT ), ns );
    }


private:

    NamespaceStorage< std::type_index, AbstractPool > mPools;
};

#endif