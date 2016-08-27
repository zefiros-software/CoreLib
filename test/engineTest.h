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

#pragma once
#ifndef __ENGINETEST_H__
#define __ENGINETEST_H__

#include "common/directory.h"
#include "common/util.h"

#include "preproc/overload.h"
#include "preproc/config.h"

#include "external/test.h"

#include <unordered_set>

#if IS_DEBUG
#   define TEST_RANDOM_SEED 42u
#else
#   define TEST_RANDOM_SEED
#endif

#define P( prefix ) CONCAT(PREFIX, prefix)
#define ENGINE_TEST_2( suite, name ) TEST( P( suite ), name )
#define ENGINE_TEST_3( suite, function, name )  TEST( P( suite ), function ## _  ## name )

#define ENGINE_TEST(...) VA_SELECT(ENGINE_TEST_,__VA_ARGS__)

#ifndef ENGINE_SHIPVERSION
#define EXPECT_CONSOLERR( expression ) EXPECT_THROW(expression, std::string)
#else
#define EXPECT_CONSOLERR( expression )
#endif

namespace Test
{
    void EnsureTestDirectories( const std::vector<std::string > &strs );

    std::string GenerateDirectoryName( const std::string &root = "" );

    std::string GenerateRandomString();

    template< U32 N >
    std::vector< std::string > GenerateUniqueStrings()
    {
        std::unordered_set< std::string > set;

        while ( set.size() < N )
        {
            set.insert( GenerateRandomString() );
        }

        return std::vector< std::string >( set.begin(), set.end() );
    }


    S32 GenerateRandomS32();

    template< U32 N >
    std::vector< S32 > GenerateUniqueS32s()
    {
        std::unordered_set< S32 > set;

        while ( set.size() < N )
        {
            set.insert( GenerateRandomS32() );
        }

        return std::vector< S32 >( set.begin(), set.end() );
    }

    U32 GenerateRandomU32();

    template< U32 N >
    std::vector< U32 > GenerateUniqueU32s()
    {
        std::unordered_set< U32 > set;

        while ( set.size() < N )
        {
            set.insert( GenerateRandomS32() );
        }

        return std::vector< U32 >( set.begin(), set.end() );
    }

    F32 GenerateRandomF32();

    template< U32 N >
    std::vector< F32 > GenerateUniqueF32s()
    {
        std::unordered_set< F32 > set;

        while ( set.size() < N )
        {
            set.insert( GenerateRandomS32() );
        }

        return std::vector< F32 >( set.begin(), set.end() );
    }

    bool GenerateRandomBool();

    void GenerateRandomFile( const std::string &path, std::string content = "" );

    void CleanUp( const std::string &path );

}

#endif