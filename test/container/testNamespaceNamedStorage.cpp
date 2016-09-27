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

#include "container/namespaceNamedStorage.h"

#include "engineTest.h"

namespace
{

    ENGINE_TEST( NamespaceNamedStorage, SanityCheck )
    {
        volatile NamespaceNamedStorage< U32, U32 > storage;
    }

    ENGINE_TEST( NamespaceNamedStorage, TakeProperOwnership )
    {
        NamespaceNamedStorage< U32, U32 > storage;
        EXPECT_TRUE( storage.Add( new U32(), 0 ) );
    }

    ENGINE_TEST( NamespaceNamedStorage, SimpleAddGet )
    {
        NamespaceNamedStorage< U32, U32 > storage;

        U32 *value = new U32( 4 );

        EXPECT_TRUE( storage.Add( value, 0 ) );

        EXPECT_EQ( *value, *storage.Get( 0 ) );
    }

    ENGINE_TEST( NamespaceNamedStorage, NoOverwrite )
    {
        NamespaceNamedStorage< U32, U32 > storage;

        U32 *valueA = new U32( 4 );
        U32 *valueB = new U32( 42 );

        EXPECT_TRUE( storage.Add( valueA, 0 ) );
        EXPECT_FALSE( storage.Add( valueB, 0 ) );

        EXPECT_EQ( *valueA, *storage.Get( 0 ) );
        EXPECT_NE( *valueB, *storage.Get( 0 ) );

        delete valueB;
    }

    ENGINE_TEST( NamespaceNamedStorage, NoOverwriteDifferentNamespace )
    {
        NamespaceNamedStorage< U32, U32 > storage;

        U32 *valueA = new U32( 4 );
        U32 *valueB = new U32( 42 );

        EXPECT_TRUE( storage.Add( valueA, 0 ) );
        EXPECT_FALSE( storage.Add( valueB, 0, 1u ) );

        EXPECT_EQ( *valueA, *storage.Get( 0 ) );
        EXPECT_NE( *valueB, *storage.Get( 0 ) );

        delete valueB;
    }

    ENGINE_TEST( NamespaceNamedStorage, GetNonExisting )
    {
        NamespaceNamedStorage< U32, U32 > storage;

        EXPECT_EQ( nullptr, storage.Get( 0 ) );
    }

    ENGINE_TEST( NamespaceNamedStorage, DifferentKeys )
    {
        NamespaceNamedStorage< U32, U32 > storage;

        U32 *valueA = new U32( 4 );
        U32 *valueB = new U32( 42 );

        EXPECT_TRUE( storage.Add( valueA, 0 ) );
        EXPECT_TRUE( storage.Add( valueB, 1 ) );

        EXPECT_EQ( *valueA, *storage.Get( 0 ) );
        EXPECT_EQ( *valueB, *storage.Get( 1 ) );
    }

    ENGINE_TEST( NamespaceNamedStorage, PluginNamespaceSeparation )
    {
        NamespaceNamedStorage< U32, U32 > storage;

        U32 *valueA = new U32( 4 );
        U32 *valueB = new U32( 42 );

        EXPECT_TRUE( storage.Add( valueA, 0, 0u ) );
        EXPECT_TRUE( storage.Add( valueB, 1, 1u ) );

        EXPECT_EQ( *valueA, *storage.Get( 0 ) );
        EXPECT_EQ( *valueB, *storage.Get( 1 ) );
    }

    ENGINE_TEST( NamespaceNamedStorage, AddinNamespaceSeparation )
    {
        NamespaceNamedStorage< U32, U32 > storage;

        U32 *valueA = new U32( 4 );
        U32 *valueB = new U32( 42 );

        EXPECT_TRUE( storage.Add( valueA, 0, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB, 1, Namespace( 0, 2 ) ) );

        EXPECT_EQ( *valueA, *storage.Get( 0 ) );
        EXPECT_EQ( *valueB, *storage.Get( 1 ) );
    }

