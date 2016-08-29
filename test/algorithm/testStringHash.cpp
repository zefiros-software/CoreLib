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

#include "algorithm/stringHash.h"

#include "engineTest.h"

namespace
{

    ENGINE_TEST( StringHash, CharArrayCstring )
    {
        char testArray[] = "Single";
        EXPECT_EQ( StringHash( testArray ).GetHash(), StringHash( "Single" ).GetHash() );
    }

    ENGINE_TEST( StringHash, NotEqual )
    {
        EXPECT_NE( StringHash( "Zefiros Engine" ).GetHash(), StringHash( "Test" ).GetHash() );
    }

    ENGINE_TEST( StringHash, Deterministic )
    {
        EXPECT_EQ( StringHash( "Zefiros Engine" ).GetHash(), StringHash( "Zefiros Engine" ).GetHash() );
    }

    ENGINE_TEST( StringHash, Algorithm )
    {
        //! [StringHash]
        EXPECT_EQ( Hash::Fnv1a( "Zefiros Engine" ), StringHash( "Zefiros Engine" ).GetHash() );
        //! [StringHash]
    }

    ENGINE_TEST( StringHash, Cstring )
    {
        const std::string string = ::Test::GenerateRandomString();
        EXPECT_EQ( Hash::Fnv1a( string.c_str() ), StringHash( string.c_str() ).GetHash() );
    }

    ENGINE_TEST( StringHash, PreCalculated )
    {
        const U32 num = ::Test::GenerateRandomU32();
        EXPECT_EQ( num, StringHash( num ).GetHash() );
    }

    ENGINE_TEST( StringHash, NULL )
    {
        EXPECT_NE( ( U32 )0, StringHash( "" ).GetHash() );
    }

    ENGINE_TEST( StringHash, RandomString )
    {
        const std::string string = ::Test::GenerateRandomString();
        EXPECT_EQ( StringHash( string.c_str() ).GetHash(), StringHash( string.c_str() ).GetHash() );
    }

}
