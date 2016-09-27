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

#include "memory/instantiator/paramInstantiator.h"

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

        virtual bool IsDerived() const
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

        bool IsDerived() const override
        {
            return true;
        }
    };

    typedef ParamInstantiator< Child, U32, Base > ParamInstantiatorImpl;

    ENGINE_TEST( ParamInstantiator, SanityCheck )
    {
        volatile ParamInstantiatorImpl inst;
    }

    ENGINE_TEST( ParamInstantiator, Create )
    {
        ParamInstantiatorImpl inst;

        Base *child = inst.Create( 0u );
        Base *child2 = inst.Create( 42u );

        EXPECT_EQ( 0u, child->GetValue() );
        EXPECT_EQ( 42u, child2->GetValue() );

        EXPECT_TRUE( child->IsDerived() );
        EXPECT_TRUE( child->IsDerived() );

        delete child;
        delete child2;
    }

    ENGINE_TEST( ParamInstantiator, Copy )
    {
        ParamInstantiatorImpl instantiator;
        delete instantiator.Copy();
    }

}