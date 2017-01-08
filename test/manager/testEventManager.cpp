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

#include "manager/eventManager.h"
#include "events/observer.h"

#include "api/system.h"
#include "api/event.h"

#include "engineTest.h"
#include "managers.h"

namespace
{

    class TestEvent
        : public IEvent
    {
    };

    class Callback
    {
    public:

        Callback()
            : mArg( false )
        {
        }

        void OnArg( const TestEvent & )
        {
            mArg = true;
        }

        bool mArg;
    };

    TEST( EventManager, Sanity )
    {
        EventManager m;
    }

    TEST( EventManager, Post )
    {
        EventManager m;
        m.Post( IEvent() );
    }

    TEST( EventManager, Post2 )
    {
        EventManager m;
        m.Post( IEvent() );
    }

    TEST( EventManager, Add )
    {
        Callback c;
        Observer< Callback, TestEvent > o( &c, &Callback::OnArg );

        EventManager m;

        m.Add< TestEvent >( &o );
        m.Post( TestEvent() );

        EXPECT_TRUE( c.mArg );
    }

    TEST( EventManager, Remove )
    {
        Callback c;
        Observer< Callback, TestEvent > o( &c, &Callback::OnArg );

        EventManager m;

        m.Add< TestEvent >( &o );
        m.Remove< TestEvent >( &o, false );
        m.Post( TestEvent() );

        EXPECT_FALSE( c.mArg );
    }

    TEST( EventManager, Remove2 )
    {
        Callback c;
        Callback c2;
        Observer< Callback, TestEvent > o( &c, &Callback::OnArg );
        Observer< Callback, TestEvent > o2( &c2, &Callback::OnArg );

        EventManager m;

        m.Add< TestEvent >( &o );
        m.Add< TestEvent >( &o2 );
        m.Remove< TestEvent >( &o2, false );
        m.Post( TestEvent() );

        EXPECT_TRUE( c.mArg );
        EXPECT_FALSE( c2.mArg );
    }

    TEST( EventManager, Remove3 )
    {
        Callback c;
        Observer< Callback, TestEvent > *o = new Observer< Callback, TestEvent >( &c, &Callback::OnArg );

        EventManager m;

        m.Add< TestEvent >( o );
        m.Remove< TestEvent >( o );
        m.Post( TestEvent() );

        EXPECT_FALSE( c.mArg );
    }

    TEST( EventManager, RemoveNonExistent )
    {
        Callback c;
        Observer< Callback, TestEvent > o( &c, &Callback::OnArg );

        EventManager m;

        EXPECT_EQ( nullptr, m.Remove< TestEvent >( &o ) );

        EXPECT_FALSE( c.mArg );
    }

    class TestManager
        : public AbstractManager
    {
    public:

        void OnEvent( const TestEvent & )
        {
            called = true;
        }

        bool called = false;
    };

    TEST( EventManager, Observe )
    {
        Managers::InitialiseEventManager();

        TestManager m;
        m.SetManagers( System::GetManagers() );
        m.Observe( &TestManager::OnEvent );

        Event::Post( TestEvent() );

        EXPECT_TRUE( m.called );

        Managers::ReleaseEventManager();
    }

    TEST( EventManager, Unobserve )
    {
        Managers::InitialiseEventManager();

        TestManager m;
        m.SetManagers( System::GetManagers() );
        m.Observe( &TestManager::OnEvent );
        m.Unobserve( &TestManager::OnEvent );

        Event::Post( TestEvent() );

        EXPECT_FALSE( m.called );

        Managers::ReleaseEventManager();
    }
}
