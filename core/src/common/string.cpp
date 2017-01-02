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

#include "common/string.h"


template<>
std::string String::To( const F32 &value )
{
    return std::to_string( value );
}

template<>
std::string String::To( const F64 &value )
{
    return std::to_string( value );
}

template<>
std::string String::To( const S32 &value )
{
    return std::to_string( value );
}

template<>
std::string String::To( const S64 &value )
{
    return std::to_string( value );
}

template<>
std::string String::To( const U32 &value )
{
    return std::to_string( value );
}

template<>
std::string String::To( const U64 &value )
{
    return std::to_string( value );
}

template<>
F32 String::From( const std::string &str )
{
    return std::stof( str );
}

template<>
F64 String::From( const std::string &str )
{
    return std::stod( str );
}

template<>
S32 String::From( const std::string &str )
{
    return std::stoi( str );
}

template<>
S64 String::From( const std::string &str )
{
    return std::stoll( str );
}

template<>
U32 String::From( const std::string &str )
{
    return std::stoi( str );
}

template<>
U64 String::From( const std::string &str )
{
    return std::stoull( str );
}

std::string String::FromWString( const std::wstring &wstr )
{
    return std::string( wstr.begin(), wstr.end() );
}

std::wstring String::ToWString( const std::string &s )
{
    std::wstring ws;
    ws.assign( s.begin(), s.end() );
    return ws;
}

bool String::IsWhiteSpace( const std::string &str )
{
    std::string trim = str;
    boost::trim( trim );
    return trim == "";
}

std::vector< std::string > String::Split( const std::string &str, char sep, bool trim /*= false */ ) noexcept
{
    std::vector< std::string > output;

    std::string::size_type pos, prevPos = 0;

    //loop over all delimiter positions
    while ( ( pos = str.find( sep, prevPos + 1 ) ) != std::string::npos )
    {
        std::string token = str.substr( prevPos, pos - prevPos );

        if ( trim )
        {
            token = Trim( token );
        }

        output.push_back( token );

        prevPos = pos + 1;
    }

    //wont catch the last bit
    std::string token = str.substr( prevPos, str.size() );

    if ( trim )
    {
        token = Trim( token );
    }

    output.push_back( token );

    return output;
}

std::string String::Trim( const std::string &str ) noexcept
{
    std::string nstr = str;
    boost::algorithm::trim( nstr );
    return nstr;
}

std::string String::Replace( const std::string &str, const std::string &search, const std::string &format ) noexcept
{
    std::string nstr = str;
    boost::algorithm::replace_all( nstr, search, format );
    return nstr;
}

std::string String::Capitalise( const std::string &str ) noexcept
{
    std::string result( str );
    result[0] = static_cast<char>( toupper( result[0] ) );
    return result;
}

std::string String::Repeat( const std::string &str, size_t n ) noexcept
{
    std::ostringstream ss;

    for ( U32 i = 0; i < n; ++i )
    {
        ss << str;
    }

    return ss.str();
}

std::wstring String::Repeat( const std::wstring &str, size_t n ) noexcept
{
    std::basic_ostringstream<wchar_t> ss;

    for ( U32 i = 0; i < n; ++i )
    {
        ss << str;
    }

    return ss.str();
}
