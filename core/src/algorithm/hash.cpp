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

#include "algorithm/hash.h"

#include <string.h>
#include <cassert>

namespace Hash
{
    U32 Fnv1a( const char *str ) noexcept
    {
        assert( str );
        const size_t length = strlen( str ) + 1;

        // The FNV offset basis
        U32 hash = 2166136261u;

        for ( size_t i = 0; i < length; ++i )
        {
            hash ^= *str++;

            // multiply with a prime number
            hash *= 16777619u; //-V127
        }

        return hash;
    }

    size_t Combine( size_t seed, size_t val ) noexcept
    {
        //rotating hash combine!
        return seed ^ ( val + 0x9e3779b9 + ( seed << 6 ) + ( seed >> 2 ) );
    }

}