    ENGINE_TEST( NamespaceNamedStorage, AddinPluginNamespaceSeparation )
    {
        NamespaceNamedStorage< U32, U32 > storage;

        U32 *valueA = new U32( 4 );
        U32 *valueB = new U32( 42 );

        U32 *valueC = new U32( 442 );
        U32 *valueD = new U32( 4242 );

        U32 *valueA2 = new U32( 4 );
        U32 *valueB2 = new U32( 42 );

        U32 *valueC2 = new U32( 442 );
        U32 *valueD2 = new U32( 4242 );

        EXPECT_TRUE( storage.Add( valueA, 0, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB, 1, Namespace( 0, 2 ) ) );
        EXPECT_TRUE( storage.Add( valueC, 2, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueD, 3, Namespace( 0, 2 ) ) );

        EXPECT_TRUE( storage.Add( valueA2, 4, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB2, 5, Namespace( 1, 2 ) ) );
        EXPECT_TRUE( storage.Add( valueC2, 6, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueD2, 7, Namespace( 1, 2 ) ) );

        EXPECT_EQ( *valueA, *storage.Get( 0 ) );
        EXPECT_EQ( *valueB, *storage.Get( 1 ) );
        EXPECT_EQ( *valueC, *storage.Get( 2 ) );
        EXPECT_EQ( *valueD, *storage.Get( 3 ) );

        EXPECT_EQ( *valueA2, *storage.Get( 4 ) );
        EXPECT_EQ( *valueB2, *storage.Get( 5 ) );
        EXPECT_EQ( *valueC2, *storage.Get( 6 ) );
        EXPECT_EQ( *valueD2, *storage.Get( 7 ) );
    }

    ENGINE_TEST( NamespaceNamedStorage, RemoveObjects )
    {
        NamespaceNamedStorage< U32, U32 > storage;

        U32 *valueA = new U32( 4 );
        U32 *valueB = new U32( 42 );

        U32 *valueC = new U32( 442 );
        U32 *valueD = new U32( 4242 );

        U32 *valueA2 = new U32( 4 );
        U32 *valueB2 = new U32( 42 );

        U32 *valueC2 = new U32( 442 );
        U32 *valueD2 = new U32( 4242 );

        EXPECT_TRUE( storage.Add( valueA, 0, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB, 1, Namespace( 0, 2 ) ) );
        EXPECT_TRUE( storage.Add( valueC, 2, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueD, 3, Namespace( 0, 2 ) ) );

        EXPECT_TRUE( storage.Add( valueA2, 4, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB2, 5, Namespace( 1, 2 ) ) );
        EXPECT_TRUE( storage.Add( valueC2, 6, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueD2, 7, Namespace( 1, 2 ) ) );

        storage.Clear();

        EXPECT_EQ( nullptr, storage.Get( 0 ) );
        EXPECT_EQ( nullptr, storage.Get( 1 ) );
        EXPECT_EQ( nullptr, storage.Get( 2 ) );
        EXPECT_EQ( nullptr, storage.Get( 3 ) );

        EXPECT_EQ( nullptr, storage.Get( 4 ) );
        EXPECT_EQ( nullptr, storage.Get( 5 ) );
        EXPECT_EQ( nullptr, storage.Get( 6 ) );
        EXPECT_EQ( nullptr, storage.Get( 7 ) );

        std::vector< U32 > result = storage.GetNames( 0u );
        std::vector< U32 > result2 = storage.GetNames( 1u );

        EXPECT_EQ( 0, result.size() );
        EXPECT_EQ( 0, result2.size() );
    }


