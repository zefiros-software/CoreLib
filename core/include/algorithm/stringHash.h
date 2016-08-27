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
#ifndef __ENGINE_STRINGHASH_H__
#define __ENGINE_STRINGHASH_H__

#include "common/utilClasses.h"  // for NonAssignable
#include "algorithm/hash.h"      // for Fnv1a
#include "common/types.h"        // for U32

/// @addtogroup docAlgorithm Algorithm
/// @{

/// @weakgroup docAlgorithm_Hashes Hashes
/// @{

/**
 * A very neat implementation of a string hash that will be computed (Most likely) on compile time, by utilizing
 * forced inlines, and compiler optimisation. The hashes are calculated with the Hash::Fnv1a hash algorithm.
 * (Standalone implementation is used)
 *
 * @details
 * @examples
 * @snippet testStringHash.cpp StringHash
 *
 * @note
 * Based on (a rather good read): http://www.altdevblogaday.com/2011/10/27/quasi-compile-time-string-hashing/.
 * (as of the time of writing it seems altdevblogaday.com is down, the article is available via the
 * [webarchive](http://web.archive.org/web/20131106173334/http://altdevblogaday.com/2011/10/27/quasi-compile-time-string-hashing/)
 * however.)
 *
 * @sa NonAssignable
 * @sa Hash::Fnv1a
 */

class StringHash
    : NonAssignable< StringHash >
{
public:

    /// @name Dark Arts Utility - Thou Shan't Passeth
    /// @{

    /**
     * Wraps c-string in a struct so we can convert them later on implicitly to the char array we
     * can operate on.
     */

    struct ConstCharPointer
    {
        const char *wrappedString;

        inline ConstCharPointer( const char *str ) noexcept
            : wrappedString( str )
        {
        }
    };

    /// @}

    /// @name Construction
    /// @{

    /**
     * Making use of implicit constructors, we can construct this class using solely c-strings. The ConstCharPointer
     * struct will convert it implicitly to the correct char array format, we can run our template functions on. We
     * calculate the hash value on construction.
     *
     * @param   str The c-string to hash.
     *
     * @post StringHash( ConstCharPointer( (const char *)"<string>") ) ).GetHash() == Hash::Fnv1a( "<string>" )
     */

    __forceinline StringHash( ConstCharPointer str ) noexcept
        : mHash( Hash::Fnv1a( str.wrappedString ) )
    {
    }

    /**
     * Calculate the hash of a char array.
     *
     * @tparam  U32 N   The length of the char array.
     * @param   str     The char array.
     *
     * @post template< U32 N >
     *       StringHash( const char( & "<string>" )[N] ).GetHash() == Hash::Fnv1a( "<string>" )
     */

    template< U32 N >
    __forceinline StringHash( const char( &str )[ N ] ) noexcept
        : mHash( Fnv1a< N, N >::Hash( str ) )
    {
    }

    /**
     * Construct the class using an already calculated (or not), hash value.
     *
     * @param   hash The hash.
     *
     * @post StringHash( U32 hash ).GetHash() == hash.
     */

    StringHash( U32 hash ) noexcept;

    /// @}

    /// @name Hash
    /// @{

    /**
     * Gets the hash value.
     *
     * @return The hash.
     */

    U32 GetHash() const;

    /// @}

private:

    // Immutable hash value. The hash will never change so the compiler may also know that.
    const U32 mHash;

    /// @name Template Magic - It is almost functional
    /// @{

    /**
     * Recursive implementation of the Hash::Fnv1a hash algorithm, using templates we can scan the whole char array,
     * using only inlined functions.
     *
     * @tparam  N Length of the char array.
     * @tparam  I Current index of the char array.
     *
     * @sa Hash::Fnv1a
     */

    template < U32 N, U32 I >
    struct Fnv1a
    {
        static __forceinline U32 Hash( const char( &str )[ N ] )
        {
            return ( Fnv1a < N, I - 1 >::Hash( str ) ^ str[ I - 1 ] ) * 16777619u;
        }
    };

    /**
     * End condition of our inlined template Fnv1a hash algorithm.
     *
     * @tparam  N Total length of the char array.
     *
     * @sa Hash::Fnv1a
     */

    template < U32 N >
    struct Fnv1a< N, 1 >
    {
        static __forceinline U32 Hash( const char( &str )[ N ] )
        {
            return ( 2166136261u ^ str[ 0 ] ) * 16777619u;
        }
    };

    /// @}
};

/// @}

/// @}

#endif