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

#include "memory/instantiator/instantiator.h"

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

        bool IsDerived() override
        {
            return true;
        }

        U32 GetValue() override
        {
            return 42;
        }
    };

    class ImplInstantiator
        : public AbstractTInstantiator< Base >
    {
    public:

        Base *Create() override
        {
            return new Child;
        }
    };

    TEST( Instantiator, SanityCheck )
    {
        volatile Instantiator< Child, Base > instantiator;
    }

    TEST( Instantiator, Copy )
    {
        Instantiator< Child, Base > instantiator;
        delete instantiator.Copy();
    }

    TEST( Instantiator, Create )
    {
        Instantiator< Child, Base > instantiator;

        Base *child = instantiator.Create();
        EXPECT_EQ( 42, child->GetValue() );
        EXPECT_TRUE( child->IsDerived() );

        delete child;
    }

}