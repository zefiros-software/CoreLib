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
#ifndef __ENGINE_CONSOLE_H__
#define __ENGINE_CONSOLE_H__

#include "manager/systemManager.h"
#include "manager/logManager.h"

#include "common/format.h"
#include "common/string.h"
#include "common/types.h"
#include "common/util.h"

#include "preproc/config.h"

#include "api/logMode.h"

#include "external/currentFunction.h"

#if IS_DEBUG
#   define LOG( str ) ( String::Format("%s( %u ):\n\t%s:\n\t\t%s", std::string(__FILE__), (U32)__LINE__, std::string(BOOST_CURRENT_FUNCTION), String::Replace( str, "\n","\n\t\t")))
#else
#   define LOG( str ) ( String::Format("%s:\n\t%s", std::string( BOOST_CURRENT_FUNCTION ), String::Replace( str, "\n", "\n\t" ) ) )
#endif

///@addtogroup Logging
///@{

/**
* This namespace holds functions used to easily output to the engine console, the functions
* provide a type safe way of formatting.
*/

namespace Console
{
    /// @name Mode
    /// @{

    /**
    * Sets the logging mode on which the log manager operates.
    *
    * @param   mode The mode.
    */

    void SetMode( LogMode mode );

    /**
    * Gets the logging mode.
    *
    * @return The mode.
    */

    LogMode GetMode();

    /// @}

    /**
    * @name    Positionally formatted Print.
    *
    * @param [in,out]  fmt Describes the formatter to use.
    * @param   args        The arguments.
    *
    * @sa
    * ::Util : String Formatters
    *
    * Prints the formatted string into the engine console. This is a normal print that can be filtered from the
    * other print types.
    */

    /// @{

    template <typename... Args>
    void Printp( Args &&... args )
    {
        SystemManager::Get()->GetManagers()->log->Printp( std::forward<Args>( args )... );
    }

    /// @}

    /**
    * @name    Formatted Print
    * @see     ::Util : String Formatters
    *
    * Prints the formatted string into the engine console. This is a normal print that can be
    * filtered from the other print types.
    *
    * @param   fmt Describes the format to use.
    */

    /// @{

    void Printf( const std::string &fmt );

    void Printf( const std::string &fmt,
                 const boost::any &v1 );

    void Printf( const std::string &fmt,
                 const boost::any &v1,
                 const boost::any &v2 );

    void Printf( const std::string &fmt,
                 const boost::any &v1,
                 const boost::any &v2,
                 const boost::any &v3 );

    void Printf( const std::string &fmt,
                 const boost::any &v1,
                 const boost::any &v2,
                 const boost::any &v3,
                 const boost::any &v4 );

    void Printf( const std::string &fmt,
                 const boost::any &v1,
                 const boost::any &v2,
                 const boost::any &v3,
                 const boost::any &v4,
                 const boost::any &v5 );

    void Printf( const std::string &fmt,
                 const boost::any &v1,
                 const boost::any &v2,
                 const boost::any &v3,
                 const boost::any &v4,
                 const boost::any &v5,
                 const boost::any &v6 );

    void Printf( const std::string &fmt,
                 const boost::any &v1,
                 const boost::any &v2,
                 const boost::any &v3,
                 const boost::any &v4,
                 const boost::any &v5,
                 const boost::any &v6,
                 const boost::any &v7 );

    void Printf( const std::string &fmt,
                 const boost::any &v1,
                 const boost::any &v2,
                 const boost::any &v3,
                 const boost::any &v4,
                 const boost::any &v5,
                 const boost::any &v6,
                 const boost::any &v7,
                 const boost::any &v8 );

    /// @}

    /**
    * @name    Positionally formatted Error.
    *
    * @param [in,out]  fmt Describes the formatter to use.
    * @param   args        The arguments.
    *
    * @sa
    * ::Util : String
    *
    * Formatters Prints the formatted error string into the engine console. This is an error log that can be
    * filtered from the other print types.
    */

    /// @{

    template <typename... Args>
    void Errorp( Args &&... args )
    {
        SystemManager::Get()->GetManagers()->log->Errorp( std::forward<Args>( args )... );
    }

