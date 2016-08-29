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
#ifndef __UTIL_H__
#define __UTIL_H__

#include "math/scalar/mathf.h"

#include "preproc/config.h"

#include "common/types.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>

/// @addtogroup docCommon
/// @{


/// @addtogroup docCommon_Utility
/// @{


/// @addtogroup docCommon_Utility_Macros
/// @{

/// @name Preprocessing
/// @{

/// @cond SKIP

/// Evil msvc bug fix hackery

#define ___EXPAND( x ) x

/// Helper macro function

#define ___CONCAT( a, b ) a ## b

/**
 * A macro that concatenate two other macro names, and expands the newly formed macro.
 *
 * @param   a
 * The first macro.
 * @param   b
 * The second macro.
 *
 * @pre
 * The concatenated version of the macros is defined.
 *
 * @post
 * The concatenated version of the macro is expanded.
 *
 * @details
 * @examples
 *
 * @snippet testUtil.cpp CONCAT
 */

#define CONCAT( a, b ) ___CONCAT( a, b )

/// @cond SKIP

/// Helper macro function

#define ___VA_SIZE( _1, _2, _3, _4, _5, _6, __count, ... ) __count

/// @endcond SKIP

/**
 * A macro that counts the amount of arguments given in a variable argument macro.
 *
 * @param    ...
 * The arguments.
 *
 * @pre
 * The argument list consists between 1 and 6 arguments. No argument will be seen as 1
 * argument, and more than 6 arguments won't be counted.
 *
 * @details
 * @examples
 *
 * @snippet testUtil.cpp VA_SIZE
 */

#define VA_SIZE( ... ) ___EXPAND( ___VA_SIZE( __VA_ARGS__, 6, 5, 4, 3, 2, 1 ) )

/**
 * A macro that allows us to overload macros on a given amount of arguments.
 *
 * @param   macroName
 * The name of the macro we are going to overload.
 * @param   ...
 * The argument list.
 *
 * @details
 * @examples
 *
 * @snippet testUtil.cpp VA_SELECT
 *
 * @see VA_SIZE
 */

//#define VA_SELECT( macroName, ... ) CONCAT( macroName, CONCAT( _, VA_SIZE( __VA_ARGS__ ) ) )( __VA_ARGS__ )

/// @}

/// @name Bitflags
/// @{

/**
 * A macro that checks if a single bit flag is set on a flag holder.
 *
 * @param   flag
 * The flags.
 * @param   flagValue
 * The flag value where the flag will be tested for.
 *
 * @pre
 * The flag value is a single, (x)or combination, of a power of two. Thus, @f$ \exists n \in
 * \mathbb{N}_0, 2^n = z@f$, where @f$z@f$ is the flag value. (XOR a combination of these)
 * When not met, the value is either '0x00' and returns true, or otherwise it returns false.
 *
 * @details
 * @examples
 *
 * * Returns true when one flag is set.
 *   @snippet testUtil.cpp IS_SET Simple
 * * Returns true when one of multiple flags is set.
 *   @snippet testUtil.cpp IS_SET Multiple
 * * Returns true when multiple bit flags are set.
 *   @snippet testUtil.cpp IS_SET HasMultipleSet
 * * Returns false when not all* of the multiple bit flags are set.
 *   @snippet testUtil.cpp IS_SET HasMultipleSetNot
 * * Returns false when the flag is '0x00'.
 *   @snippet testUtil.cpp IS_SET Null
 * * Returns true when we check whether '0x00' is set.
 *   @snippet testUtil.cpp IS_SET NullSet
 * * Returns false when a bit flag is not set.
 *   @snippet testUtil.cpp IS_SET NotSet
 */

#define IS_SET( flag, flagValue ) ( ( (flag) & (flagValue) ) == (flagValue) )

/**
 * A macro that checks whether one of the given bit flags is set.
 *
 * @param   flag
 * The flag.
 * @param   flagvalues
 * The flag value where the flag will be tested for.
 *
 * @pre
 * The flag value is a single, (x)or combination, of a power of two. Thus, @f$ \exists n \in
 * \mathbb{N}_0, 2^n = z@f$, where @f$z@f$ is the flag value. (XOR a combination of these)
 * When not met, the value is either '0x00' and returns true, or otherwise it returns false.
 *
 * @details
 * @examples
 *
 * * Returns true when a single bit flag is set.
 *   @snippet testUtil.cpp HAS_ANY_SET Simple
 * * Returns true when one of multiple flags is set.
 *   @snippet testUtil.cpp HAS_ANY_SET Multiple
 * * Returns true when multiple bit flags are set.
 *   @snippet testUtil.cpp HAS_ANY_SET HasMultipleSet
 * * Returns true wen one of the given bit flags is set.
 *   @snippet testUtil.cpp HAS_ANY_SET HasOneOfSet
 * * Returns false when the flag is '0x00'.
 *   @snippet testUtil.cpp HAS_ANY_SET Null
 * * Returns false when we check whether '0x00' is set.
 *   @snippet testUtil.cpp HAS_ANY_SET NullSet
 * * Returns false when none of the flags is set.
 *   @snippet testUtil.cpp HAS_ANY_SET NotSet
 */

#define HAS_ANY_SET( flag, flagvalues ) ( ( (flag) & (flagvalues) ) != 0 )

/// @}

/// @name Freeing Memory
/// @{

