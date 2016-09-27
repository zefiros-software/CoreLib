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

#pragma once
#ifndef __ENGINE_PARAMMEMORYPOOLABLEINSTANTIATOR_H__
#define __ENGINE_PARAMMEMORYPOOLABLEINSTANTIATOR_H__

#include "memory/instantiator/paramPoolableInstantiator.h"
#include "memory/instantiator/memoryPoolInstantiator.h"

template< typename tT, typename tParam, typename tBase = tT >
class ParamMemoryPoolableInstantiator
    : public ParamPoolableInstantiator< tT, tParam, tBase >
{
public:

    ParamMemoryPoolableInstantiator( tParam parameter, size_t blocksize = 1000, size_t maxBlocks = 1000 )
        : ParamPoolableInstantiator( parameter ),
          mMemoryPool( blocksize, maxBlocks )
    {
    }

    virtual tBase *Create() override
    {
        tBase *obj = mMemoryPool.Create();
        obj->Initialise( this->mParameter );
        return obj;
    }

    virtual void Destroy( tBase *object ) override
    {
        mMemoryPool.Destroy( object );
    }

    virtual AbstractInstantiator *Copy() override
    {
        return new ParamMemoryPoolableInstantiator< tT, tParam, tBase >( ParamPoolableInstantiator::mParameter,
                                                                         mMemoryPool.mBlockSize,
                                                                         mMemoryPool.mMaxBlocks );
    }

private:

    MemoryPoolInstantiator< tT, tBase > mMemoryPool;
};

#endif