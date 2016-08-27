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

#include "memory/factory/namespaceDynamicParamFactory.h"

#include "engineTest.h"

namespace
{

    class Base
    {
    public:

        Base( U32 param )
            : mValue( param )
        {

        }

        virtual ~Base()
        {
        }

        virtual bool IsDerived()
        {
            return false;
        }

        U32 GetValue() const
        {
            return mValue;
        }

    private:

        U32 mValue;
    };

    class Child
        : public Base
    {
    public:

        Child( U32 param )
            : Base( param )
        {
        }

        bool IsDerived() override
        {
            return true;
        }
    };

    typedef NamespaceDynamicParamFactory< U32, Base, U32 > NamespaceDynamicParamFactoryImpl;

    ENGINE_TEST( NamespaceDynamicParamFactory, SanityCheck )
    {
        volatile NamespaceDynamicParamFactoryImpl factory;
    }

    ENGINE_TEST( NamespaceDynamicParamFactory, CreateInstanceNone )
    {
        NamespaceDynamicParamFactoryImpl factory;

        EXPECT_EQ( NULL, factory.CreateInstance( 0, 0 ) );
    }

    ENGINE_TEST( NamespaceDynamicParamFactory, CreateInstance )
    {
        NamespaceDynamicParamFactoryImpl factory;
        factory.Register< Base >( 0 );
        factory.Register< Child >( 1 );

        Base *base = factory.CreateInstance( 0, 42 );
        Base *base2 = factory.CreateInstance( 0, 0 );
        Base *child = factory.CreateInstance( 1, 42 );
        Base *child2 = factory.CreateInstance( 1, 0 );

        EXPECT_FALSE( base->IsDerived() );
        EXPECT_EQ( 42, base->GetValue() );

        EXPECT_FALSE( base2->IsDerived() );
        EXPECT_EQ( 0, base2->GetValue() );

        EXPECT_TRUE( child->IsDerived() );
        EXPECT_EQ( 42, child->GetValue() );

        EXPECT_TRUE( child2->IsDerived() );
        EXPECT_EQ( 0, child2->GetValue() );

        delete base;
        delete base2;
        delete child;
        delete child2;
    }

}