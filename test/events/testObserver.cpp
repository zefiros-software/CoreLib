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

#include "events/observer.h"

#include "engineTest.h"


namespace
{
    class EventTest
        : public IEvent
    {
    public:

        EventTest()
            : mVal( ::Test::GenerateRandomU32() )
        {
        }

        uint32_t mVal;
    };

    class Callback
    {
    public:

        Callback()
            : mVal( 0 ),
              mWithArg( false ),
              mNoArg( false )
        {
        }

        void OnArg( const EventTest &ev )
        {
            mWithArg = true;
            mVal = ev.mVal;
        }

        void OnNoArg()
        {
            mNoArg = true;
        }

        uint32_t mVal;
        bool mWithArg;
        bool mNoArg;
    };

    TEST( Observer, SanityCheck2 )
    {
        Observer< Callback, void > m;
    }

    TEST( Observer, Notify )
    {
        Observer<Callback, EventTest> m;
        m.Notify( IEvent() );
    }

    TEST( Observer, Notify2 )
    {
        Observer<Callback, void > m;
        m.Notify();
    }

    TEST( Observer, NotifyArg )
    {
        Callback c;
        Observer< Callback, EventTest > m( &c, &Callback::OnArg );

        EventTest e;
        m.Notify( e );

        EXPECT_EQ( c.mVal, e.mVal );
        EXPECT_TRUE( c.mWithArg );
        EXPECT_FALSE( c.mNoArg );
    }

    TEST( Observer, NotifyArg2 )
    {
        Callback c;
        Observer< Callback, void > m( &c, &Callback::OnNoArg );

        m.Notify();

        EXPECT_FALSE( c.mWithArg );
        EXPECT_TRUE( c.mNoArg );
    }

    TEST( Observer, NotifyNullPtr )
    {
        Observer< Callback, EventTest > m( nullptr, &Callback::OnArg );

        EventTest e;
        m.Notify( e );
    }

    TEST( Observer, NotifyNullPtr2 )
    {
        Observer< Callback, void > m( nullptr, &Callback::OnNoArg );

        m.Notify();
    }

    TEST( Observer, OpEq )
    {
        Observer< Callback, EventTest > m( nullptr, &Callback::OnArg );
        Callback c;
        m = Observer< Callback, EventTest >( &c, &Callback::OnArg );

        EventTest e;
        m.Notify( e );

        EXPECT_EQ( c.mVal, e.mVal );
        EXPECT_TRUE( c.mWithArg );
        EXPECT_FALSE( c.mNoArg );
    }

    TEST( Observer, OpEq2 )
    {
        Observer< Callback, void > m( nullptr, &Callback::OnNoArg );
        Callback c;
        m = Observer< Callback, void >( &c, &Callback::OnNoArg );

        m.Notify();

        EXPECT_FALSE( c.mWithArg );
        EXPECT_TRUE( c.mNoArg );
    }

    TEST( Observer, Copy )
    {
        Observer< Callback, EventTest > m( nullptr, &Callback::OnArg );
        Observer< Callback, EventTest > m2( m );

        EventTest e;
        m.Notify( e );
    }

    TEST( Observer, Copy2 )
    {
        Observer< Callback, void > m( nullptr, &Callback::OnNoArg );
        Observer< Callback, void > m2( m );

        m.Notify();
    }

    TEST( Observer, OpComp )
    {
        Observer< Callback, EventTest > m( nullptr, &Callback::OnArg );
        Observer< Callback, EventTest > m2( m );

        EXPECT_EQ( m, m2 );
    }

    TEST( Observer, OpComp2 )
    {
        Observer< Callback, EventTest > m( nullptr, &Callback::OnArg );
        Callback c;
        Observer< Callback, EventTest > m2( &c, &Callback::OnArg );

        EXPECT_NE( m, m2 );
    }

    TEST( Observer, OpComp3 )
    {
        Observer< Callback, void > m( nullptr, &Callback::OnNoArg );
        Observer< Callback, void > m2( m );

        EXPECT_EQ( m, m2 );
    }

    TEST( Observer, OpComp4 )
    {
        Observer< Callback, void > m( nullptr, &Callback::OnNoArg );
        Callback c;
        Observer< Callback, void > m2( &c, &Callback::OnNoArg );

        EXPECT_NE( m, m2 );
    }
}