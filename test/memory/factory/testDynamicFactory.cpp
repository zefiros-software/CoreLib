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

#include "memory/factory/dynamicFactory.h"

#include "engineTest.h"

namespace
{

    class Base
    {
    public:
        virtual ~Base()
        {
        }

        virtual bool IsDerived()
        {
            return false;
        }

        virtual U32 GetValue()
        {
            return 0;
        }
    };

    class Child
        : public Base
    {
    public:

        virtual bool IsDerived() override
        {
            return true;
        }

        virtual U32 GetValue() override
        {
            return 42;
        }
    };

    class Child2
        : public Base
    {
    public:

        virtual bool IsDerived() override
        {
            return true;
        }

        virtual U32 GetValue() override
        {
            return 84;
        }
    };

    class ImplInstantiator
        : public AbstractTInstantiator< Base >
    {
    public:

        virtual Base *Create() override
        {
            return new Child;
        }

        virtual AbstractInstantiator *Copy() override
        {
            return new ImplInstantiator;
        }
    };

    class ImplInstantiator2
        : public AbstractTInstantiator< Base >
    {
    public:

        virtual Base *Create() override
        {
            return new Child2;
        }

        virtual AbstractInstantiator *Copy() override
        {
            return new ImplInstantiator2;
        }
    };

    typedef DynamicFactory< U32, Base > DynamicFactoryImpl;


    TEST( DynamicFactory, SanityCheck )
    {
        volatile DynamicFactoryImpl factory;
    }

    TEST( DynamicFactory, TakeProperOwnership )
    {
        DynamicFactoryImpl factory;
        factory.RegisterExt( new ImplInstantiator, 0 );
    }

    TEST( DynamicFactory, RegisterCreate )
    {
        DynamicFactoryImpl factory;
        EXPECT_TRUE( factory.Register< Child >( 0 ) );

        Base *first = factory.Create( 0 );

        EXPECT_EQ( 42, first->GetValue() );

        EXPECT_TRUE( first->IsDerived() );

        delete first;
    }

    TEST( DynamicFactory, NoOverwrite )
    {
        DynamicFactoryImpl factory;

        EXPECT_TRUE( factory.Register< Child >( 0 ) );
        EXPECT_FALSE( factory.Register< Child2 >( 0 ) );

        Base *first = factory.Create( 0 );
        Base *second = factory.Create( 0 );

        EXPECT_EQ( 42u, first->GetValue() );
        EXPECT_NE( 84u, second->GetValue() );

        delete first;
        delete second;
    }

    TEST( DynamicFactory, CreateNonExisting )
    {
        DynamicFactoryImpl factory;

        EXPECT_EQ( NULL, factory.Create( 0 ) );
    }

    TEST( DynamicFactory, DifferentKeys )
    {
        DynamicFactoryImpl factory;
        EXPECT_TRUE( factory.Register< Child >( 0 ) );
        EXPECT_TRUE( factory.Register< Child2 >( 1 ) );
        EXPECT_TRUE( factory.Register< Base >( 2 ) );

        Base *first = factory.Create( 0 );
        Base *second = factory.Create( 1 );
        Base *base = factory.Create( 2 );

        EXPECT_EQ( 42, first->GetValue() );
        EXPECT_EQ( 84, second->GetValue() );
        EXPECT_EQ( 0, base->GetValue() );

        EXPECT_TRUE( first->IsDerived() );
        EXPECT_TRUE( second->IsDerived() );
        EXPECT_FALSE( base->GetValue() );

        delete first;
        delete second;
        delete base;
    }

    TEST( DynamicFactory, RegisterExt )
    {
        DynamicFactoryImpl factory;

        EXPECT_TRUE( factory.RegisterExt( new ImplInstantiator, 0 ) );
        EXPECT_TRUE( factory.Register< Child >( 1 ) );

        EXPECT_TRUE( factory.RegisterExt( new ImplInstantiator2, 2 ) );
        EXPECT_TRUE( factory.Register< Child2 >( 3 ) );

        Base *firstExt = factory.Create( 0 );
        Base *first = factory.Create( 1 );

        Base *secondExt = factory.Create( 2 );
        Base *second = factory.Create( 3 );

        EXPECT_EQ( 42, first->GetValue() );
        EXPECT_EQ( 84, second->GetValue() );

        EXPECT_EQ( 42, firstExt->GetValue() );
        EXPECT_EQ( 84, secondExt->GetValue() );

        delete firstExt;
        delete first;

        delete secondExt;
        delete second;
    }

    TEST( DynamicFactory, Remove )
    {
        DynamicFactoryImpl factory;

        EXPECT_TRUE( factory.RegisterExt( new ImplInstantiator, 0 ) );
        EXPECT_TRUE( factory.Register< Child >( 1 ) );

        EXPECT_TRUE( factory.RegisterExt( new ImplInstantiator2, 2 ) );
        EXPECT_TRUE( factory.Register< Child2 >( 3 ) );

        {
            Base *firstExt = factory.Create( 0 );
            Base *first = factory.Create( 1 );

            Base *secondExt = factory.Create( 2 );
            Base *second = factory.Create( 3 );

            EXPECT_EQ( 42, first->GetValue() );
            EXPECT_EQ( 84, second->GetValue() );

            EXPECT_EQ( 42, firstExt->GetValue() );
            EXPECT_EQ( 84, secondExt->GetValue() );

            delete firstExt;
            delete first;

            delete secondExt;
            delete second;
        }

        factory.Remove( 0 );
        factory.Remove( 1 );
        factory.Remove( 2 );
        factory.Remove( 3 );

        EXPECT_TRUE( factory.Register< Child2 >( 0 ) );
        EXPECT_TRUE( factory.RegisterExt( new ImplInstantiator2, 1 ) );

        EXPECT_TRUE( factory.Register< Child >( 2 ) );
        EXPECT_TRUE( factory.RegisterExt( new ImplInstantiator, 3 ) );

        {
            Base *secondExt = factory.Create( 1 );
            Base *second = factory.Create( 0 );

            Base *first = factory.Create( 2 );
            Base *firstExt = factory.Create( 3 );

            EXPECT_EQ( 42, firstExt->GetValue() );
            EXPECT_EQ( 84, secondExt->GetValue() );

            EXPECT_EQ( 42, first->GetValue() );
            EXPECT_EQ( 84, second->GetValue() );

            delete secondExt;
            delete second;

            delete firstExt;
            delete first;
        }
    }

    TEST( DynamicFactory, HasNone )
    {
        DynamicFactoryImpl factory;

        EXPECT_FALSE( factory.Has( 0 ) );
    }

    TEST( DynamicFactory, Has )
    {
        DynamicFactoryImpl factory;
        factory.Register< Child >( 0 );

        EXPECT_TRUE( factory.Has( 0 ) );
    }

    TEST( DynamicFactory, RemoveNonExisting )
    {
        DynamicFactoryImpl factory;

        factory.Remove( 0 );
    }

    TEST( DynamicFactory, HasRemoved )
    {
        DynamicFactoryImpl factory;
        factory.Register< Child >( 0 );
        factory.Remove( 0 );

        EXPECT_FALSE( factory.Has( 0 ) );
    }
}