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

#include "container/namespaceStorage.h"

#include "engineTest.h"

namespace
{

    TEST( NamespaceStorage, SanityCheck )
    {
        volatile NamespaceStorage< U32, U32 > storage;
    }

    TEST( NamespaceStorage, TakeProperOwnership )
    {
        NamespaceStorage< U32, U32 > storage;
        EXPECT_TRUE( storage.Add( new U32(), 0 ) );
    }

    TEST( NamespaceStorage, SimpleAddGet )
    {
        NamespaceStorage< U32, U32 > storage;

        U32 *value = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( value, 0 ) );

        EXPECT_EQ( *value, *storage.Get( 0 ) );
    }

    TEST( NamespaceStorage, NoOverwrite )
    {
        NamespaceStorage< U32, U32 > storage;

        U32 *valueA = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( valueA, 0 ) );
        EXPECT_FALSE( storage.Add( valueB, 0 ) );

        EXPECT_EQ( *valueA, *storage.Get( 0 ) );
        EXPECT_NE( *valueB, *storage.Get( 0 ) );

        EXPECT_EQ( valueA, storage.Get( 0 ) );
        EXPECT_NE( valueB, storage.Get( 0 ) );

        delete valueB;
    }

    TEST( NamespaceStorage, NoOverwrite2 )
    {
        NamespaceStorage< U32, U32 > storage;

        U32 *valueA = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( valueA, 0, Namespace( 0, 1 ) ) );
        EXPECT_FALSE( storage.Add( valueB, 0, Namespace( 0, 1 ) ) );

        EXPECT_EQ( *valueA, *storage.Get( 0, Namespace( 0, 1 ) ) );
        EXPECT_NE( *valueB, *storage.Get( 0, Namespace( 0, 1 ) ) );

        EXPECT_EQ( valueA, storage.Get( 0, Namespace( 0, 1 ) ) );
        EXPECT_NE( valueB, storage.Get( 0, Namespace( 0, 1 ) ) );

        delete valueB;
    }


    TEST( NamespaceStorage, GetNonExisting )
    {
        NamespaceStorage< U32, U32 > storage;

        EXPECT_EQ( nullptr, storage.Get( 0 ) );
    }

    TEST( NamespaceStorage, DifferentKeys )
    {
        NamespaceStorage< U32, U32 > storage;

        U32 *valueA = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( valueA, 0 ) );
        EXPECT_TRUE( storage.Add( valueB, 1 ) );

        EXPECT_EQ( *valueA, *storage.Get( 0 ) );
        EXPECT_EQ( *valueB, *storage.Get( 1 ) );

        EXPECT_EQ( valueA, storage.Get( 0 ) );
        EXPECT_EQ( valueB, storage.Get( 1 ) );
    }

    TEST( NamespaceStorage, DifferentNamespaces )
    {
        NamespaceStorage< U32, U32 > storage;

        U32 *valueA = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( valueA, 0, 0u ) );
        EXPECT_TRUE( storage.Add( valueB, 0, 1u ) );

        EXPECT_EQ( *valueA, *storage.Get( 0 ) );
        EXPECT_EQ( *valueB, *storage.Get( 0, 1u ) );

        EXPECT_EQ( valueA, storage.Get( 0 ) );
        EXPECT_EQ( valueB, storage.Get( 0, 1u ) );
    }

    TEST( NamespaceStorage, PluginNamespaceSeparation )
    {
        NamespaceStorage< U32, U32 > storage;

        U32 *valueA = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( valueA, 0, 0u ) );
        EXPECT_TRUE( storage.Add( valueB, 1, 1u ) );

        EXPECT_EQ( nullptr, storage.Get( 1, 0u ) );
        EXPECT_EQ( nullptr, storage.Get( 0, 1u ) );

        EXPECT_EQ( *valueA, *storage.Get( 0 ) );
        EXPECT_EQ( *valueB, *storage.Get( 1, 1u ) );

        EXPECT_EQ( valueA, storage.Get( 0 ) );
        EXPECT_EQ( valueB, storage.Get( 1, 1u ) );
    }

    TEST( NamespaceStorage, AddinNamespaceSeparation )
    {
        NamespaceStorage< U32, U32 > storage;

        U32 *valueA = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( valueA, 0, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB, 1, Namespace( 0, 2 ) ) );

        EXPECT_EQ( nullptr, storage.Get( 1, Namespace( 0, 1 ) ) );
        EXPECT_EQ( nullptr, storage.Get( 0, Namespace( 0, 2 ) ) );

        EXPECT_EQ( *valueA, *storage.Get( 0, Namespace( 0, 1 ) ) );
        EXPECT_EQ( *valueB, *storage.Get( 1, Namespace( 0, 2 ) ) );

        EXPECT_EQ( valueA, storage.Get( 0, Namespace( 0, 1 ) ) );
        EXPECT_EQ( valueB, storage.Get( 1, Namespace( 0, 2 ) ) );
    }

    TEST( NamespaceStorage, AddinPluginNamespaceSeparation )
    {
        NamespaceStorage< U32, U32 > storage;

        U32 *valueA = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB = new U32( ::Test::GenerateRandomU32() );

        U32 *valueC = new U32( ::Test::GenerateRandomU32() );
        U32 *valueD = new U32( ::Test::GenerateRandomU32() );

        U32 *valueA2 = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB2 = new U32( ::Test::GenerateRandomU32() );

        U32 *valueC2 = new U32( ::Test::GenerateRandomU32() );
        U32 *valueD2 = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( valueA, 0, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB, 1, Namespace( 0, 2 ) ) );
        EXPECT_TRUE( storage.Add( valueC, 2, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueD, 3, Namespace( 0, 2 ) ) );

        EXPECT_TRUE( storage.Add( valueA2, 0, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB2, 1, Namespace( 1, 2 ) ) );
        EXPECT_TRUE( storage.Add( valueC2, 4, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueD2, 5, Namespace( 1, 2 ) ) );

        EXPECT_EQ( nullptr, storage.Get( 4, Namespace( 0, 1 ) ) );
        EXPECT_EQ( nullptr, storage.Get( 5, Namespace( 0, 2 ) ) );
        EXPECT_EQ( nullptr, storage.Get( 2, Namespace( 1, 1 ) ) );
        EXPECT_EQ( nullptr, storage.Get( 3, Namespace( 1, 2 ) ) );

        EXPECT_EQ( *valueA, *storage.Get( 0, Namespace( 0, 1 ) ) );
        EXPECT_EQ( *valueB, *storage.Get( 1, Namespace( 0, 2 ) ) );
        EXPECT_EQ( *valueC, *storage.Get( 2, Namespace( 0, 1 ) ) );
        EXPECT_EQ( *valueD, *storage.Get( 3, Namespace( 0, 2 ) ) );

        EXPECT_EQ( *valueA2, *storage.Get( 0, Namespace( 1, 1 ) ) );
        EXPECT_EQ( *valueB2, *storage.Get( 1, Namespace( 1, 2 ) ) );
        EXPECT_EQ( *valueC2, *storage.Get( 4, Namespace( 1, 1 ) ) );
        EXPECT_EQ( *valueD2, *storage.Get( 5, Namespace( 1, 2 ) ) );

        EXPECT_EQ( valueA, storage.Get( 0, Namespace( 0, 1 ) ) );
        EXPECT_EQ( valueB, storage.Get( 1, Namespace( 0, 2 ) ) );
        EXPECT_EQ( valueC, storage.Get( 2, Namespace( 0, 1 ) ) );
        EXPECT_EQ( valueD, storage.Get( 3, Namespace( 0, 2 ) ) );

        EXPECT_EQ( valueA2, storage.Get( 0, Namespace( 1, 1 ) ) );
        EXPECT_EQ( valueB2, storage.Get( 1, Namespace( 1, 2 ) ) );
        EXPECT_EQ( valueC2, storage.Get( 4, Namespace( 1, 1 ) ) );
        EXPECT_EQ( valueD2, storage.Get( 5, Namespace( 1, 2 ) ) );
    }

    TEST( NamespaceStorage, RemoveObjects )
    {
        NamespaceStorage< U32, U32 > storage;

        U32 *valueA = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB = new U32( ::Test::GenerateRandomU32() );

        U32 *valueC = new U32( ::Test::GenerateRandomU32() );
        U32 *valueD = new U32( ::Test::GenerateRandomU32() );

        U32 *valueA2 = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB2 = new U32( ::Test::GenerateRandomU32() );

        U32 *valueC2 = new U32( ::Test::GenerateRandomU32() );
        U32 *valueD2 = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( valueA, 0, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB, 1, Namespace( 0, 2 ) ) );
        EXPECT_TRUE( storage.Add( valueC, 2, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueD, 3, Namespace( 0, 2 ) ) );

        EXPECT_TRUE( storage.Add( valueA2, 0, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB2, 1, Namespace( 1, 2 ) ) );
        EXPECT_TRUE( storage.Add( valueC2, 4, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueD2, 5, Namespace( 1, 2 ) ) );

        storage.Clear();

        EXPECT_EQ( nullptr, storage.Get( 0, Namespace( 0, 1 ) ) );
        EXPECT_EQ( nullptr, storage.Get( 1, Namespace( 0, 2 ) ) );
        EXPECT_EQ( nullptr, storage.Get( 2, Namespace( 0, 1 ) ) );
        EXPECT_EQ( nullptr, storage.Get( 3, Namespace( 0, 2 ) ) );

        EXPECT_EQ( nullptr, storage.Get( 0, Namespace( 1, 1 ) ) );
        EXPECT_EQ( nullptr, storage.Get( 1, Namespace( 1, 2 ) ) );
        EXPECT_EQ( nullptr, storage.Get( 4, Namespace( 1, 1 ) ) );
        EXPECT_EQ( nullptr, storage.Get( 5, Namespace( 1, 2 ) ) );
    }


    TEST( NamespaceStorage, RemoveObjectsByPluginNamespace )
    {
        NamespaceStorage< U32, U32 > storage;

        U32 *valueA = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB = new U32( ::Test::GenerateRandomU32() );

        U32 *valueC = new U32( ::Test::GenerateRandomU32() );
        U32 *valueD = new U32( ::Test::GenerateRandomU32() );

        U32 *valueA2 = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB2 = new U32( ::Test::GenerateRandomU32() );

        U32 *valueC2 = new U32( ::Test::GenerateRandomU32() );
        U32 *valueD2 = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( valueA, 0, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB, 1, Namespace( 0, 2 ) ) );
        EXPECT_TRUE( storage.Add( valueC, 2, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueD, 3, Namespace( 0, 2 ) ) );

        EXPECT_TRUE( storage.Add( valueA2, 0, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB2, 1, Namespace( 1, 2 ) ) );
        EXPECT_TRUE( storage.Add( valueC2, 4, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueD2, 5, Namespace( 1, 2 ) ) );

        storage.Clear( 0u );

        EXPECT_FALSE( storage.HasNamespace( 0u ) );
        EXPECT_FALSE( storage.HasNamespace( Namespace( 0, 1 ) ) );
        EXPECT_FALSE( storage.HasNamespace( Namespace( 0, 2 ) ) );

        EXPECT_TRUE( storage.HasNamespace( 1u ) );
        EXPECT_TRUE( storage.HasNamespace( Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.HasNamespace( Namespace( 1, 2 ) ) );

        EXPECT_FALSE( storage.Has( 0, Namespace( 0, 1 ) ) );
        EXPECT_FALSE( storage.Has( 1, Namespace( 0, 2 ) ) );
        EXPECT_FALSE( storage.Has( 2, Namespace( 0, 1 ) ) );
        EXPECT_FALSE( storage.Has( 3, Namespace( 0, 2 ) ) );

        EXPECT_TRUE( storage.Has( 0, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Has( 1, Namespace( 1, 2 ) ) );
        EXPECT_TRUE( storage.Has( 4, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Has( 5, Namespace( 1, 2 ) ) );

        EXPECT_EQ( nullptr, storage.Get( 0, Namespace( 0, 1 ) ) );
        EXPECT_EQ( nullptr, storage.Get( 1, Namespace( 0, 2 ) ) );
        EXPECT_EQ( nullptr, storage.Get( 2, Namespace( 0, 1 ) ) );
        EXPECT_EQ( nullptr, storage.Get( 3, Namespace( 0, 2 ) ) );

        EXPECT_EQ( *valueA2, *storage.Get( 0, Namespace( 1, 1 ) ) );
        EXPECT_EQ( *valueB2, *storage.Get( 1, Namespace( 1, 2 ) ) );
        EXPECT_EQ( *valueC2, *storage.Get( 4, Namespace( 1, 1 ) ) );
        EXPECT_EQ( *valueD2, *storage.Get( 5, Namespace( 1, 2 ) ) );

        EXPECT_EQ( valueA2, storage.Get( 0, Namespace( 1, 1 ) ) );
        EXPECT_EQ( valueB2, storage.Get( 1, Namespace( 1, 2 ) ) );
        EXPECT_EQ( valueC2, storage.Get( 4, Namespace( 1, 1 ) ) );
        EXPECT_EQ( valueD2, storage.Get( 5, Namespace( 1, 2 ) ) );
    }

    TEST( NamespaceStorage, RemoveObjectsByAddinNamespace )
    {
        NamespaceStorage< U32, U32 > storage;

        U32 *valueA = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB = new U32( ::Test::GenerateRandomU32() );

        U32 *valueC = new U32( ::Test::GenerateRandomU32() );
        U32 *valueD = new U32( ::Test::GenerateRandomU32() );

        U32 *valueA2 = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB2 = new U32( ::Test::GenerateRandomU32() );

        U32 *valueC2 = new U32( ::Test::GenerateRandomU32() );
        U32 *valueD2 = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( valueA, 0, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB, 1, Namespace( 0, 2 ) ) );
        EXPECT_TRUE( storage.Add( valueC, 2, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueD, 3, Namespace( 0, 2 ) ) );

        EXPECT_TRUE( storage.Add( valueA2, 0, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB2, 1, Namespace( 1, 2 ) ) );
        EXPECT_TRUE( storage.Add( valueC2, 4, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueD2, 5, Namespace( 1, 2 ) ) );

        storage.Clear( Namespace( 0, 1 ) );

        EXPECT_TRUE( storage.HasNamespace( 0u ) );
        EXPECT_FALSE( storage.HasNamespace( Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.HasNamespace( Namespace( 0, 2 ) ) );

        EXPECT_TRUE( storage.HasNamespace( 1u ) );
        EXPECT_TRUE( storage.HasNamespace( Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.HasNamespace( Namespace( 1, 2 ) ) );

        EXPECT_FALSE( storage.Has( 0, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Has( 1, Namespace( 0, 2 ) ) );
        EXPECT_FALSE( storage.Has( 2, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Has( 3, Namespace( 0, 2 ) ) );

        EXPECT_TRUE( storage.Has( 0, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Has( 1, Namespace( 1, 2 ) ) );
        EXPECT_TRUE( storage.Has( 4, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Has( 5, Namespace( 1, 2 ) ) );

        EXPECT_EQ( nullptr, storage.Get( 0, Namespace( 0, 1 ) ) );
        EXPECT_EQ( *valueB, *storage.Get( 1, Namespace( 0, 2 ) ) );
        EXPECT_EQ( nullptr, storage.Get( 2, Namespace( 0, 1 ) ) );
        EXPECT_EQ( *valueD, *storage.Get( 3, Namespace( 0, 2 ) ) );

        EXPECT_EQ( *valueA2, *storage.Get( 0, Namespace( 1, 1 ) ) );
        EXPECT_EQ( *valueB2, *storage.Get( 1, Namespace( 1, 2 ) ) );
        EXPECT_EQ( *valueC2, *storage.Get( 4, Namespace( 1, 1 ) ) );
        EXPECT_EQ( *valueD2, *storage.Get( 5, Namespace( 1, 2 ) ) );

        EXPECT_EQ( valueB, storage.Get( 1, Namespace( 0, 2 ) ) );
        EXPECT_EQ( valueD, storage.Get( 3, Namespace( 0, 2 ) ) );

        EXPECT_EQ( valueA2, storage.Get( 0, Namespace( 1, 1 ) ) );
        EXPECT_EQ( valueB2, storage.Get( 1, Namespace( 1, 2 ) ) );
        EXPECT_EQ( valueC2, storage.Get( 4, Namespace( 1, 1 ) ) );
        EXPECT_EQ( valueD2, storage.Get( 5, Namespace( 1, 2 ) ) );
    }

    TEST( NamespaceStorage, RemoveObjectsByAddinNamespace2 )
    {
        NamespaceStorage< U32, U32 > storage;

        U32 *valueA = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB = new U32( ::Test::GenerateRandomU32() );

        U32 *valueA2 = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB2 = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( valueA, 0, Namespace( 0, 0 ) ) );
        EXPECT_TRUE( storage.Add( valueB, 2, Namespace( 0, 0 ) ) );

        EXPECT_TRUE( storage.Add( valueA2, 6, Namespace( 1, 0 ) ) );
        EXPECT_TRUE( storage.Add( valueB2, 8, Namespace( 1, 0 ) ) );

        storage.Clear( Namespace( 0, 0 ) );

        EXPECT_FALSE( storage.Has( 0, Namespace( 0, 0 ) ) );
        EXPECT_FALSE( storage.Has( 2, Namespace( 0, 0 ) ) );

        EXPECT_TRUE( storage.Has( 6, Namespace( 1, 0 ) ) );
        EXPECT_TRUE( storage.Has( 8, Namespace( 1, 0 ) ) );

        EXPECT_EQ( nullptr, storage.Get( 0, Namespace( 0, 0 ) ) );
        EXPECT_EQ( nullptr, storage.Get( 2, Namespace( 0, 0 ) ) );

        EXPECT_EQ( *valueA2, *storage.Get( 6, Namespace( 1, 0 ) ) );
        EXPECT_EQ( *valueB2, *storage.Get( 8, Namespace( 1, 0 ) ) );
    }

    TEST( NamespaceStorage, RemoveObjectsByPluginNamespace2 )
    {
        NamespaceStorage< U32, U32 > storage;

        U32 *valueA = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB = new U32( ::Test::GenerateRandomU32() );

        U32 *valueA2 = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB2 = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( valueA, 0, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB, 2, Namespace( 0, 1 ) ) );

        EXPECT_TRUE( storage.Add( valueA2, 6, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB2, 8, Namespace( 1, 1 ) ) );

        storage.Clear( Namespace( 1, 1 ) );

        EXPECT_TRUE( storage.Has( 0, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Has( 2, Namespace( 0, 1 ) ) );

        EXPECT_FALSE( storage.Has( 6, Namespace( 1, 1 ) ) );
        EXPECT_FALSE( storage.Has( 8, Namespace( 1, 1 ) ) );

        EXPECT_EQ( *valueA, *storage.Get( 0, Namespace( 0, 1 ) ) );
        EXPECT_EQ( *valueB, *storage.Get( 2, Namespace( 0, 1 ) ) );

        EXPECT_EQ( nullptr, storage.Get( 6, Namespace( 1, 1 ) ) );
        EXPECT_EQ( nullptr, storage.Get( 8, Namespace( 1, 1 ) ) );
    }

    TEST( NamespaceStorage, RemoveObjectsByPluginNamespace3 )
    {
        NamespaceStorage< U32, U32 > storage;

        U32 *valueA = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB = new U32( ::Test::GenerateRandomU32() );

        U32 *valueA2 = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB2 = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( valueA, 0, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB, 2, Namespace( 0, 1 ) ) );

        EXPECT_TRUE( storage.Add( valueA2, 6, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB2, 8, Namespace( 1, 1 ) ) );

        storage.Clear( Namespace( 2, 1 ) );

        EXPECT_TRUE( storage.Has( 0, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Has( 2, Namespace( 0, 1 ) ) );

        EXPECT_TRUE( storage.Has( 6, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Has( 8, Namespace( 1, 1 ) ) );

        EXPECT_EQ( *valueA, *storage.Get( 0, Namespace( 0, 1 ) ) );
        EXPECT_EQ( *valueB, *storage.Get( 2, Namespace( 0, 1 ) ) );

        EXPECT_EQ( *valueA2, *storage.Get( 6, Namespace( 1, 1 ) ) );
        EXPECT_EQ( *valueB2, *storage.Get( 8, Namespace( 1, 1 ) ) );
    }

    TEST( NamespaceStorage, RemoveObjectsByPluginNamespace4 )
    {
        NamespaceStorage< U32, U32 > storage;

        U32 *valueA = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB = new U32( ::Test::GenerateRandomU32() );

        U32 *valueA2 = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB2 = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( valueA, 0, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB, 2, Namespace( 0, 1 ) ) );

        EXPECT_TRUE( storage.Add( valueA2, 6, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB2, 8, Namespace( 1, 1 ) ) );

        storage.Clear( Namespace( 0, 2 ) );

        EXPECT_TRUE( storage.Has( 0, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Has( 2, Namespace( 0, 1 ) ) );

        EXPECT_TRUE( storage.Has( 6, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Has( 8, Namespace( 1, 1 ) ) );

        EXPECT_EQ( *valueA, *storage.Get( 0, Namespace( 0, 1 ) ) );
        EXPECT_EQ( *valueB, *storage.Get( 2, Namespace( 0, 1 ) ) );

        EXPECT_EQ( *valueA2, *storage.Get( 6, Namespace( 1, 1 ) ) );
        EXPECT_EQ( *valueB2, *storage.Get( 8, Namespace( 1, 1 ) ) );
    }

    TEST( NamespaceStorage, RemoveObjectsByPluginNamespace5 )
    {
        NamespaceStorage< U32, U32 > storage;

        U32 *valueA = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB = new U32( ::Test::GenerateRandomU32() );

        U32 *valueA2 = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB2 = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( valueA, 0, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB, 2, Namespace( 0, 1 ) ) );

        EXPECT_TRUE( storage.Add( valueA2, 6, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB2, 8, Namespace( 1, 1 ) ) );

        storage.Clear( Namespace( 2, 0 ) );

        EXPECT_TRUE( storage.Has( 0, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Has( 2, Namespace( 0, 1 ) ) );

        EXPECT_TRUE( storage.Has( 6, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Has( 8, Namespace( 1, 1 ) ) );

        EXPECT_EQ( *valueA, *storage.Get( 0, Namespace( 0, 1 ) ) );
        EXPECT_EQ( *valueB, *storage.Get( 2, Namespace( 0, 1 ) ) );

        EXPECT_EQ( *valueA2, *storage.Get( 6, Namespace( 1, 1 ) ) );
        EXPECT_EQ( *valueB2, *storage.Get( 8, Namespace( 1, 1 ) ) );
    }

    TEST( NamespaceStorage, RemoveObjectsByName )
    {
        NamespaceStorage< U32, U32 > storage;

        U32 *valueA = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB = new U32( ::Test::GenerateRandomU32() );

        U32 *valueC = new U32( ::Test::GenerateRandomU32() );
        U32 *valueD = new U32( ::Test::GenerateRandomU32() );

        U32 *valueA2 = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB2 = new U32( ::Test::GenerateRandomU32() );

        U32 *valueC2 = new U32( ::Test::GenerateRandomU32() );
        U32 *valueD2 = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( valueA, 0, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB, 1, Namespace( 0, 2 ) ) );
        EXPECT_TRUE( storage.Add( valueC, 2, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueD, 3, Namespace( 0, 2 ) ) );

        EXPECT_TRUE( storage.Add( valueA2, 0, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB2, 1, Namespace( 1, 2 ) ) );
        EXPECT_TRUE( storage.Add( valueC2, 4, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueD2, 5, Namespace( 1, 2 ) ) );

        storage.Remove( 0, Namespace( 0, 1 ) );

        EXPECT_TRUE( storage.HasNamespace( 0u ) );
        EXPECT_TRUE( storage.HasNamespace( Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.HasNamespace( Namespace( 0, 2 ) ) );

        EXPECT_TRUE( storage.HasNamespace( 1u ) );
        EXPECT_TRUE( storage.HasNamespace( Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.HasNamespace( Namespace( 1, 2 ) ) );

        EXPECT_FALSE( storage.Has( 0, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Has( 1, Namespace( 0, 2 ) ) );
        EXPECT_TRUE( storage.Has( 2, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Has( 3, Namespace( 0, 2 ) ) );

        EXPECT_TRUE( storage.Has( 0, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Has( 1, Namespace( 1, 2 ) ) );
        EXPECT_TRUE( storage.Has( 4, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Has( 5, Namespace( 1, 2 ) ) );

        EXPECT_EQ( nullptr, storage.Get( 0, Namespace( 0, 1 ) ) );
        EXPECT_EQ( *valueB, *storage.Get( 1, Namespace( 0, 2 ) ) );
        EXPECT_EQ( *valueC, *storage.Get( 2, Namespace( 0, 1 ) ) );
        EXPECT_EQ( *valueD, *storage.Get( 3, Namespace( 0, 2 ) ) );

        EXPECT_EQ( *valueA2, *storage.Get( 0, Namespace( 1, 1 ) ) );
        EXPECT_EQ( *valueB2, *storage.Get( 1, Namespace( 1, 2 ) ) );
        EXPECT_EQ( *valueC2, *storage.Get( 4, Namespace( 1, 1 ) ) );
        EXPECT_EQ( *valueD2, *storage.Get( 5, Namespace( 1, 2 ) ) );

        EXPECT_EQ( valueB, storage.Get( 1, Namespace( 0, 2 ) ) );
        EXPECT_EQ( valueD, storage.Get( 3, Namespace( 0, 2 ) ) );

        EXPECT_EQ( valueA2, storage.Get( 0, Namespace( 1, 1 ) ) );
        EXPECT_EQ( valueB2, storage.Get( 1, Namespace( 1, 2 ) ) );
        EXPECT_EQ( valueC2, storage.Get( 4, Namespace( 1, 1 ) ) );
        EXPECT_EQ( valueD2, storage.Get( 5, Namespace( 1, 2 ) ) );
    }

    TEST( NamespaceStorage, RemoveObjectByName2 )
    {
        NamespaceStorage< U32, U32 > storage;

        U32 *value = new U32( 4 );
        U32 *value2 = new U32( 0 );

        EXPECT_TRUE( storage.Add( value, 0, Namespace( 2, 1 ) ) );
        EXPECT_TRUE( storage.Add( value2, 1, Namespace( 2, 2 ) ) );

        storage.Remove( 0, Namespace( 2, 1 ) );
        EXPECT_FALSE( storage.Has( 0, Namespace( 2, 1 ) ) );
    }

    TEST( NamespaceStorage, RemoveObjectByNameNotExists )
    {
        NamespaceStorage< U32, U32 > storage;

        U32 *value = new U32( 4 );

        EXPECT_TRUE( storage.Add( value, 0 ) );

        storage.Remove( 1 );
        EXPECT_TRUE( storage.Has( 0 ) );
    }

    TEST( NamespaceStorage, RemoveObjectByNameNotExists2 )
    {
        NamespaceStorage< U32, U32 > storage;

        U32 *value = new U32( 4 );

        EXPECT_TRUE( storage.Add( value, 0 ) );

        storage.Remove( 1, Namespace( 2, 1 ) );
        EXPECT_TRUE( storage.Has( 0 ) );
    }

    TEST( NamespaceStorage, HasName )
    {
        NamespaceStorage< U32, U32 > storage;

        U32 *value = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( value, 0 ) );

        EXPECT_TRUE( storage.Has( 0 ) );
    }

    TEST( NamespaceStorage, HasNamePlugin )
    {
        NamespaceStorage< U32, U32 > storage;

        U32 *value = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( value, 0, 1u ) );

        EXPECT_TRUE( storage.Has( 0, 1u ) );
    }

    TEST( NamespaceStorage, HasNameAddin )
    {
        NamespaceStorage< U32, U32 > storage;

        U32 *value = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( value, 0, Namespace( 0, 1 ) ) );

        EXPECT_TRUE( storage.Has( 0, Namespace( 0, 1 ) ) );
        EXPECT_FALSE( storage.Has( 0 ) );
    }

    TEST( NamespaceStorage, HasNameNotExisting )
    {
        NamespaceStorage< U32, U32 > storage;

        EXPECT_FALSE( storage.Has( 0 ) );
    }

    TEST( NamespaceStorage, HasNamespace )
    {
        NamespaceStorage< U32, U32 > storage;

        U32 *value = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( value, 0 ) );

        EXPECT_TRUE( storage.HasNamespace( 0u ) );
        EXPECT_FALSE( storage.HasNamespace( Namespace( 0, 1 ) ) );
    }

    TEST( NamespaceStorage, HasNamespacePlugin )
    {
        NamespaceStorage< U32, U32 > storage;

        U32 *value = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( value, 0, 1u ) );

        EXPECT_TRUE( storage.HasNamespace( 1u ) );
        EXPECT_FALSE( storage.HasNamespace( Namespace( 0, 1 ) ) );
    }

    TEST( NamespaceStorage, HasNamespaceAddin )
    {
        NamespaceStorage< U32, U32 > storage;

        U32 *value = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( value, 0, Namespace( 0, 1 ) ) );

        EXPECT_TRUE( storage.HasNamespace( Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.HasNamespace( 0u ) );
    }

    TEST( NamespaceStorage, HasNamespaceNotExisting )
    {
        NamespaceStorage< U32, U32 > storage;

        EXPECT_FALSE( storage.HasNamespace( 0u ) );
    }
}