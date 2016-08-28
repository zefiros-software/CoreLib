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

#include "manager/poolManager.h"

#include "engineTest.h"

namespace
{
    class PoolTest
    {
    public:

        void OnInit()
        {
            init = true;
        }

        void OnRelease()
        {
            released = true;
        }

        bool released = false;
        bool init = false;
    };

    ENGINE_TEST( PoolManager, Sanity )
    {
        PoolManager m;

        EXPECT_FALSE( m.HasPools( 0 ) );
    }

    ENGINE_TEST( PoolManager, AddPool )
    {
        PoolManager m;
        auto pool = m.AddPool< PoolTest >();

        EXPECT_TRUE( m.HasPool< PoolTest >() );
        EXPECT_EQ( pool, m.GetPool< PoolTest >() );

        EXPECT_TRUE( m.HasPools( 0 ) );
    }

    ENGINE_TEST( PoolManager, AddPoolTwice )
    {
        PoolManager m;
        m.AddPool< PoolTest >();
        EXPECT_ANY_THROW( m.AddPool< PoolTest >() );
    }

    ENGINE_TEST( PoolManager, ReleasePool )
    {
        PoolManager m;
        auto pool = m.AddPool< PoolTest >();

        EXPECT_TRUE( m.HasPool< PoolTest >() );
        EXPECT_EQ( pool, m.GetPool< PoolTest >() );

        m.ReleasePool< PoolTest >();

        EXPECT_FALSE( m.HasPool< PoolTest >() );
        EXPECT_EQ( nullptr, m.GetPool< PoolTest >() );
    }

    ENGINE_TEST( PoolManager, ReleasePoolNS )
    {
        PoolManager m;
        auto pool = m.AddPool< PoolTest >();

        EXPECT_TRUE( m.HasPool< PoolTest >() );
        EXPECT_EQ( pool, m.GetPool< PoolTest >() );


        auto pool2 = m.AddPool< PoolTest >( 1 );

        EXPECT_TRUE( m.HasPool< PoolTest >( 1 ) );
        EXPECT_EQ( pool2, m.GetPool< PoolTest >( 1 ) );

        m.ReleasePool< PoolTest >();

        EXPECT_FALSE( m.HasPool< PoolTest >() );
        EXPECT_EQ( nullptr, m.GetPool< PoolTest >() );

        EXPECT_TRUE( m.HasPool< PoolTest >( 1 ) );
        EXPECT_EQ( pool2, m.GetPool< PoolTest >( 1 ) );
    }

    ENGINE_TEST( PoolManager, ReleasePools )
    {
        PoolManager m;
        auto pool = m.AddPool< PoolTest >();

        EXPECT_TRUE( m.HasPool< PoolTest >() );
        EXPECT_EQ( pool, m.GetPool< PoolTest >() );


        auto pool2 = m.AddPool< PoolTest >( 1 );

        EXPECT_TRUE( m.HasPool< PoolTest >( 1 ) );
        EXPECT_EQ( pool2, m.GetPool< PoolTest >( 1 ) );

        m.ReleasePools( 1 );

        EXPECT_FALSE( m.HasPool< PoolTest >( 1 ) );
        EXPECT_EQ( nullptr, m.GetPool< PoolTest >( 1 ) );

        EXPECT_TRUE( m.HasPool< PoolTest >() );
        EXPECT_EQ( pool, m.GetPool< PoolTest >() );
    }

    ENGINE_TEST( PoolManager, OnReleaseNS )
    {
        PoolManager m;
        auto pool = m.AddPool< PoolTest >();

        EXPECT_TRUE( m.HasPool< PoolTest >() );
        EXPECT_EQ( pool, m.GetPool< PoolTest >() );


        auto pool2 = m.AddPool< PoolTest >( 1 );

        EXPECT_TRUE( m.HasPool< PoolTest >( 1 ) );
        EXPECT_EQ( pool2, m.GetPool< PoolTest >( 1 ) );

        m.OnRelease( 1 );

        EXPECT_FALSE( m.HasPool< PoolTest >( 1 ) );
        EXPECT_EQ( nullptr, m.GetPool< PoolTest >( 1 ) );

        EXPECT_TRUE( m.HasPool< PoolTest >() );
        EXPECT_EQ( pool, m.GetPool< PoolTest >() );

        EXPECT_TRUE( m.HasPools( 0 ) );
        EXPECT_FALSE( m.HasPools( 1 ) );
    }
}