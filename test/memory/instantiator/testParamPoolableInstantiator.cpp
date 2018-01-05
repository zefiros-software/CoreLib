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

#include "memory/instantiator/paramPoolableInstantiator.h"

#include "engineTest.h"

namespace
{

    class Base
    {
    public:

        Base()
            : mValue(0)
        {
        }

        Base(U32 param)
            : mValue(param)
        {
        }

        virtual ~Base()
        {
        }

        virtual bool IsDerived()
        {
            return false;
        }

        virtual U32 *GetValue()
        {
            return &mValue;
        }

        virtual void OnInit()
        {

        }

        virtual void OnRelease()
        {

        }

    private:

        U32 mValue;
    };

    class Child
        : public Base
    {
    public:

        Child()
            : mValue(nullptr)
        {
        }

        Child(U32 param)
            : Base(param),
              mValue(nullptr)
        {

        }

        bool IsDerived() override
        {
            return true;
        }

        U32 *GetValue() override
        {
            return mValue;
        }

        void OnInit() override
        {
            mValue = new U32(*Base::GetValue());
        }

        void OnRelease() override
        {
            delete mValue;
        }

    private:

        U32 *mValue;
    };

    typedef ParamPoolableInstantiator< Child, U32, Base > ParamPoolableInstantiatorImpl;

    TEST(ParamPoolableInstantiator, SanityCheck)
    {
        volatile ParamPoolableInstantiatorImpl inst(42u);
    }

    TEST(ParamPoolableInstantiator, Create)
    {
        ParamPoolableInstantiatorImpl inst(42u);

        Base *child = inst.Create();
        child->OnInit();

        EXPECT_TRUE(child->IsDerived());
        EXPECT_EQ(42u, *child->GetValue());

        delete child->GetValue();
        delete child;
    }

    TEST(ParamPoolableInstantiator, OnInit)
    {
        ParamPoolableInstantiatorImpl inst(42u);

        Base *child = inst.Create();

        inst.Initialise(child);

        EXPECT_TRUE(child->IsDerived());
        EXPECT_EQ(42u, *child->GetValue());

        delete child->GetValue();
        delete child;
    }

    TEST(ParamPoolableInstantiator, Release)
    {
        ParamPoolableInstantiatorImpl inst(42u);

        Base *child = inst.Create();

        inst.Initialise(child);

        EXPECT_TRUE(child->IsDerived());
        EXPECT_EQ(42u, *child->GetValue());

        inst.Release(child);

        delete child;
    }

    TEST(ParamPoolableInstantiator, Destroy)
    {
        ParamPoolableInstantiatorImpl inst(42u);

        Base *child = inst.Create();

        inst.Initialise(child);

        EXPECT_TRUE(child->IsDerived());
        EXPECT_EQ(42u, *child->GetValue());

        inst.Release(child);

        inst.Destroy(child);
    }

    TEST(ParamPoolableInstantiator, Copy)
    {
        ParamPoolableInstantiatorImpl instantiator(42);
        delete instantiator.Copy();
    }

}