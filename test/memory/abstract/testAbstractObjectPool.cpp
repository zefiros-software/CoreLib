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

#include "memory/abstract/abstractObjectPool.h"

#include "engineTest.h"

namespace
{
    class ImplObjectPool
        : public AbstractObjectPool< U32 >
    {
    public:

        U32 *Get() override
        {
            return new U32;
        }

        void Dispose( U32 *object ) override
        {
            delete object;
        }

        size_t GetBorrowedCount() const noexcept override
        {
            return 1;
        }

        size_t GetReturnedCount() const noexcept override
        {
            return 1;
        }

    };

    ENGINE_TEST( AbstractObjectPool, ConstructDeconstruct )
    {
        ImplObjectPool inst;
    }

    ENGINE_TEST( AbstractObjectPool, Get )
    {
        AbstractObjectPool< U32 > *inst = new ImplObjectPool;
        delete inst->Get();
        delete inst;
    }

    ENGINE_TEST( AbstractObjectPool, FastGet )
    {
        AbstractObjectPool< U32 > *inst = new ImplObjectPool;
        delete inst->FastGet();
        delete inst;
    }

    ENGINE_TEST( AbstractObjectPool, DisposeObject )
    {
        AbstractObjectPool< U32 > *inst = new ImplObjectPool;
        inst->Dispose( new U32 );
        delete inst;
    }

    ENGINE_TEST( AbstractObjectPool, GetDispose )
    {
        AbstractObjectPool< U32 > *inst = new ImplObjectPool;
        inst->Dispose( inst->Get() );
        delete inst;
    }

}