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

#include "memory/abstract/abstractPoolableInstantiator.h"

#include "engineTest.h"

namespace
{

    class ImplPoolableInstantiator
        : public AbstractPoolableInstantiator< U32 >
    {
    public:

        virtual U32 *CreateInstance() override
        {
            return new U32( 0 );
        }

        virtual void DestroyObject( U32 *object ) override
        {
            delete object;
        }

        virtual void InitialiseObject( U32 *const object ) override
        {
            *object = 42;
        }

        virtual void ReleaseObject( U32 *const object ) override
        {
            *object = 0;
        }

    };

    ENGINE_TEST( AbstractPoolableInstantiator, SanityCheck )
    {
        volatile ImplPoolableInstantiator inst;
    }

    ENGINE_TEST( AbstractPoolableInstantiator, CreateInstance )
    {
        ImplPoolableInstantiator inst;

        U32 *obj = inst.CreateInstance();

        EXPECT_EQ( 0, *obj );

        delete obj;
    }

    ENGINE_TEST( AbstractPoolableInstantiator, DestroyObject )
    {
        ImplPoolableInstantiator inst;

        inst.DestroyObject( new U32 );
    }

    ENGINE_TEST( AbstractPoolableInstantiator, InitialiseObject )
    {
        ImplPoolableInstantiator inst;

        U32 num = 0;

        inst.InitialiseObject( &num );

        EXPECT_EQ( 42, num );
    }

    ENGINE_TEST( AbstractPoolableInstantiator, ReleaseObject )
    {
        ImplPoolableInstantiator inst;

        U32 num = 42;

        inst.ReleaseObject( &num );

        EXPECT_EQ( 0, num );
    }

}