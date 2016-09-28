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
#ifndef __ENGINE_STRING_H__
#define __ENGINE_STRING_H__

#include "common/types.h"

#include "external/stringAlgorithm.h"

#include <sstream>
#include <vector>

namespace String
{
    /// @name String converters
    /// @{

    /**
     * Convert this object into a string representation.
     *
     * @tparam  tT Type to convert to string.
     * @param   value The value to convert.
     *
     * @return The value as a string.
     *
     * @details
     * @examples
     *
     * For example, U8 will be interpreted as an ascii character.
     * @snippet testUtil.cpp ToStringU8
     */

    template< typename tT >
    std::string To( const tT &value )
    {
        std::stringstream ss;
        ss << value;
        return ss.str();
    }

    /**
     * @see ToString
     */

    template<>
    std::string To< F32 >( const F32 &value );

    /**
     * @see ToString
     */

    template<>
    std::string To< F64 >( const F64 &value );

    /**
     * @see ToString
     */

    template<>
    std::string To< S32 >( const S32 &value );

    /**
     * @see ToString
     */

    template<>
    std::string To< S64 >( const S64 &value );

    /**
     * @see ToString
     */

    template<>
    std::string To< U32 >( const U32 &value );

    /**
     * @see ToString
     */

    template<>
    std::string To< U64 >( const U64 &value );

    /**
     * Converts a string to the given type.
     *
     * @tparam  tT  Type convert from string.
     * @param   str The string to convert to the given type.
     *
     * @return The to the type converted string.
     *
     * @details
     * @examples
     *
     * For example, S8 will be interpreted as an ascii character.
     * @snippet testUtil.cpp FromStringS8
     */

    template <typename tT>
    tT From( const std::string &str )
    {
        std::istringstream ss( str );
        tT result;
        return ( ss >> result ) ? result : 0;
    }

    /**
     * @see FromString
     */

    template<>
    F32 From< F32 >( const std::string &str );

    /**
     * @see FromString
     */

    template<>
    F64 From< F64 >( const std::string &str );

    /**
     * @see FromString
     */

    template<>
    S32 From< S32 >( const std::string &str );

    /**
     * @see FromString
     */

    template<>
    S64 From< S64 >( const std::string &str );

    /**
     * @see FromString
     */

    template<>
    U32 From< U32 >( const std::string &str );

    /**
     * @see FromString
     */

    template<>
    U64 From< U64 >( const std::string &str );

    /**
     * Converts a std::wstring to a std::string.
     *
     * @param   wstr The wstring to convert.
     *
     * @return The string.
     *
     * @details
     * @examples
     *
     * @snippet testUtil.cpp WStringToString
     */

    std::string FromWString( const std::wstring &wstr );

    /**
     * Converts a std::string to a std::wstring.
     *
     * @param   str The string to convert.
     *
     * @return The wstring.
     *
     * @details
     * @examples
     *
     * @snippet testUtil.cpp StringToWString
     */

    std::wstring ToWString( const std::string &str );

    /// @}

    /// @name String utilities
    /// @{

    /**
     * Query if 'string' is white space.
     *
     * @param   str The string.
     *
     * @return true if white space, false if not.
     *
     * @post All whitespace, meaning all '\\t', '\\n', '\\r' and ' ', is considered as whitespace.
     *
     * @details
     * @examples
     *
     * @snippet testUtil.cpp IsWhiteSpace
     */

    bool IsWhiteSpace( const std::string &str );

    /**
     * Trims the given string, by removing all trailing and heading whitespace.
     *
     * @param   str The string.
     *
     * @return The trimmed string.
     *
     * @post
     * All whitespace, meaning all '\\t', '\\n', '\\r' and ' ', is removed from the beginning
     * and the end of the string.
     *
     * @details
     * @examples
     *
     * @snippet testUtil.cpp Trim
     */

    std::string Trim( const std::string &str ) noexcept;

    /**
     * Split a string into parts seperated by the given seperator.
     *
     * @param   str  The string.
     * @param   sep  The separator.
     * @param   trim True to trim whitespace between the parts.
     *
     * @return  The parts of the string.
     *
     * @post
     * for each @f$ s_i \in \hat{S} @f$:
     *      * return @f$\{s_1, ..., s_n\}@f$
     *      * if @f$trim == true@f$: @f$ s_i = @f$ Util::Trim(@f$ s_i @f$)
     *
     * @details
     * @examples
     *
     * @snippet testUtil.cpp StringSplit
     *
     * We can also trim the string parts:
     * @snippet testUtil.cpp StringSplit Trimmed
     */

    std::vector< std::string > Split( const std::string &str, char sep, bool trim = false ) noexcept;

    /**
     * Replaces all occurrences of a certain string, in a larger string by an other given string.
     *
     * @param   str    The string to replace the strings in.
     * @param   search The string we search for to replace.
     * @param   format The string we will put in place.
     *
     * @return A string with all occurrences replaced.
     *
     * @post All occurences of the 'search' string are replaced with the 'format' string.
     *
     * @details
     * @examples
     *
     * @snippet testUtil.cpp Replace
     */

    std::string Replace( const std::string &str, const std::string &search, const std::string &format ) noexcept;

    /**
     * Capitalises the given string, by uppercasing the first letter.
     *
     * @param   str The string.
     *
     * @return The capitalised string.
     *
     * @post The first character of the string is upper case.
     *
     * @details
     * @examples
     *
     * @snippet testUtil.cpp Capitalise
     */

    std::string Capitalise( const std::string &str ) noexcept;

    /// @}

}

#endif