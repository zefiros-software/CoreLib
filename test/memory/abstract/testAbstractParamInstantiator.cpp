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

#include "memory/abstract/abstractParamInstantiator.h"

#include "engineTest.h"

namespace
{
    class ImplParamInstantiator
        : public AbstractParamInstantiator< U32, U32 >
    {
    public:

        U32 *Create( const U32 &param ) override
        {
            return new U32( param );
        }

        virtual AbstractInstantiator *Copy() override
        {
            return new ImplParamInstantiator;
        }
    };

    TEST( AbstractParamInstantiator, ConstructDeconstruct )
    {
        ImplParamInstantiator inst;
    }

    TEST( AbstractParamInstantiator, Create )
    {
        AbstractParamInstantiator< U32, U32 > *inst = new ImplParamInstantiator;
        U32 *ptr = inst->Create( 42 );

        EXPECT_EQ( 42, *ptr );
        delete ptr;
        delete inst;
    }

}