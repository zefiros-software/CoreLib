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
#ifndef __ENGINE_EVENTMANAGER_H__
#define __ENGINE_EVENTMANAGER_H__

#include "events/abstract/abstractObserver.h"

#include "manager/abstract/abstractManager.h"

#include "events/observer.h"

#include "threading/spinlock.h"

#include <unordered_map>
#include <assert.h>
#include <vector>
#include <mutex>
#include <typeindex>

/// @addtogroup Events
/// @{

class EventManager
    : public AbstractManager
{
public:

    EventManager();

    void OnRelease() override;

    template< typename tT >
    const tT *Add( AbstractObserver *observer )
    {
        std::lock_guard< SpinLock > lock( mLock );
        mOberservers[ GetClassID< tT >() ].push_back( observer );

        return nullptr;
    }

    template< typename tT >
    const tT *Remove( const AbstractObserver *observer, bool allowDelete = true )
    {
        std::lock_guard< SpinLock > lock( mLock );

        std::vector< AbstractObserver * > &observers = mOberservers[ GetClassID< tT >() ];

        for ( auto it = observers.begin(), end = observers.end(); it != end; ++it )
        {
            if ( *observer == **it )
            {
                if ( allowDelete )
                {
                    delete *it;
                }

                observers.erase( it );
                break;
            }
        }

        return nullptr;
    }

    template< typename tT >
    void Post( const tT &event )
    {
        mLock.lock();

        std::vector< AbstractObserver * > observers = mOberservers[GetClassID< tT >()];

        mLock.unlock();

        for ( AbstractObserver *observer : observers )
        {
            observer->Notify( event );
        }
    }

    template< typename tT >
    U32 GetClassID()
    {
        return AssignClassID< tT >( *this );
    }

private:

    std::unordered_map< U32, std::vector< AbstractObserver * > > mOberservers;
    std::unordered_map< std::type_index, U32 > mClassIDCache;

    SpinLock mClassIDLock;
    U32 mClassIDCounter;

    SpinLock mLock;

    template< typename tT >
    static U32 AssignClassID( EventManager &eventManager )
    {
        static U32 mClassId = CacheClassID< tT >( eventManager );

        assert( mClassId > 0 && "A class ID of 0 is not allowed." );

        return mClassId;
    }

    template< typename tT >
    static U32 CacheClassID( EventManager &eventManager )
    {
        const std::type_index typeIndex = std::type_index( typeid( tT ) );

        std::lock_guard< SpinLock > lock( eventManager.mClassIDLock );

        auto it = eventManager.mClassIDCache.find( typeIndex );

        if ( it == eventManager.mClassIDCache.end() )
        {
            eventManager.mClassIDCache.insert( std::make_pair( typeIndex, ++eventManager.mClassIDCounter ) );
            return eventManager.mClassIDCounter;
        }

        return it->second;
    }

};

template <class tC, class tN>
void AbstractManager::Observe( void( tC::* method )( const tN & ) )
{
    GetManagers()->event->Add< tN >( new Observer< tC, tN >( static_cast<tC *const>( this ), method ) );
}

template< class tC, class tN >
void AbstractManager::Unobserve( void ( tC::*method )( const tN & ) )
{
    Observer< tC, tN > observer( static_cast<tC *const>( this ), method );
    GetManagers()->event->Remove< tN >( &observer );
}

/// @}

#endif