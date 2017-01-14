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
#ifndef __ENGINE_OBSERVER_H__
#define __ENGINE_OBSERVER_H__

#include "events/abstract/abstractObserver.h"

template< class tC, class tN >
class Observer
    : public AbstractObserver
{
public:

    Observer()
        : mObject( nullptr ),
          mMethod( nullptr )
    {
    }

    template< typename tC2 >
    Observer( tC *const object, void ( tC2:: * method )( const tN & ) )
        : mObject( object ),
          mMethod( static_cast< void ( tC::* )( const tN & ) >( method ) )
    {
    }

    Observer( const Observer &observer )
        : AbstractObserver( observer ),
          mObject( observer.mObject ),
          mMethod( observer.mMethod )
    {
    }

    Observer &operator=( const Observer &observer )
    {
        mObject = observer.mObject;
        mMethod = observer.mMethod;

        return *this;
    }

    void Notify( const tN &event ) const
    {
        if ( mObject && mMethod )
        {
            ( mObject->*mMethod )( event );
        }
    }

    virtual void Notify( const IEvent &event ) const override
    {
        Notify( static_cast<const tN &>( event ) );
    }

    virtual bool operator==( const AbstractObserver &other ) const override
    {
        return other.GetObject() == GetObject();
    }

    virtual bool operator!=( const AbstractObserver &other ) const override
    {
        return !( *this == other );
    }

protected:

    virtual void *GetObject() const override
    {
        return mObject;
    }

private:

    tC *mObject;
    void ( tC::*mMethod )( const tN & );
};

template< typename tC >
class Observer< tC, void >
    : public AbstractObserver
{
public:

    Observer()
        : mObject( nullptr ),
          mMethod( nullptr )
    {
    }

    Observer( tC *const object, void ( tC::*method )() )
        : mObject( object ),
          mMethod( method )
    {
    }

    Observer( const Observer &observer )
        : AbstractObserver( observer ),
          mObject( observer.mObject ),
          mMethod( observer.mMethod )
    {
    }

    Observer &operator=( const Observer &observer )
    {
        mObject = observer.mObject;
        mMethod = observer.mMethod;

        return *this;
    }

    void Notify() const
    {
        if ( mObject && mMethod )
        {
            ( mObject->*mMethod )();
        }
    }

    virtual void Notify( const IEvent & ) const override
    {
        Notify();
    }

    virtual bool operator==( const AbstractObserver &other ) const override
    {
        return other.GetObject() == GetObject();
    }

    virtual bool operator!=( const AbstractObserver &other ) const override
    {
        return !( *this == other );
    }

protected:

    virtual void *GetObject() const override
    {
        return mObject;
    }

private:

    tC *mObject;
    void ( tC::*mMethod )();
};

#endif