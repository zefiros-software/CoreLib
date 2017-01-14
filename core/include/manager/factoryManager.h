/**
 * @cond ___LICENSE___
 *
 * Copyright (c) 2017 Koen Visscher, Paul Visscher and individual contributors.
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
#ifndef __ENGINE_FACTORYMANAGER_H__
#define __ENGINE_FACTORYMANAGER_H__

#include "manager/abstract/abstractManager.h"

#include "memory/abstract/abstractTInstantiator.h"
#include "memory/instantiator/instantiator.h"

#include "container/namespaceStorage.h"

#include "api/console.h"

#include <typeindex>

class FactoryManager
    : public AbstractManager
{
public:

    virtual void OnRelease() override;

    template< typename tT, typename tBase = tT >
    AbstractTInstantiator< tBase > *Add( Namespace ns = 0U )
    {
        const std::type_index typeID = typeid( tT );
        AbstractInstantiator *instantiator = nullptr;

        if ( !mFactories.Has( typeID, ns ) )
        {
            instantiator = new Instantiator< tT, tBase >();
            mFactories.Add( instantiator, typeID, ns );
        }
        else
        {
            Console::Errorf( LOG( "Factory already registered." ) );
        }

        return static_cast< AbstractTInstantiator< tBase > * >( instantiator );
    }

    template< typename tT, typename tBase = tT>
    bool Add( AbstractTInstantiator< tBase > *instantiator, Namespace ns = 0u )
    {
        const std::type_index typeID = typeid( tT );

        if ( !mFactories.Has( typeID, ns ) )
        {
            mFactories.Add( instantiator, typeID, ns );

            return true;
        }
        else
        {
            Console::Errorf( LOG( "Factory already registered." ) );
        }

        return false;
    }

    template< typename tT >
    void Clear( Namespace ns = 0u )
    {
        mFactories.Remove( typeid( tT ), ns );
    }

    void ClearAll( Namespace ns = 0u );

    template< typename tT >
    bool Has( Namespace ns = 0u ) const
    {
        return mFactories.Has( typeid( tT ), ns );
    }

    template< typename tT, typename tBase = tT >
    AbstractTInstantiator< tBase > *Get( Namespace ns = 0u ) const
    {
        return static_cast< AbstractTInstantiator< tBase > * >( mFactories.Get( typeid( tT ), ns ) );
    }

    template< typename tT, typename tBase = tT >
    tT * Create( Namespace ns = 0u )
    {
        return Get< tT, tBase >( ns )->Create();
    }

private:

    NamespaceStorage< std::type_index, AbstractInstantiator > mFactories;
};

#endif