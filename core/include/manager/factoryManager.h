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
#ifndef __ENGINE_FACTORYMANAGER_H__
#define __ENGINE_FACTORYMANAGER_H__

#include "manager/abstract/abstractManager.h"
#include "manager/poolManager.h"

#include "memory/abstract/abstractTInstantiator.h"
#include "memory/instantiator/instantiator.h"

#include "container/namespaceStorage.h"

#include "api/console.h"

class FactoryManager
    : public AbstractManager
{
public:

    virtual void OnRelease() override;

    template< typename tT >
    AbstractTInstantiator< tT > *AddFactory( Namespace ns = 0U )
    {
        return AddFactory< tT, tT >( ns );
    }

    template< typename tT, typename tBase >
    AbstractTInstantiator< tBase > *AddFactory( Namespace ns = 0U )
    {
        const std::type_index typeID = typeid( tT );
        AbstractInstantiator *instantiator = nullptr;

        if ( !mFactories.HasName( typeID, ns ) )
        {
            instantiator = new Instantiator< tT, tBase >();
            mFactories.Add( instantiator, typeID, ns );
        }
        else
        {
            Console::Errorf( LOG( "Factory already registered." ) );
        }

        return instantiator;
    }

    template< typename tT >
    bool AddFactoryExt( AbstractTInstantiator< tT > *instantiator, Namespace ns = 0u )
    {
        return AddFactoryExt< tT, tT >( instantiator, ns );
    }

    template< typename tT, typename tBase >
    bool AddFactoryExt( AbstractTInstantiator< tBase > *instantiator, Namespace ns = 0u )
    {
        const std::type_index typeID = typeid( tT );

        if ( !mFactories.HasName( typeID, ns ) )
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
    void ReleaseFactory( Namespace ns = 0u )
    {
        mFactories.RemoveObjectByName( typeid( tT ), ns );
    }

    void ReleaseFactories( Namespace ns );

    template< typename tT >
    bool HasFactory( Namespace ns = 0u )
    {
        return mFactories.HasName( typeid( tT ), ns );
    }

    template< typename tT >
    AbstractTInstantiator< tT > *GetFactory( Namespace ns = 0u ) const
    {
        return GetFactory< tT, tT >( ns );
    }

    template< typename tT, typename tBase >
    AbstractTInstantiator< tBase > *GetFactory( Namespace ns = 0u ) const
    {
        return static_cast< AbstractTInstantiator< tBase > * >( mFactories.Get( typeid( tT ), ns ) );
    }

    template< typename tT>
    tT *CreateInstance( Namespace ns = 0u )
    {
        return CreateInstance< tT, tT >( ns );
    }

    template< typename tT, typename tBase >
    tT *CreateInstance( Namespace ns = 0u )
    {
        return GetFactory< tT, tBase >( ns )->CreateInstance();
    }

private:

    NamespaceStorage< std::type_index, AbstractInstantiator > mFactories;
};

#endif