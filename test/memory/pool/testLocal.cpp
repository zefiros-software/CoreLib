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

#include "memory/instantiator/simplePoolableInstantiator.h"
#include "memory/pool/local.h"

#include "engineTest.h"
#include "managers.h"

namespace
{

    class ImplObjectPool
        : public AbstractObjectPool< U32 >
    {
    public:

        ImplObjectPool()
            : mBorrowedCount( 0 ),
              mReturnedCount( 0 )
        {

        }

        U32 *Get() override
        {
            ++mBorrowedCount;
            return new U32( 42 );
        }

        void Dispose( U32 *object ) override
        {
            ++mReturnedCount;
            delete object;
        }

        size_t GetBorrowedCount() const noexcept override
        {
            return mBorrowedCount;
        }

        size_t GetReturnedCount() const noexcept override
        {
            return mReturnedCount;
        }

    private:

        std::size_t mBorrowedCount;
        std::size_t mReturnedCount;
    };

    typedef ObjectPool< U32, U32, SimplePoolableInstantiator< U32, U32 > > U32Pool;

    ENGINE_TEST( Local, DefaultConstruction )
    {
        Managers::InitialisePoolManager();

        ManagerHolder *const holder = SystemManager::Get()->GetManagers();
        PoolManager *const poolMngr = holder->pool;

        U32Pool *const pool = static_cast< U32Pool * >
                              ( poolMngr->Add< U32, U32, SimplePoolableInstantiator< U32, U32> >() );

        {
            Local< U32 > local;

            EXPECT_EQ( 1, pool->GetBorrowedCount() );
        }
        EXPECT_EQ( 1, pool->GetReturnedCount() );

        Managers::ReleasePoolManager();
    }

    ENGINE_TEST( Local, ManagerConstruction )
    {
        Managers::InitialisePoolManager();

        ManagerHolder *const holder = SystemManager::Get()->GetManagers();
        PoolManager *const poolMngr = holder->pool;

        U32Pool *const pool = static_cast< U32Pool * >
                              ( poolMngr->Add< U32, U32, SimplePoolableInstantiator< U32, U32 > >() );

        {
            Local< U32 > local( holder );

            EXPECT_EQ( 1, pool->GetBorrowedCount() );
        }
        EXPECT_EQ( 1, pool->GetReturnedCount() );

        Managers::ReleasePoolManager();
    }

    ENGINE_TEST( Local, PoolConstruction )
    {
        U32Pool pool;

        {
            Local< U32 > local( &pool );

            EXPECT_EQ( 1, pool.GetBorrowedCount() );
        }
        EXPECT_EQ( 1, pool.GetReturnedCount() );
    }

    ENGINE_TEST( Local, Retrieval )
    {
        ImplObjectPool pool;
        Local< U32 > local( &pool );

        U32 *const obj = local.Get();
        EXPECT_EQ( obj, ( U32 * const )local );
        EXPECT_EQ( obj, *local );
        EXPECT_EQ( obj, local.operator->() );
        EXPECT_EQ( obj, ( ( const Local< U32 > * )&local )->operator->() );
        EXPECT_EQ( 42u, *obj );
    }

    ENGINE_TEST( Local, Recreate )
    {
        ImplObjectPool pool;
        {
            Local< U32 > local( &pool );

            U32 *const first = local;

            local.Recreate();

            U32 *const second = local;

            EXPECT_NE( first, second );
            EXPECT_EQ( 42u, *second );
        }

        EXPECT_EQ( 2, pool.GetBorrowedCount() );
        EXPECT_EQ( 2, pool.GetReturnedCount() );
    }

    class ImplObjectPoolNullptr
        : public AbstractObjectPool< U32 >
    {
    public:

        ImplObjectPoolNullptr()
            : mBorrowedCount( 0 ),
              mReturnedCount( 0 )
        {

        }

        U32 *Get() override
        {
            ++mBorrowedCount;
            return nullptr;
        }

        void Dispose( U32 *object ) override
        {
            ++mReturnedCount;
            delete object;
        }

        size_t GetBorrowedCount() const noexcept override
        {
            return mBorrowedCount;
        }

        size_t GetReturnedCount() const noexcept override
        {
            return mReturnedCount;
        }

    private:

        std::size_t mBorrowedCount;
        std::size_t mReturnedCount;
    };

    ENGINE_TEST( Local, Nullptr )
    {
        ImplObjectPoolNullptr pool;
        const Local< U32 > local( &pool );

        const U32 *const obj = local.Get();
        EXPECT_EQ( obj, nullptr );
    }

}