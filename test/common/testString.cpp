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

#include "common/string.h"

#include "engineTest.h"

namespace
{
    TEST( String, To, U8 )
    {
        //! [ToU8]
        EXPECT_EQ( "*", String::To<U8>( 42 ) );
        //! [ToU8]
    }

    TEST( String, To, U16 )
    {
        EXPECT_EQ( "42", String::To<U16>( 42 ) );
    }

    TEST( String, To, U32 )
    {
        EXPECT_EQ( "42", String::To<U32>( 42 ) );
    }

    TEST( String, To, U64 )
    {
        EXPECT_EQ( "42", String::To<U64>( 42 ) );
    }

    TEST( String, To, S8 )
    {
        EXPECT_EQ( "*", String::To<S8>( 42 ) );
    }

    TEST( String, To, S16 )
    {
        EXPECT_EQ( "-42", String::To<S16>( -42 ) );
    }

    TEST( String, To, S32 )
    {
        EXPECT_EQ( "-42", String::To<S32>( -42 ) );
    }

    TEST( String, To, S64 )
    {
        EXPECT_EQ( "-42", String::To<S64>( -42 ) );
    }

    TEST( String, To, F32 )
    {
        EXPECT_FLOAT_EQ( 42.0f, String::From<F32>( String::To<F32>( 42.0f ) ) );
    }

    TEST( String, To, F64 )
    {
        EXPECT_DOUBLE_EQ( 42.0, String::From<F64>( String::To<F64>( 42.0 ) ) );
    }

    TEST( String, To, F32Decimal )
    {
        EXPECT_FLOAT_EQ( 42.42f, String::From<F32>( String::To<F32>( 42.42f ) ) );
    }

    TEST( String, To, F64Decimal )
    {
        EXPECT_DOUBLE_EQ( 42.42, String::From<F64>( String::To<F64>( 42.42 ) ) );
    }

    TEST( String, From, U8 )
    {
        //! [From]
        EXPECT_EQ( 42u, String::From<U8>( "*" ) );
        //! [From]
    }

    TEST( String, From, U16 )
    {
        EXPECT_EQ( 42u, String::From<U16>( "42" ) );
    }

    TEST( String, From, U32 )
    {
        EXPECT_EQ( 42u, String::From<U32>( "42" ) );
    }

    TEST( String, From, U64 )
    {
        EXPECT_EQ( 42u, String::From<U64>( "42" ) );
    }

    TEST_3( String, From, S16 )
    {
        EXPECT_EQ( -42, String::From<S16>( "-42" ) );
    }

    TEST( String, From, S32 )
    {
        EXPECT_EQ( -42, String::From<S32>( "-42" ) );
    }

    TEST( String, From, S64 )
    {
        EXPECT_EQ( -42, String::From<S64>( "-42" ) );
    }

    TEST( String, From, F32 )
    {
        EXPECT_FLOAT_EQ( 42, String::From<F32>( "42.0" ) );
    }

    TEST( String, From, F32f )
    {
        EXPECT_FLOAT_EQ( 42, String::From<F32>( "42.0f" ) );
    }

    TEST( String, From, F64 )
    {
        EXPECT_DOUBLE_EQ( 42.0, String::From<F64>( "42.0" ) );
    }

    TEST( String, From, F32Decimal )
    {
        EXPECT_FLOAT_EQ( 42.42f, String::From<F32>( "42.42" ) );
    }

    TEST( String, From, F32Decimalf )
    {
        EXPECT_FLOAT_EQ( 42.42f, String::From<F32>( "42.42f" ) );
    }

    TEST( String, From, F64Decimal )
    {
        EXPECT_DOUBLE_EQ( 42.42, String::From<F64>( "42.42" ) );
    }

    TEST( String, FromWString )
    {
        //! [FromWString]
        EXPECT_EQ( "Test String", String::FromWString( L"Test String" ) );
        //! [FromWString]
    }

    TEST( String, ToWString )
    {
        //! [ToWString]
        EXPECT_EQ( L"Test String", String::ToWString( "Test String" ) );
        //! [ToWString]
    }

    TEST( String, IsWhiteSpace, None )
    {
        EXPECT_TRUE( String::IsWhiteSpace( "" ) );
    }

    TEST( String, IsWhiteSpace, Space )
    {
        EXPECT_TRUE( String::IsWhiteSpace( " " ) );
    }

    TEST( String, IsWhiteSpace, Tab )
    {
        EXPECT_TRUE( String::IsWhiteSpace( "\t" ) );
    }

    TEST( String, IsWhiteSpace, Newline )
    {
        EXPECT_TRUE( String::IsWhiteSpace( "\n" ) );
    }

    TEST( String, IsWhiteSpace )
    {
        //! [IsWhiteSpace]
        EXPECT_TRUE( String::IsWhiteSpace( "\n\t\r\n\t  " ) );
        //! [IsWhiteSpace]
    }

    TEST( String, Trim, None )
    {
        EXPECT_EQ( "", String::Trim( "" ) );
    }

    TEST( String, Trim, Space )
    {
        EXPECT_EQ( "#", String::Trim( " # " ) );
    }

    TEST( String, Trim, Tab )
    {
        EXPECT_EQ( "#", String::Trim( "\t#\t" ) );
    }

    TEST( String, Trim, Newline )
    {
        EXPECT_EQ( "#", String::Trim( "\n#\n" ) );
    }

    TEST( String, Trim )
    {
        //! [Trim]
        EXPECT_EQ( "#", String::Trim( "\n\t\r\n\t #\n\t\r\n\t " ) );
        //! [Trim]
    }

    TEST( String, Split )
    {
        //! [Split]
        std::vector< std::string > parts = String::Split( "< >", ' ', false );
        EXPECT_EQ( "<", parts[0] );
        EXPECT_EQ( ">", parts[1] );
        //! [Split]
    }

    TEST( String, Split, Multiple )
    {
        std::vector< std::string > parts = String::Split( "< > < > < >", ' ', false );
        EXPECT_EQ( "<", parts[0] );
        EXPECT_EQ( ">", parts[1] );
        EXPECT_EQ( "<", parts[2] );
        EXPECT_EQ( ">", parts[3] );
        EXPECT_EQ( "<", parts[4] );
        EXPECT_EQ( ">", parts[5] );
    }

    TEST( String, Split, Trimmed )
    {
        //! [Split Trimmed]
        std::vector< std::string > parts = String::Split( "< ; >", ';', true );
        EXPECT_EQ( "<", parts[0] );
        EXPECT_EQ( ">", parts[1] );
        //! [Split Trimmed]
    }

    TEST( String, Split, Whitespace )
    {
        {
            std::vector< std::string > parts = String::Split( " ", ' ', true );
            EXPECT_EQ( "", parts[0] );
        }
        {
            std::vector< std::string > parts = String::Split( "   ", ' ', true );
            EXPECT_EQ( "", parts[0] );
        }
        {
            std::vector< std::string > parts = String::Split( "   ", ' ', false );
            EXPECT_EQ( " ", parts[0] );
            EXPECT_EQ( " ", parts[1] );
        }
    }

    TEST( String, Replace )
    {
        //! [Replace]
        EXPECT_EQ( "@@@@", String::Replace( "##", "#", "@@" ) );
        //! [Replace]
    }

    TEST( String, Capitalise )
    {
        //! [Capitalise]
        EXPECT_EQ( "Test string", String::Capitalise( "test string" ) );
        //! [Capitalise]
    }

    TEST( String, Capitalise, WithCapital )
    {
        EXPECT_EQ( "Test String", String::Capitalise( "Test String" ) );
    }
}