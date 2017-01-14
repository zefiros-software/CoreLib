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

#include "common/namespace.h"

#include "engineTest.h"

namespace
{

    TEST( Namespace, DefaultConstruction )
    {
        const Namespace ns;

        EXPECT_EQ( 0, ns.GetAddinNamespace() );
        EXPECT_EQ( 0, ns.GetPluginNamespace() );

        EXPECT_EQ( 0u, ns.GetNamespace() );
    }

    TEST( Namespace, NamespaceConstruction )
    {
        // ![SeperateNS]
        const Namespace ns( Namespace( 1, 2 ), Namespace( 3, 4 ) );

        EXPECT_EQ( 1, ns.GetPluginNamespace() );
        EXPECT_EQ( 4, ns.GetAddinNamespace() );

        EXPECT_EQ( Namespace( 1, 0 ), ns.GetPlugin() );
        EXPECT_EQ( Namespace( 0, 4 ), ns.GetAddin() );

        EXPECT_TRUE( ns.IsAddin() );

        EXPECT_EQ( Namespace( 1, 4 ).GetNamespace(), ns.GetNamespace() );
        // ![SeperateNS]
    }

    TEST( Namespace, NamespaceConstructionWithU32 )
    {
        const Namespace ns( Namespace( 1, 2 ), 4 );

        EXPECT_EQ( 1, ns.GetPluginNamespace() );
        EXPECT_EQ( 0, ns.GetAddinNamespace() );

        EXPECT_EQ( Namespace( 1, 0 ).GetNamespace(), ns.GetNamespace() );
    }

    TEST( Namespace, FullNamespaceConstruction )
    {
        const Namespace ns( Namespace( 1, 2 ).GetNamespace() );

        EXPECT_EQ( 1, ns.GetPluginNamespace() );
        EXPECT_EQ( 2, ns.GetAddinNamespace() );

        EXPECT_EQ( Namespace( 1, 2 ).GetNamespace(), ns.GetNamespace() );
    }

    TEST( Namespace, IntegerConstruction )
    {
        const Namespace ns( 1, 2 );

        EXPECT_EQ( 1, ns.GetPluginNamespace() );
        EXPECT_EQ( 2, ns.GetAddinNamespace() );

        EXPECT_EQ( Namespace( 1, 2 ).GetNamespace(), ns.GetNamespace() );
    }

    TEST( Namespace, LargeIntegerConstruction )
    {
        // ![GetNamespace]
        const U16 u16max = std::numeric_limits<U16>::max();
        const U32 u32max = std::numeric_limits<U32>::max();

        Namespace ns( u16max, u16max );

        EXPECT_EQ( u16max, ns.GetPluginNamespace() );
        EXPECT_EQ( u16max, ns.GetAddinNamespace() );

        EXPECT_EQ( u32max, ns.GetNamespace() );
        // ![GetNamespace]
    }

    TEST( Namespace, LargeIntegerConstruction2 )
    {
        // ![IsAddin]
        const U16 u16max = std::numeric_limits<U16>::max();

        Namespace ns( u16max, 0 );

        EXPECT_EQ( u16max, ns.GetPluginNamespace() );
        EXPECT_EQ( 0, ns.GetAddinNamespace() );

        EXPECT_FALSE( ns.IsAddin() );

        EXPECT_EQ( u16max, ns.GetNamespace() );
        // ![IsAddin]
    }

    TEST( Namespace, LargeIntegerConstruction3 )
    {
        const U16 u16max = std::numeric_limits<U16>::max();

        Namespace ns( 0, u16max );

        EXPECT_EQ( 0, ns.GetPluginNamespace() );
        EXPECT_EQ( u16max, ns.GetAddinNamespace() );

        EXPECT_EQ( ( u16max << 16 ), ns.GetNamespace() );
    }

    TEST( Namespace, Equality, Equal )
    {
        const U32 random = ::Test::GenerateRandomU32();

        Namespace ns( random );

        EXPECT_EQ( ns, Namespace( random ) );
    }

    TEST( Namespace, Equality, NotEqual )
    {
        U32 random = ::Test::GenerateRandomU32();
        random = random == 0 ? 1 : random;

        Namespace ns( random );

        EXPECT_NE( ns, Namespace( 0 ) );
    }

    TEST( Namespace, Cast )
    {
        const U32 random = ::Test::GenerateRandomU32();
        Namespace ns( random );

        EXPECT_EQ( random, ns() );
    }

    TEST( Namespace, Hashable )
    {
        const std::unordered_set< Namespace > set;
    }

}