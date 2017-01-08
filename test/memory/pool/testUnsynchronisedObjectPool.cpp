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

#include "memory/pool/unsynchronisedObjectPool.h"

#include "engineTest.h"

namespace
{
    class Base
    {
    public:

        virtual ~Base()
        {
        }

        virtual void OnInit()
        {

        }

        virtual void OnRelease()
        {

        }

        virtual bool IsDerived()
        {
            return false;
        }

        virtual U32 GetValue()
        {
            return 42;
        }
    };

    class Child
        : public Base
    {
    public:

        Child()
            : mValue( 91 )
        {
        }

        virtual bool IsDerived() override
        {
            return true;
        }

        virtual U32 GetValue() override
        {
            return mValue;
        }

        virtual void OnInit() override
        {
            mValue = Base::GetValue();
        }

        virtual void OnRelease() override
        {
            mValue = 0;
        }

    private:

        U32 mValue;
    };

    class ImplPoolableInstantiator
        : public AbstractPoolableInstantiator< U32 >
    {
    public:

        ImplPoolableInstantiator()
            : created( false ),
              destroyed( false )
        {

        }

        ImplPoolableInstantiator( const ImplPoolableInstantiator &other )
            : created( other.created ),
              destroyed( other.destroyed )
        {

        }

        virtual U32 *Create() override
        {
            created = true;
            return new U32( 0 );
        }

        virtual void Destroy( U32 *object ) override
        {
            destroyed = true;
            delete object;
        }

        virtual void Initialise( U32 *const object ) override
        {
            *object = 42;
        }

        virtual void Release( U32 *const object ) override
        {
            *object = 0;
        }

        virtual AbstractInstantiator *Copy() override
        {
            return new ImplPoolableInstantiator( *this );
        }

        bool created;
        bool destroyed;

    };

    typedef UnsynchronisedObjectPool< Child, Base > UnsynchronisedObjectPoolImpl;

    TEST( UnsynchronisedObjectPool, SanityCheck )
    {
        volatile UnsynchronisedObjectPoolImpl pool;
    }

    TEST( UnsynchronisedObjectPool, CustomInstantiator )
    {
        ImplPoolableInstantiator *inst = new ImplPoolableInstantiator;
        UnsynchronisedObjectPool< U32, U32, ImplPoolableInstantiator > pool( inst, 0 );

        U32 *first = pool.Get();
        EXPECT_TRUE( inst->created );
        EXPECT_EQ( 42, *first );

        pool.Dispose( first );

        EXPECT_TRUE( inst->destroyed );
    }

    TEST( UnsynchronisedObjectPool, TooFull )
    {
        ImplPoolableInstantiator *inst = new ImplPoolableInstantiator;
        UnsynchronisedObjectPool< U32, U32, ImplPoolableInstantiator > pool( inst, 0 );

        U32 *first = pool.Get();
        EXPECT_TRUE( inst->created );
        EXPECT_EQ( 42, *first );

        pool.FastDispose( first );

        EXPECT_TRUE( inst->destroyed );
    }

    TEST( UnsynchronisedObjectPool, CustomInstantiatorNonDefault )
    {
        ImplPoolableInstantiator *inst = new ImplPoolableInstantiator;
        UnsynchronisedObjectPool< U32 > pool( inst, 1 );

        U32 *first = pool.Get();
        EXPECT_TRUE( inst->created );
        EXPECT_EQ( 42, *first );

        pool.Dispose( first );

        EXPECT_FALSE( inst->destroyed );
        EXPECT_EQ( 0, *first );
    }

    TEST( UnsynchronisedObjectPool, Get )
    {
        UnsynchronisedObjectPoolImpl pool( 1 );

        Base *first = pool.Get();
        EXPECT_EQ( 42, first->GetValue() );

        pool.Dispose( first );

        EXPECT_EQ( 0, first->GetValue() );

        Base *second = pool.Get();
        Base *third = pool.Get();

        EXPECT_EQ( first, second );
        EXPECT_NE( first, third );

        pool.Dispose( third );
        pool.Dispose( second );

        Base *fourth = pool.Get();

        EXPECT_EQ( third, fourth );

        pool.Dispose( fourth );
    }

    TEST( UnsynchronisedObjectPool, FastGet )
    {
        UnsynchronisedObjectPoolImpl pool;

        Base *first = pool.FastGet();
        EXPECT_EQ( 91, first->GetValue() );

        pool.Dispose( first );
    }

    TEST( UnsynchronisedObjectPool, Dispose )
    {
        UnsynchronisedObjectPoolImpl pool;
        pool.Dispose( pool.Get() );
        pool.Dispose( pool.FastGet() );
    }

    TEST( UnsynchronisedObjectPool, FastDispose )
    {
        UnsynchronisedObjectPoolImpl pool;
        pool.FastDispose( pool.Get() );
        pool.FastDispose( pool.FastGet() );
    }

    TEST( UnsynchronisedObjectPool, FastDisposeNoRelease )
    {
        ImplPoolableInstantiator *inst = new ImplPoolableInstantiator;
        UnsynchronisedObjectPool< U32 > pool( inst, 1 );

        U32 *first = pool.Get();
        EXPECT_TRUE( inst->created );
        EXPECT_EQ( 42, *first );

        pool.FastDispose( first );

        EXPECT_FALSE( inst->destroyed );
        EXPECT_EQ( 42, *first );
    }

    TEST( UnsynchronisedObjectPool, GetBorrowedCount )
    {
        UnsynchronisedObjectPoolImpl pool;

        EXPECT_EQ( 0, pool.GetBorrowedCount() );

        Base *first = pool.FastGet();

        EXPECT_EQ( 1, pool.GetBorrowedCount() );

        pool.Dispose( first );
        Base *second = pool.Get();

        EXPECT_EQ( 2, pool.GetBorrowedCount() );

        pool.Dispose( second );
    }

    TEST( UnsynchronisedObjectPool, GetReturnedCount )
    {
        UnsynchronisedObjectPoolImpl pool;

        Base *first = pool.FastGet();

        EXPECT_EQ( 0, pool.GetReturnedCount() );

        pool.Dispose( first );

        EXPECT_EQ( 1, pool.GetReturnedCount() );

        Base *second = pool.Get();
        pool.Dispose( second );

        EXPECT_EQ( 2, pool.GetReturnedCount() );
    }

}