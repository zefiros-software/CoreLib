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

#include "memory/pool/objectPool.h"

#include "engineTest.h"

namespace
{
    class Base
    {
    public:

        virtual ~Base()
        {
        }

        virtual void OnInit()
        {

        }

        virtual void OnRelease()
        {

        }

        virtual bool IsDerived()
        {
            return false;
        }

        virtual U32 GetValue()
        {
            return 42u;
        }
    };

    class Child
        : public Base
    {
    public:

        Child()
            : mValue(91)
        {
        }

        bool IsDerived() override
        {
            return true;
        }

        U32 GetValue() override
        {
            return mValue;
        }

        void OnInit() override
        {
            mValue = Base::GetValue();
        }

        void OnRelease() override
        {
            mValue = 0;
        }

    private:

        U32 mValue;
    };

    class ImplPoolableInstantiator
        : public AbstractPoolableInstantiator< U32 >
    {
    public:

        ImplPoolableInstantiator()
            : created(false),
              destroyed(false)
        {
        }

        ImplPoolableInstantiator(const ImplPoolableInstantiator &other)
            : created(other.created),
              destroyed(other.destroyed)
        {
        }

        U32 *Create() override
        {
            created = true;
            return new U32(0);
        }

        void Destroy(U32 *object) override
        {
            destroyed = true;
            delete object;
        }

        virtual void Initialise(U32 *const object) override
        {
            *object = 42u;
        }

        virtual void Release(U32 *const object) override
        {
            *object = 0;
        }

        virtual AbstractInstantiator *Copy() override
        {
            return new ImplPoolableInstantiator(*this);
        }

        bool created;
        bool destroyed;

    };

    typedef ObjectPool< Child, Base > ObjectPoolImpl;

    TEST(ObjectPool, SanityCheck)
    {
        volatile ObjectPoolImpl pool;
    }

    TEST(ObjectPool, CustomInstantiator)
    {
        ImplPoolableInstantiator *inst = new ImplPoolableInstantiator;
        ObjectPool< U32, U32, ImplPoolableInstantiator > pool(inst, 0);

        U32 *first = pool.Get();
        EXPECT_TRUE(inst->created);
        EXPECT_EQ(42u, *first);

        pool.Dispose(first);

        EXPECT_TRUE(inst->destroyed);
    }

    TEST(ObjectPool, TooFull)
    {
        ImplPoolableInstantiator *inst = new ImplPoolableInstantiator;
        ObjectPool< U32, U32, ImplPoolableInstantiator > pool(inst, 0);

        U32 *first = pool.Get();
        EXPECT_TRUE(inst->created);
        EXPECT_EQ(42u, *first);

        pool.FastDispose(first);

        EXPECT_TRUE(inst->destroyed);
    }

    TEST(ObjectPool, CustomInstantiatorNonDefault)
    {
        ImplPoolableInstantiator *inst = new ImplPoolableInstantiator;
        ObjectPool< U32 > pool(inst, 1);

        U32 *first = pool.Get();
        EXPECT_TRUE(inst->created);
        EXPECT_EQ(42u, *first);

        pool.Dispose(first);

        EXPECT_FALSE(inst->destroyed);
        EXPECT_EQ(0u, *first);
    }

    TEST(ObjectPool, Get)
    {
        ObjectPoolImpl pool(1);

        Base *first = pool.Get();
        EXPECT_EQ(42u, first->GetValue());

        pool.Dispose(first);

        EXPECT_EQ(0u, first->GetValue());

        Base *second = pool.Get();
        Base *third = pool.Get();

        EXPECT_EQ(first, second);
        EXPECT_NE(first, third);

        pool.Dispose(third);
        pool.Dispose(second);

        Base *fourth = pool.Get();

        EXPECT_EQ(third, fourth);

        pool.Dispose(fourth);
    }

    TEST(ObjectPool, FastGet)
    {
        ObjectPoolImpl pool;

        Base *first = pool.FastGet();
        EXPECT_EQ(91u, first->GetValue());

        pool.Dispose(first);
    }

    TEST(ObjectPool, Dispose)
    {
        ObjectPoolImpl pool;
        pool.Dispose(pool.Get());
        pool.Dispose(pool.FastGet());
    }

    TEST(ObjectPool, FastDispose)
    {
        ObjectPoolImpl pool;
        pool.FastDispose(pool.Get());
        pool.FastDispose(pool.FastGet());
    }

    TEST(ObjectPool, FastDisposeNoRelease)
    {
        ImplPoolableInstantiator *inst = new ImplPoolableInstantiator;
        ObjectPool< U32 > pool(inst, 1);

        U32 *first = pool.Get();
        EXPECT_TRUE(inst->created);
        EXPECT_EQ(42u, *first);

        pool.FastDispose(first);

        EXPECT_FALSE(inst->destroyed);
        EXPECT_EQ(42u, *first);
    }

    TEST(ObjectPool, GetBorrowedCount)
    {
        ObjectPoolImpl pool;

        EXPECT_EQ(0u, pool.GetBorrowedCount());

        Base *first = pool.FastGet();

        EXPECT_EQ(1u, pool.GetBorrowedCount());

        pool.Dispose(first);
        Base *second = pool.Get();

        EXPECT_EQ(2u, pool.GetBorrowedCount());

        pool.Dispose(second);
    }

    TEST(ObjectPool, GetReturnedCount)
    {
        ObjectPoolImpl pool;

        Base *first = pool.FastGet();

        EXPECT_EQ(0u, pool.GetReturnedCount());

        pool.Dispose(first);

        EXPECT_EQ(1u, pool.GetReturnedCount());

        Base *second = pool.Get();
        pool.Dispose(second);

        EXPECT_EQ(2u, pool.GetReturnedCount());
    }

    TEST(ObjectPool, CheckReturnedCount)
    {
        ObjectPoolImpl pool;

        delete pool.FastGet();
    }
}
