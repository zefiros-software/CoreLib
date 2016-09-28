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
#ifndef __ENGINE_HASH_H__
#define __ENGINE_HASH_H__

#include "common/types.h"

/// @addtogroup docAlgorithm
/// @{

/// @weakgroup docAlgorithm_Hashes
/// @{

/**
 * Holds a collection of hash algorithms.
 */

namespace Hash
{
    /**
     * Hashes the input string, using the Fowler-Noll-Vo hash algorithm, version 1a. The algorithm is iterative and
     * relies on multiplication and XOR so, the hash is sensitive to NULL data. Because we insert strings (NULL
     * terminated) in this function, we negate this effect. The hash is calculated using the following algorithm:
     * @code
     *      <hash> = 2166136261u
     *      for each <byte> in <data>
     *          <hash> = <hash> XOR <byte>
     *          <hash> = <hash> * the prime 16777619u
     *      endfor
     * @endcode.
     *
     * @param   str The string to calculate the hash from.
     *
     * @return The hash.
     *
     * @post
     * * Hash::Fnv1a( "<string>" ) == Hash::Fnv1a( "<string>" )
     * * Hash::Fnv1a( "<string>" ) @f$\not\approx@f$ Hash::Fnv1a( "<sting2>" ),
     *   where "<string>" @f$\neq@f$ "<string2>"
     *
     * @details
     * @examples
     *
     * * Different strings usually give other hashes:
     *   @snippet testHash.cpp Fnva1NotEqual
     *
     * * Same string always gives the same result (deterministic):
     *   @snippet testHash.cpp Fnva1Deterministic
     *
     * @note
     * For more information read the wikipedia page:
     *    http://en.wikipedia.org/wiki/Fowler%E2%80%93Noll%E2%80%93Vo_hash_function.
     */

    U32 Fnv1a( const char *str ) noexcept;

    /**
     * Combine two hashes to a new hash.
     *
     * @param   seed The seed.
     * @param   val  The value.
     *
     * @return The combined seed.
     *
     * @details
     * @examples
     *
     * @snippet testUtil.cpp HashCombine
     */
    size_t Combine( size_t seed, size_t val ) noexcept;
}

/// @}

/// @}

#endif