    ENGINE_TEST( NamespaceNamedStorage, RemoveObjectsByPluginNamespace )
    {
        NamespaceNamedStorage< U32, U32 > storage;

        U32 *valueA = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB = new U32( ::Test::GenerateRandomU32() );

        U32 *valueC = new U32( ::Test::GenerateRandomU32() );
        U32 *valueD = new U32( ::Test::GenerateRandomU32() );

        U32 *valueE = new U32( ::Test::GenerateRandomU32() );
        U32 *valueF = new U32( ::Test::GenerateRandomU32() );

        U32 *valueA2 = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB2 = new U32( ::Test::GenerateRandomU32() );

        U32 *valueC2 = new U32( ::Test::GenerateRandomU32() );
        U32 *valueD2 = new U32( ::Test::GenerateRandomU32() );

        U32 *valueE2 = new U32( ::Test::GenerateRandomU32() );
        U32 *valueF2 = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( valueA, 0, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB, 1, Namespace( 0, 2 ) ) );
        EXPECT_TRUE( storage.Add( valueC, 2, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueD, 3, Namespace( 0, 2 ) ) );
        EXPECT_TRUE( storage.Add( valueE, 4, Namespace( 0, 0 ) ) );
        EXPECT_TRUE( storage.Add( valueF, 5, Namespace( 0, 0 ) ) );

        EXPECT_TRUE( storage.Add( valueA2, 6, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB2, 7, Namespace( 1, 2 ) ) );
        EXPECT_TRUE( storage.Add( valueC2, 8, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueD2, 9, Namespace( 1, 2 ) ) );
        EXPECT_TRUE( storage.Add( valueE2, 10, Namespace( 1, 0 ) ) );
        EXPECT_TRUE( storage.Add( valueF2, 11, Namespace( 1, 0 ) ) );

        storage.Clear( 0u );

        EXPECT_FALSE( storage.Has( 0 ) );
        EXPECT_FALSE( storage.Has( 1 ) );
        EXPECT_FALSE( storage.Has( 2 ) );
        EXPECT_FALSE( storage.Has( 3 ) );
        EXPECT_FALSE( storage.Has( 4 ) );
        EXPECT_FALSE( storage.Has( 5 ) );

        EXPECT_TRUE( storage.Has( 6 ) );
        EXPECT_TRUE( storage.Has( 7 ) );
        EXPECT_TRUE( storage.Has( 8 ) );
        EXPECT_TRUE( storage.Has( 9 ) );
        EXPECT_TRUE( storage.Has( 10 ) );
        EXPECT_TRUE( storage.Has( 11 ) );

        EXPECT_EQ( nullptr, storage.Get( 0 ) );
        EXPECT_EQ( nullptr, storage.Get( 1 ) );
        EXPECT_EQ( nullptr, storage.Get( 2 ) );
        EXPECT_EQ( nullptr, storage.Get( 3 ) );
        EXPECT_EQ( nullptr, storage.Get( 4 ) );
        EXPECT_EQ( nullptr, storage.Get( 5 ) );

        EXPECT_EQ( *valueA2, *storage.Get( 6 ) );
        EXPECT_EQ( *valueB2, *storage.Get( 7 ) );
        EXPECT_EQ( *valueC2, *storage.Get( 8 ) );
        EXPECT_EQ( *valueD2, *storage.Get( 9 ) );
        EXPECT_EQ( *valueE2, *storage.Get( 10 ) );
        EXPECT_EQ( *valueF2, *storage.Get( 11 ) );

        std::vector< U32 > names;
        names.push_back( 6 );
        names.push_back( 7 );
        names.push_back( 8 );
        names.push_back( 9 );
        names.push_back( 10 );
        names.push_back( 11 );

        std::vector< U32 > result = storage.GetNames( 0 );
        std::vector< U32 > result2 = storage.GetNames( 1u );

