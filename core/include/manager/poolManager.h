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
    void Remove( const Namespace ns = 0u )
    {
        mPools.Remove( typeid( tT ), ns );
    }

    void ClearAll( const Namespace ns );

    template< typename tT, typename tBase = tT >
    AbstractObjectPool< tBase > *AddFromFactory( const Namespace ns = 0U, size_t capacity = 500 )
    {
        const std::type_index typeID = typeid( tT );
        ObjectPool< tT, tBase, AbstractPoolableInstantiator<tBase> > *pool = nullptr;

        if ( !mPools.Has( typeID, ns ) )
        {
            AbstractPoolableInstantiator<tBase> *inst = static_cast< AbstractPoolableInstantiator<tBase> *>
                                                        ( GetManagers()->factory->Get< tT >()->Copy() );

            pool = new ObjectPool< tT, tBase, AbstractPoolableInstantiator<tBase> >( inst, capacity );
            mPools.Add( pool, typeID, ns );
        }
        else
        {
            Console::Errorf( LOG( "Pool already registered." ) );
        }

        return pool;
    }

    template< typename tT, typename tBase = tT, typename tInstantiator = PoolableInstantiator< tT, tBase > >
    AbstractObjectPool< tBase > *Add( const Namespace ns = 0U, size_t capacity = 500 )
    {
        const std::type_index typeID = typeid( tT );
        ObjectPool< tT, tBase, tInstantiator > *pool = nullptr;

        if ( !mPools.Has( typeID, ns ) )
        {
            pool = new ObjectPool< tT, tBase, tInstantiator >( capacity );
            mPools.Add( pool, typeID, ns );
        }
        else
        {
            Console::Errorf( LOG( "Pool already registered." ) );
        }

        return pool;
    }

    template< typename tT, typename tBase = tT >
    AbstractObjectPool< tBase > *Get( const Namespace ns = 0u ) const
    {
        return static_cast< AbstractObjectPool< tBase > * >( mPools.Get( typeid( tT ), ns ) );
    }

    bool HasPools( const Namespace ns ) const;

    template< typename tT >
    bool Has( const Namespace ns = 0u ) const
    {
        return mPools.Has( typeid( tT ), ns );
    }


private:

    NamespaceStorage< std::type_index, AbstractPool > mPools;
};

#endif