    /// @}

    /**
    * @name    Formatted Error
    * @see     ::Util : String Formatters
    *
    * Prints the formatted error string into the engine console. This is an error log that can be
    * filtered from the other print types.
    */
    /// @{

    void Errorf( const std::string &fmt );

    void Errorf( const std::string &fmt,
                 const boost::any &v1 );

    void Errorf( const std::string &fmt,
                 const boost::any &v1,
                 const boost::any &v2 );

    void Errorf( const std::string &fmt,
                 const boost::any &v1,
                 const boost::any &v2,
                 const boost::any &v3 );

    void Errorf( const std::string &fmt,
                 const boost::any &v1,
                 const boost::any &v2,
                 const boost::any &v3,
                 const boost::any &v4 );

    void Errorf( const std::string &fmt,
                 const boost::any &v1,
                 const boost::any &v2,
                 const boost::any &v3,
                 const boost::any &v4,
                 const boost::any &v5 );

    void Errorf( const std::string &fmt,
                 const boost::any &v1,
                 const boost::any &v2,
                 const boost::any &v3,
                 const boost::any &v4,
                 const boost::any &v5,
                 const boost::any &v6 );

    void Errorf( const std::string &fmt,
                 const boost::any &v1,
                 const boost::any &v2,
                 const boost::any &v3,
                 const boost::any &v4,
                 const boost::any &v5,
                 const boost::any &v6,
                 const boost::any &v7 );

    void Errorf( const std::string &fmt,
                 const boost::any &v1,
                 const boost::any &v2,
                 const boost::any &v3,
                 const boost::any &v4,
                 const boost::any &v5,
                 const boost::any &v6,
                 const boost::any &v7,
                 const boost::any &v8 );

    /// @}

    /**
    * @name    Positionally formatted Warning.
    *
    * @param [in,out]  fmt Describes the formatter to use.
    * @param   args        The arguments.
    *
    * @sa
    * ::Util : String
    *
    * Formatters Prints the formatted error string into the engine console. This is an error log that can be
    * filtered from the other print types.
    */

    /// @{

    template <typename... Args>
    void Warningp( Args &&... args )
    {
        SystemManager::Get()->GetManagers()->log->Warningp( std::forward<Args>( args )... );
    }

    /// @}

    /**
    * @name    Formatted Warning
    * @see     ::Util : String Formatters
    *
    * Prints the formatted warning string into the engine console. This is an warning log that can be
    * filtered from the other print types.
    */
    /// @{

    void Warningf( const std::string &fmt );

    void Warningf( const std::string &fmt,
                   const boost::any &v1 );

    void Warningf( const std::string &fmt,
                   const boost::any &v1,
                   const boost::any &v2 );

    void Warningf( const std::string &fmt,
                   const boost::any &v1,
                   const boost::any &v2,
                   const boost::any &v3 );

    void Warningf( const std::string &fmt,
                   const boost::any &v1,
                   const boost::any &v2,
                   const boost::any &v3,
                   const boost::any &v4 );

    void Warningf( const std::string &fmt,
                   const boost::any &v1,
                   const boost::any &v2,
                   const boost::any &v3,
                   const boost::any &v4,
                   const boost::any &v5 );

    void Warningf( const std::string &fmt,
                   const boost::any &v1,
                   const boost::any &v2,
                   const boost::any &v3,
                   const boost::any &v4,
                   const boost::any &v5,
                   const boost::any &v6 );

    void Warningf( const std::string &fmt,
                   const boost::any &v1,
                   const boost::any &v2,
                   const boost::any &v3,
                   const boost::any &v4,
                   const boost::any &v5,
                   const boost::any &v6,
                   const boost::any &v7 );

    void Warningf( const std::string &fmt,
                   const boost::any &v1,
                   const boost::any &v2,
                   const boost::any &v3,
                   const boost::any &v4,
                   const boost::any &v5,
                   const boost::any &v6,
                   const boost::any &v7,
                   const boost::any &v8 );

    /// @}