/**
 * A macro that defines checks if a pointer exists before releasing it and sets it to nullptr.
 *
 * @param   ptr The pointer.
 *
 * @pre The pointer type will free its memory when the "Release()" function is called.
 *
 * @post The pointer is set to nullptr and its memory is freed.
 *
 * @details
 * @examples
 *
 * Given the following class:
 * @snippet testUtil.cpp SAFE_RELEASE Example class
 *
 * * The macro will call the "Release()" function.
 *   @snippet testUtil.cpp SAFE_RELEASE
 * * The macro accepts nullptr's.
 *   @snippet testUtil.cpp SAFE_RELEASE nullptr
 */

#define SAFE_RELEASE( ptr ) if( ptr ) { ptr->Release(); ptr = nullptr; }

/**
 * A macro that releases the pointer if it exists, deletes it and sets it to nullptr.
 *
 * @param   ptr The pointer to release and delete.
 *
 * @pre
 * The pointer type will free its memory when either the "Release()" or destructor function
 * is called.
 *
 * @post The pointer is set to nullptr and its memory is freed.
 *
 * @details
 * @examples
 *
 * Given the following class:
 * @snippet testUtil.cpp SAFE_RELEASEDELETE Example class
 *
 * * The macro will call the "Release()" function and the destructor.
 *   @snippet testUtil.cpp SAFE_RELEASEDELETE
 * * The macro accepts nullptr's.
 *   @snippet testUtil.cpp SAFE_RELEASEDELETE nullptr
 */

#define SAFE_RELEASE_DELETE( ptr )  if( ptr ) { ptr->Release(); delete ptr; ptr = nullptr; }

/**
 * A macro that deletes a pointer and sets it to nullptr.
 *
 * @param   ptr The pointer.
 *
 * @pre The pointer type will free its memory when the destructor is called.
 *
 * @post The pointer is set to nullptr and its memory is freed.
 *
 * @details
 * @examples
 *
 * * The macro frees the memory.
 *   @snippet testUtil.cpp SAFE_DELETE
 * * The macro accepts nullptr's.
 *   @snippet testUtil.cpp SAFE_DELETE nullptr
 */

#define SAFE_DELETE( ptr ) { delete ptr; ptr = nullptr; }

/**
 * A macro that deletes an array pointer and sets it to nullptr.
 *
 * @param   ptr The pointer.
 *
 * @pre The array type will free its memory when the destructor is called.
 *
 * @post The array pointer is set to nullptr and its memory is freed.
 *
 * @details
 * @examples
 *
 * * The macro frees the memory.
 *   @snippet testUtil.cpp SAFE_ARRAY_DELETE
 * * The macro accepts nullptr's.
 *   @snippet testUtil.cpp SAFE_ARRAY_DELETE nullptr
 */

#define SAFE_ARRAY_DELETE( ptr ) { delete[] ptr; ptr = nullptr; }

/**
 * A macro that defines size of the given kilobytes in bytes.
 *
 * @param   x The amount of kilobytes.
 *
 * @details
 * @examples
 *
 * @snippet testUtil.cpp SIZE_OF_KB
 *
 * @note Technically this is a KiB
 */

#define SIZE_OF_KB(x) ((x) * ( 1 << 10 ))

/**
 * A macro that defines size of the given megabytes in bytes.
 *
 * @param   x The amount of bytes.
 *
 * @details
 * @examples
 *
 * @snippet testUtil.cpp SIZE_OF_MB
 *
 * @note Technically this is a MiB
 */

#define SIZE_OF_MB(x) ((x) * ( 1 << 20 ))

#if IS_DEBUG
#   define ASSERT(condition, message)                                       \
 do {if (! (condition)) {                                                   \
    std::cerr << __FILE__ << "(" << __LINE__ << "):\n" <<                   \
    "Assertion `" << #condition << "` failed!\n" << (message) << std::endl; \
    fflush(stderr);std::exit(-1);                                           \
}} while(false)
#else
#   define ASSERT(condition, message)
#endif

/// @}


/// @}

/**
 * A collection of utility functions.
 */
namespace Util
{

    /// @name Hashes
    /// @{

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

    size_t HashCombine( size_t seed, size_t val );

    /// @}

    /**
     * Get the most significant set bit.
     *
     * @param   in The value to get the msb from.
     *
     * @return The most significant set bit.
     *
     * @details
     * @examples
     *
     * @snippet testUtil.cpp HashCombine
     */

    U32 BitExponent( U64 in );

    /**
     * Nearest power of 2 of the given value.
     *
     * @tparam  T Generic type parameter.
     * @param   in The value to get the nearest power of 2 from.
     *
     * @return The nearest power of 2.
     *
     * @details
     * @examples
     *
     * @snippet testUtil.cpp NearestPower2
     */

    template< class tT >
    tT NearestPower2( tT in )
    {
        static_assert( std::is_integral<tT>::value, "The type should be integral to have meaning." );
        return Mathf::Pow< tT >( 2, static_cast< F32 >( BitExponent( in ) ) );
    }


    /// @name Traits
    /// @{

    /**
     * Determines whether the child type can be used when we use the base type as the base class.
     * This holds true when the child class derives from the parent class, or is the same class.
     *
     * @tparam  tChild  Type of the child.
     * @tparam  tParent Type of the base.
     *
     * @details
     * The member value holds true if it is accepted, false otherwise.
     *
     * @details
     * @examples
     *
     * Given the following classes.
     * @snippet testUtil.cpp IsChildParent Example
     *
     * The following will hold.
     * @snippet testUtil.cpp IsChildParent
     */

    template< typename tChild, typename tParent >
    struct IsChildParent
    : std::integral_constant < bool, ( std::is_base_of<tParent, tChild >::value ||
                                       std::is_same< tChild, tParent >::value ) >
      {
      };

    /// @}

};

/// @}

/// @}

#endif