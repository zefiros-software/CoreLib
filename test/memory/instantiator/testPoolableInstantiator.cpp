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

#include "memory/instantiator/poolableInstantiator.h"

#include "engineTest.h"

namespace
{

    class Base
    {
    public:

        Base()
            : mValue( 84 )
        {
        }
        virtual ~Base()
        {
        }

        virtual bool IsDerived()
        {
            return false;
        }

        virtual U32 *GetValue()
        {
            return &mValue;
        }

        virtual void OnInit()
        {

        }

        virtual void OnRelease()
        {

        }

    private:

        U32 mValue;
    };

    class Child
        : public Base
    {
    public:

        Child()
            : mValue( nullptr )
        {
        }

        bool IsDerived() override
        {
            return true;
        }

        U32 *GetValue() override
        {
            return mValue;
        }

        void OnInit() override
        {
            mValue = new U32( *Base::GetValue() );
        }

        void OnRelease() override
        {
            delete mValue;
        }

    private:

        U32 *mValue;
    };

    typedef PoolableInstantiator< Child, Base > PoolableInstantiatorImpl;

    ENGINE_TEST( PoolableInstantiator, SanityCheck )
    {
        volatile PoolableInstantiatorImpl inst;
    }

    ENGINE_TEST( PoolableInstantiator, CreateInstance )
    {
        PoolableInstantiatorImpl inst;

        Base *child = inst.CreateInstance();
        child->OnInit();

        EXPECT_TRUE( child->IsDerived() );
        EXPECT_EQ( 84u, *child->GetValue() );

        delete child->GetValue();
        delete child;
    }

    ENGINE_TEST( PoolableInstantiator, OnInit )
    {
        PoolableInstantiatorImpl inst;

        Base *child = inst.CreateInstance();

        inst.InitialiseObject( child );

        EXPECT_TRUE( child->IsDerived() );
        EXPECT_EQ( 84u, *child->GetValue() );

        delete child->GetValue();
        delete child;
    }

    ENGINE_TEST( PoolableInstantiator, ReleaseObject )
    {
        PoolableInstantiatorImpl inst;

        Base *child = inst.CreateInstance();

        inst.InitialiseObject( child );

        EXPECT_TRUE( child->IsDerived() );
        EXPECT_EQ( 84u, *child->GetValue() );

        inst.ReleaseObject( child );

        delete child;
    }

    ENGINE_TEST( PoolableInstantiator, DestroyObject )
    {
        PoolableInstantiatorImpl inst;

        Base *child = inst.CreateInstance();

        inst.InitialiseObject( child );

        EXPECT_TRUE( child->IsDerived() );
        EXPECT_EQ( 84u, *child->GetValue() );

        inst.ReleaseObject( child );

        inst.DestroyObject( child );
    }

}