    /**
    * @name    Positionally formatted Init.
    *
    * @param [in,out]  fmt Describes the formatter to use.
    * @param   args        The arguments.
    *
    * @sa
    * ::Util : String
    *
    * Formatters Prints the formatted error string into the engine console. This is an error log that can be
    * filtered from the other print types.
    */

    /// @{

    template <typename... Args>
    void Initp( Args &&... args )
    {
        SystemManager::Get()->GetManagers()->log->Initp( std::forward<Args>( args )... );
    }

    /// @}

    /**
    * @name    Formatted Init
    * @see     ::Util : String Formatters
    *
    * Prints the formatted initialization string into the engine console. This is an init log that can be
    * filtered from the other print types.
    */
    /// @{

    void Initf( const std::string &fmt );

    void Initf( const std::string &fmt,
                const boost::any &v1 );

    void Initf( const std::string &fmt,
                const boost::any &v1,
                const boost::any &v2 );

    void Initf( const std::string &fmt,
                const boost::any &v1,
                const boost::any &v2,
                const boost::any &v3 );

    void Initf( const std::string &fmt,
                const boost::any &v1,
                const boost::any &v2,
                const boost::any &v3,
                const boost::any &v4 );

    void Initf( const std::string &fmt,
                const boost::any &v1,
                const boost::any &v2,
                const boost::any &v3,
                const boost::any &v4,
                const boost::any &v5 );

    void Initf( const std::string &fmt,
                const boost::any &v1,
                const boost::any &v2,
                const boost::any &v3,
                const boost::any &v4,
                const boost::any &v5,
                const boost::any &v6 );

    void Initf( const std::string &fmt,
                const boost::any &v1,
                const boost::any &v2,
                const boost::any &v3,
                const boost::any &v4,
                const boost::any &v5,
                const boost::any &v6,
                const boost::any &v7 );

    void Initf( const std::string &fmt,
                const boost::any &v1,
                const boost::any &v2,
                const boost::any &v3,
                const boost::any &v4,
                const boost::any &v5,
                const boost::any &v6,
                const boost::any &v7,
                const boost::any &v8 );


    /// @}

    /// @name Unformatted Print
    /// @{

    /**
    * Prints the given variable by converting it to a string.
    *
    * @see     ::String::To()
    *
    * @tparam  typename T  Type of the given variable.
    * @param   variable   The variable to print.
    *
    * @note    The variable type has to be compatible with String::To()
    */

    template< typename tT >
    void Print( const tT &variable )
    {
        Printf( String::To( variable ) );
    }

    /// @}

    /// @name Unformatted Error
    /// @{

    /**
    * Error logs the given variable by converting it to a string.
    *
    * @see     ::String::To()
    *
    * @tparam  typename T  Type of the given variable.
    * @param   variable    The variable to log.
    *
    * @note    The variable type has to be compatible with String::To()
    */
    template< typename tT >
    void Error( const tT &variable )
    {
        Errorf( String::To( variable ) );
    }


    /// @}

    /// @name Unformatted Warning
    /// @{

    /**
    * Warning logs the given variable by converting it to a string.
    *
    * @see     ::String::To()
    *
    * @tparam  typename T  Type of the given variable.
    * @param   variable    The variable to log.
    *
    * @note    The variable type has to be compatible with String::To()
    */
    template< typename tT >
    void Warning( const tT &variable )
    {
        Warningf( String::To( variable ) );
    }


    /// @}

    /// @name Unformated Init
    /// @{

    /**
    * Init logs the given variable by converting it to a string.
    *
    * @see     ::String::To()
    *
    * @tparam  typename T  Type of the given variable.
    * @param   variable    The variable to log.
    *
    * @note    The variable type has to be compatible with String::To()
    */
    template< typename tT >
    void Init( const tT &variable )
    {
        Initf( String::To( variable ) );
    }

    /**
    * Prints a title in the log, is printed as an init statement.
    *
    * @param   title           The title should be no larger than 255 characters.
    * @param   headerLength    (Optional) length of the header.
    */

    void PrintTitle( const std::string &title, const U8 headerLength = 70 );

    /// @}
};

/// @}

#endif