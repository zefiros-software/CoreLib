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

#include "memory/factory/namespaceDynamicFactory.h"

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

    typedef NamespaceDynamicFactory< U32, Base > NamespaceDynamicFactoryImpl;

    TEST( NamespaceDynamicFactory, SanityCheck )
    {
        volatile NamespaceDynamicFactoryImpl factory;
    }

    TEST( NamespaceDynamicFactory, CreateNone )
    {
        NamespaceDynamicFactoryImpl factory;

        EXPECT_EQ( NULL, factory.Create( 0 ) );
    }

    TEST( NamespaceDynamicFactory, Create )
    {
        NamespaceDynamicFactoryImpl factory;
        factory.Register< Base >( 0 );

        Base *base = factory.Create( 0 );

        EXPECT_FALSE( base->IsDerived() );
        EXPECT_EQ( 0, base->GetValue() );

        delete base;
    }


    TEST( NamespaceDynamicFactory, CreateFactory )
    {
        NamespaceDynamicFactoryImpl factory;
        factory.Register< Base >( 0 );

        Base *base = factory.Get( 0 )->Create();

        EXPECT_FALSE( base->IsDerived() );
        EXPECT_EQ( 0, base->GetValue() );

        delete base;
    }

    TEST( NamespaceDynamicFactory, RegisterExt )
    {
        NamespaceDynamicFactoryImpl factory;
        factory.RegisterExt( new Instantiator< Child, Base>, 0 );

        Base *child = factory.Create( 0 );

        EXPECT_TRUE( child->IsDerived() );
        EXPECT_EQ( 42, child->GetValue() );

        delete child;
    }

    TEST( NamespaceDynamicFactory, GetNames )
    {
        NamespaceDynamicFactoryImpl factory;
        factory.RegisterExt( new Instantiator< Child, Base>, 0, Namespace( 1, 1 ) );
        factory.RegisterExt( new Instantiator< Child, Base>, 1, Namespace( 1, 1 ) );
        factory.RegisterExt( new Instantiator< Child, Base>, 2, Namespace( 1, 2 ) );

        EXPECT_THAT( factory.GetByNamespace( { 1, 1 } ), testing::UnorderedElementsAre( 0, 1 ) );
        EXPECT_THAT( factory.GetByNamespace( { 1, 2 } ), testing::UnorderedElementsAre( 2 ) );
    }

    TEST( NamespaceDynamicFactory, UnregisterClassesByNamespace )
    {
        NamespaceDynamicFactoryImpl factory;
        factory.RegisterExt( new Instantiator< Child, Base>, 0, Namespace( 1, 1 ) );
        factory.RegisterExt( new Instantiator< Child, Base>, 1, Namespace( 1, 1 ) );
        factory.RegisterExt( new Instantiator< Child, Base>, 2, Namespace( 1, 2 ) );

        EXPECT_THAT( factory.GetByNamespace( { 1, 1 } ), testing::UnorderedElementsAre( 0, 1 ) );
        factory.Clear( { 1, 1 } );
        EXPECT_EQ( 0, factory.GetByNamespace( { 1, 1 } ).size() );

        EXPECT_THAT( factory.GetByNamespace( { 1, 2 } ), testing::UnorderedElementsAre( 2 ) );
        factory.Clear( { 1, 2 } );
        EXPECT_EQ( 0, factory.GetByNamespace( { 1, 2 } ).size() );
    }

    TEST( NamespaceDynamicFactory, UnregisterClasses )
    {
        NamespaceDynamicFactoryImpl factory;
        factory.RegisterExt( new Instantiator< Child, Base>, 0, Namespace( 1, 1 ) );
        factory.RegisterExt( new Instantiator< Child, Base>, 1, Namespace( 1, 1 ) );
        factory.RegisterExt( new Instantiator< Child, Base>, 2, Namespace( 1, 2 ) );

        EXPECT_THAT( factory.GetByNamespace( { 1, 1 } ), testing::UnorderedElementsAre( 0, 1 ) );
        EXPECT_THAT( factory.GetByNamespace( { 1, 2 } ), testing::UnorderedElementsAre( 2 ) );

        factory.Clear();

        EXPECT_EQ( 0, factory.GetByNamespace( { 1, 1 } ).size() );
        EXPECT_EQ( 0, factory.GetByNamespace( { 1, 2 } ).size() );
    }

    TEST( NamespaceDynamicFactory, UnregisterClassByName )
    {
        NamespaceDynamicFactoryImpl factory;
        factory.RegisterExt( new Instantiator< Child, Base>, 0, Namespace( 1, 1 ) );
        factory.RegisterExt( new Instantiator< Child, Base>, 1, Namespace( 1, 1 ) );
        factory.RegisterExt( new Instantiator< Child, Base>, 2, Namespace( 1, 2 ) );

        EXPECT_THAT( factory.GetByNamespace( { 1, 1 } ), testing::UnorderedElementsAre( 0, 1 ) );
        EXPECT_THAT( factory.GetByNamespace( { 1, 2 } ), testing::UnorderedElementsAre( 2 ) );

        factory.Remove( 2 );

        EXPECT_FALSE( factory.Has( 2 ) );
        EXPECT_TRUE( factory.Has( 0 ) );
        EXPECT_TRUE( factory.Has( 1 ) );

        EXPECT_THAT( factory.GetByNamespace( { 1, 1 } ), testing::UnorderedElementsAre( 0, 1 ) );
        EXPECT_EQ( 0, factory.GetByNamespace( { 1, 2 } ).size() );
    }
}