        EXPECT_EQ( 0, result.size() );
        EXPECT_TRUE( std::is_permutation( names.begin(), names.end(), result2.begin() ) );
    }

    ENGINE_TEST( NamespaceNamedStorage, RemoveObjectsByAddinNamespace )
    {
        NamespaceNamedStorage< U32, U32 > storage;

        U32 *valueA = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB = new U32( ::Test::GenerateRandomU32() );

        U32 *valueC = new U32( ::Test::GenerateRandomU32() );
        U32 *valueD = new U32( ::Test::GenerateRandomU32() );

        U32 *valueE = new U32( ::Test::GenerateRandomU32() );
        U32 *valueF = new U32( ::Test::GenerateRandomU32() );

        U32 *valueA2 = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB2 = new U32( ::Test::GenerateRandomU32() );

        U32 *valueC2 = new U32( ::Test::GenerateRandomU32() );
        U32 *valueD2 = new U32( ::Test::GenerateRandomU32() );

        U32 *valueE2 = new U32( ::Test::GenerateRandomU32() );
        U32 *valueF2 = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( valueA, 0, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB, 1, Namespace( 0, 2 ) ) );
        EXPECT_TRUE( storage.Add( valueC, 2, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueD, 3, Namespace( 0, 2 ) ) );
        EXPECT_TRUE( storage.Add( valueE, 4, Namespace( 0, 0 ) ) );
        EXPECT_TRUE( storage.Add( valueF, 5, Namespace( 0, 0 ) ) );

        EXPECT_TRUE( storage.Add( valueA2, 6, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB2, 7, Namespace( 1, 2 ) ) );
        EXPECT_TRUE( storage.Add( valueC2, 8, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueD2, 9, Namespace( 1, 2 ) ) );
        EXPECT_TRUE( storage.Add( valueE2, 10, Namespace( 1, 0 ) ) );
        EXPECT_TRUE( storage.Add( valueF2, 11, Namespace( 1, 0 ) ) );

        storage.Clear( Namespace( 0, 1 ) );

        EXPECT_FALSE( storage.Has( 0 ) );
        EXPECT_TRUE( storage.Has( 1 ) );
        EXPECT_FALSE( storage.Has( 2 ) );
        EXPECT_TRUE( storage.Has( 3 ) );
        EXPECT_TRUE( storage.Has( 4 ) );
        EXPECT_TRUE( storage.Has( 5 ) );

        EXPECT_TRUE( storage.Has( 6 ) );
        EXPECT_TRUE( storage.Has( 7 ) );
        EXPECT_TRUE( storage.Has( 8 ) );
        EXPECT_TRUE( storage.Has( 9 ) );
        EXPECT_TRUE( storage.Has( 10 ) );
        EXPECT_TRUE( storage.Has( 11 ) );

        EXPECT_EQ( nullptr, storage.Get( 0 ) );
        EXPECT_EQ( *valueB, *storage.Get( 1 ) );
        EXPECT_EQ( nullptr, storage.Get( 2 ) );
        EXPECT_EQ( *valueD, *storage.Get( 3 ) );
        EXPECT_EQ( *valueE, *storage.Get( 4 ) );
        EXPECT_EQ( *valueF, *storage.Get( 5 ) );

        EXPECT_EQ( *valueA2, *storage.Get( 6 ) );
        EXPECT_EQ( *valueB2, *storage.Get( 7 ) );
        EXPECT_EQ( *valueC2, *storage.Get( 8 ) );
        EXPECT_EQ( *valueD2, *storage.Get( 9 ) );
        EXPECT_EQ( *valueE2, *storage.Get( 10 ) );
        EXPECT_EQ( *valueF2, *storage.Get( 11 ) );

        std::vector< U32 > names;
        names.push_back( 1 );
        names.push_back( 3 );
        names.push_back( 4 );
        names.push_back( 5 );


        std::vector< U32 > names2;
        names2.push_back( 6 );
        names2.push_back( 7 );
        names2.push_back( 8 );
        names2.push_back( 9 );
        names2.push_back( 10 );
        names2.push_back( 11 );

        std::vector< U32 > result = storage.GetNames( 0 );
        std::vector< U32 > result2 = storage.GetNames( 1u );

        EXPECT_EQ( 4, result.size() );
        EXPECT_EQ( 6, result2.size() );

        EXPECT_TRUE( std::is_permutation( names.begin(), names.end(), result.begin() ) );
        EXPECT_TRUE( std::is_permutation( names2.begin(), names2.end(), result2.begin() ) );
    }

    ENGINE_TEST( NamespaceNamedStorage, RemoveObjectsByAddinNamespace2 )
    {
        NamespaceNamedStorage< U32, U32 > storage;

        U32 *valueA = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB = new U32( ::Test::GenerateRandomU32() );

        U32 *valueA2 = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB2 = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( valueA, 0, Namespace( 0, 0 ) ) );
        EXPECT_TRUE( storage.Add( valueB, 2, Namespace( 0, 0 ) ) );

        EXPECT_TRUE( storage.Add( valueA2, 6, Namespace( 1, 0 ) ) );
        EXPECT_TRUE( storage.Add( valueB2, 8, Namespace( 1, 0 ) ) );

        storage.Clear( Namespace( 0, 0 ) );

        EXPECT_FALSE( storage.Has( 0 ) );
        EXPECT_FALSE( storage.Has( 2 ) );

        EXPECT_TRUE( storage.Has( 6 ) );
        EXPECT_TRUE( storage.Has( 8 ) );

        EXPECT_EQ( nullptr, storage.Get( 0 ) );
        EXPECT_EQ( nullptr, storage.Get( 2 ) );

        EXPECT_EQ( *valueA2, *storage.Get( 6 ) );
        EXPECT_EQ( *valueB2, *storage.Get( 8 ) );
    }

    ENGINE_TEST( NamespaceNamedStorage, RemoveObjectsByPluginNamespace2 )
    {
        NamespaceNamedStorage< U32, U32 > storage;

        U32 *valueA = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB = new U32( ::Test::GenerateRandomU32() );

        U32 *valueA2 = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB2 = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( valueA, 0, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB, 2, Namespace( 0, 1 ) ) );

        EXPECT_TRUE( storage.Add( valueA2, 6, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB2, 8, Namespace( 1, 1 ) ) );

        storage.Clear( Namespace( 1, 1 ) );

        EXPECT_TRUE( storage.Has( 0 ) );
        EXPECT_TRUE( storage.Has( 2 ) );

        EXPECT_FALSE( storage.Has( 6 ) );
        EXPECT_FALSE( storage.Has( 8 ) );

        EXPECT_EQ( *valueA, *storage.Get( 0 ) );
        EXPECT_EQ( *valueB, *storage.Get( 2 ) );

        EXPECT_EQ( nullptr, storage.Get( 6 ) );
        EXPECT_EQ( nullptr, storage.Get( 8 ) );
    }

    ENGINE_TEST( NamespaceNamedStorage, RemoveObjectsByPluginNamespace3 )
    {
        NamespaceNamedStorage< U32, U32 > storage;

        U32 *valueA = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB = new U32( ::Test::GenerateRandomU32() );

        U32 *valueA2 = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB2 = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( valueA, 0, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB, 2, Namespace( 0, 1 ) ) );

        EXPECT_TRUE( storage.Add( valueA2, 6, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB2, 8, Namespace( 1, 1 ) ) );

        storage.Clear( Namespace( 2, 1 ) );

        EXPECT_TRUE( storage.Has( 0 ) );
        EXPECT_TRUE( storage.Has( 2 ) );

        EXPECT_TRUE( storage.Has( 6 ) );
        EXPECT_TRUE( storage.Has( 8 ) );

        EXPECT_EQ( *valueA, *storage.Get( 0 ) );
        EXPECT_EQ( *valueB, *storage.Get( 2 ) );

        EXPECT_EQ( *valueA2, *storage.Get( 6 ) );
        EXPECT_EQ( *valueB2, *storage.Get( 8 ) );
    }

    ENGINE_TEST( NamespaceNamedStorage, RemoveObjectsByPluginNamespace4 )
    {
        NamespaceNamedStorage< U32, U32 > storage;

        U32 *valueA = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB = new U32( ::Test::GenerateRandomU32() );

        U32 *valueA2 = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB2 = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( valueA, 0, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB, 2, Namespace( 0, 1 ) ) );

        EXPECT_TRUE( storage.Add( valueA2, 6, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB2, 8, Namespace( 1, 1 ) ) );

        storage.Clear( Namespace( 0, 2 ) );

        EXPECT_TRUE( storage.Has( 0 ) );
        EXPECT_TRUE( storage.Has( 2 ) );

        EXPECT_TRUE( storage.Has( 6 ) );
        EXPECT_TRUE( storage.Has( 8 ) );

        EXPECT_EQ( *valueA, *storage.Get( 0 ) );
        EXPECT_EQ( *valueB, *storage.Get( 2 ) );

        EXPECT_EQ( *valueA2, *storage.Get( 6 ) );
        EXPECT_EQ( *valueB2, *storage.Get( 8 ) );
    }

    ENGINE_TEST( NamespaceNamedStorage, RemoveObjectsByPluginNamespace5 )
    {
        NamespaceNamedStorage< U32, U32 > storage;

        U32 *valueA = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB = new U32( ::Test::GenerateRandomU32() );

        U32 *valueA2 = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB2 = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( valueA, 0, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB, 2, Namespace( 0, 1 ) ) );

        EXPECT_TRUE( storage.Add( valueA2, 6, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB2, 8, Namespace( 1, 1 ) ) );

        storage.Clear( Namespace( 2, 0 ) );

        EXPECT_TRUE( storage.Has( 0 ) );
        EXPECT_TRUE( storage.Has( 2 ) );

        EXPECT_TRUE( storage.Has( 6 ) );
        EXPECT_TRUE( storage.Has( 8 ) );

        EXPECT_EQ( *valueA, *storage.Get( 0 ) );
        EXPECT_EQ( *valueB, *storage.Get( 2 ) );

        EXPECT_EQ( *valueA2, *storage.Get( 6 ) );
        EXPECT_EQ( *valueB2, *storage.Get( 8 ) );
    }

    ENGINE_TEST( NamespaceNamedStorage, Remove )
    {
        NamespaceNamedStorage< U32, U32 > storage;

        U32 *value = new U32( 4 );

        EXPECT_TRUE( storage.Add( value, 0 ) );

        storage.Remove( 0 );
        EXPECT_FALSE( storage.Has( 0 ) );
    }

    ENGINE_TEST( NamespaceNamedStorage, Remove2 )
    {
        NamespaceNamedStorage< U32, U32 > storage;

        U32 *value = new U32( 4 );
        U32 *value2 = new U32( 0 );

        EXPECT_TRUE( storage.Add( value, 0, Namespace( 2, 1 ) ) );
        EXPECT_TRUE( storage.Add( value2, 1, Namespace( 2, 2 ) ) );

        storage.Remove( 0 );
        EXPECT_FALSE( storage.Has( 0 ) );
    }

    ENGINE_TEST( NamespaceNamedStorage, RemoveNotExists )
    {
        NamespaceNamedStorage< U32, U32 > storage;

        U32 *value = new U32( 4 );

        EXPECT_TRUE( storage.Add( value, 0 ) );

        storage.Remove( 1 );
        EXPECT_TRUE( storage.Has( 0 ) );
    }

    ENGINE_TEST( NamespaceNamedStorage, Has )
    {
        NamespaceNamedStorage< U32, U32 > storage;

        U32 *value = new U32( 4 );

        EXPECT_TRUE( storage.Add( value, 0 ) );

        EXPECT_TRUE( storage.Has( 0 ) );
    }

    ENGINE_TEST( NamespaceNamedStorage, HasNamePlugin )
    {
        NamespaceNamedStorage< U32, U32 > storage;

        U32 *value = new U32( 4 );

        EXPECT_TRUE( storage.Add( value, 0, 1u ) );

        EXPECT_TRUE( storage.Has( 0 ) );
    }

    ENGINE_TEST( NamespaceNamedStorage, HasNameAddin )
    {
        NamespaceNamedStorage< U32, U32 > storage;

        U32 *value = new U32( 4 );

        EXPECT_TRUE( storage.Add( value, 0, Namespace( 0, 1 ) ) );

        EXPECT_TRUE( storage.Has( 0 ) );
    }

    ENGINE_TEST( NamespaceNamedStorage, HasNameNotExisting )
    {
        NamespaceNamedStorage< U32, U32 > storage;

        EXPECT_FALSE( storage.Has( 0 ) );
    }

    ENGINE_TEST( NamespaceNamedStorage, GetNames )
    {
        NamespaceNamedStorage< U32, U32 > storage;

        U32 *value = new U32( 4 );

        EXPECT_TRUE( storage.Add( value, 0 ) );

        std::vector< U32 > names = storage.GetNames( 0u );

        EXPECT_EQ( 1, names.size() );

        EXPECT_EQ( 0, names[0] );
    }

    ENGINE_TEST( NamespaceNamedStorage, GetNamesNotExists )
    {
        NamespaceNamedStorage< U32, U32 > storage;

        U32 *value = new U32( 4 );

        EXPECT_TRUE( storage.Add( value, 0 ) );

        std::vector< U32 > names = storage.GetNames( 1u );

        EXPECT_EQ( 0, names.size() );
    }

    ENGINE_TEST( NamespaceNamedStorage, GetNamesNotExists2 )
    {
        NamespaceNamedStorage< U32, U32 > storage;

        U32 *value = new U32( 4 );

        EXPECT_TRUE( storage.Add( value, 0 ) );

        std::vector< U32 > names = storage.GetNames( Namespace( 0, 1 ) );

        EXPECT_EQ( 0, names.size() );
    }

    ENGINE_TEST( NamespaceNamedStorage, GetNames2 )
    {
        NamespaceNamedStorage< U32, U32 > storage;

        U32 *value = new U32( 4 );

        EXPECT_TRUE( storage.Add( value, 0, Namespace( 0, 1 ) ) );

        std::vector< U32 > names = storage.GetNames( Namespace( 0, 1 ) );

        EXPECT_EQ( 1, names.size() );

        EXPECT_EQ( 0, names[0] );
    }


    ENGINE_TEST( NamespaceNamedStorage, GetNamesMultiple )
    {
        NamespaceNamedStorage< U32, U32 > storage;

        U32 *valueA = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB = new U32( ::Test::GenerateRandomU32() );

        U32 *valueC = new U32( ::Test::GenerateRandomU32() );
        U32 *valueD = new U32( ::Test::GenerateRandomU32() );

        U32 *valueE = new U32( ::Test::GenerateRandomU32() );
        U32 *valueF = new U32( ::Test::GenerateRandomU32() );

        U32 *valueA2 = new U32( ::Test::GenerateRandomU32() );
        U32 *valueB2 = new U32( ::Test::GenerateRandomU32() );

        U32 *valueC2 = new U32( ::Test::GenerateRandomU32() );
        U32 *valueD2 = new U32( ::Test::GenerateRandomU32() );

        U32 *valueE2 = new U32( ::Test::GenerateRandomU32() );
        U32 *valueF2 = new U32( ::Test::GenerateRandomU32() );

        EXPECT_TRUE( storage.Add( valueA, 0, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB, 1, Namespace( 0, 2 ) ) );
        EXPECT_TRUE( storage.Add( valueC, 2, Namespace( 0, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueD, 3, Namespace( 0, 2 ) ) );
        EXPECT_TRUE( storage.Add( valueE, 4, Namespace( 0, 0 ) ) );
        EXPECT_TRUE( storage.Add( valueF, 5, Namespace( 0, 0 ) ) );

        EXPECT_TRUE( storage.Add( valueA2, 6, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueB2, 7, Namespace( 1, 2 ) ) );
        EXPECT_TRUE( storage.Add( valueC2, 8, Namespace( 1, 1 ) ) );
        EXPECT_TRUE( storage.Add( valueD2, 9, Namespace( 1, 2 ) ) );
        EXPECT_TRUE( storage.Add( valueE2, 10, Namespace( 1, 0 ) ) );
        EXPECT_TRUE( storage.Add( valueF2, 11, Namespace( 1, 0 ) ) );

        std::vector< U32 > names;
        names.push_back( 0 );
        names.push_back( 1 );
        names.push_back( 2 );
        names.push_back( 3 );
        names.push_back( 4 );
        names.push_back( 5 );

        std::vector< U32 > result = storage.GetNames( 0 );

        EXPECT_EQ( 6, result.size() );

        EXPECT_TRUE( std::is_permutation( names.begin(), names.end(), result.begin() ) );
    }

}