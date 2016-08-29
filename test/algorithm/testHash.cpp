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

#include "algorithm/hash.h"

#include "engineTest.h"

namespace
{

    ENGINE_TEST( Hash, Fnva1NotEqual )
    {
        //! [Fnva1NotEqual]
        EXPECT_NE( Hash::Fnv1a( "Zefiros Engine" ), Hash::Fnv1a( "Test" ) );
        //! [Fnva1NotEqual]
    }

    ENGINE_TEST( Hash, Fnva1Deterministic )
    {
        //! [Fnva1Deterministic]
        EXPECT_EQ( Hash::Fnv1a( "Zefiros Engine" ), Hash::Fnv1a( "Zefiros Engine" ) );
        //! [Fnva1Deterministic]
    }

    ENGINE_TEST( Hash, Fnva1NULL )
    {
        EXPECT_NE( ( U32 )0, Hash::Fnv1a( "" ) );
    }

    ENGINE_TEST( Hash, Fnva1Random )
    {
        const std::string string = ::Test::GenerateRandomString();
        EXPECT_EQ( Hash::Fnv1a( string.c_str() ), Hash::Fnv1a( string.c_str() ) );
    }

}