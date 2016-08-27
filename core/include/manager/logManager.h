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
#ifndef __ENGINE_LOGMANAGER_H__
#define __ENGINE_LOGMANAGER_H__

#include "manager/abstract/abstractManager.h"

#include "common/format.h"

#include "api/logMode.h"

#include "external/any.h"

#include <sstream>
#include <mutex>

/// @addtogroup Console
/// @{

class LogManager
    : public AbstractManager
{
public:

    LogManager();

    /// @name Mode
    /// @{

    /**
     * Sets the logging mode on which the log manager operates.
     *
     * @param   mode The mode.
     */

    void SetMode( Console::LogMode mode );

    /**
     * Gets the logging mode.
     *
     * @return The mode.
     */

    Console::LogMode GetMode() const;

    /// @}

    /// @name Events
    /// @{

    virtual void OnInit() override;

    virtual void OnRelease() override;

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
        Printf( String::Place( std::forward<Args>( args )... ) );
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
        Errorf( String::Place( std::forward<Args>( args )... ) );
    }

    /// @}

    /**
     * @name    Formatted Error
     * @see     ::Util : String Formatters
     *
     * Prints the formatted error string into the engine console. This is an error log that can be
     * filtered from the other print types.
     *
     * @param   fmt Describes the format to use.
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
        Warningf( String::Place( std::forward<Args>( args )... ) );
    }

    /// @}

    /**
     * @name    Formatted Warning
     * @see     ::Util : String Formatters
     *
     * Prints the formatted warning string into the engine console. This is an warning log that can
     * be filtered from the other print types.
     *
     * @param   fmt Describes the format to use.
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
        Initf( String::Place( std::forward<Args>( args )... ) );
    }

    /// @}

    /**
     * @name    Formatted Init
     * @see     ::Util : String Formatters
     *
     * Prints the formatted initialization string into the engine console. This is an init log that
     * can be filtered from the other print types.
     *
     * @param   parameter1  The first parameter.
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

private:

    std::stringstream mStartupBuffer;
    mutable std::recursive_mutex mMutex;

    /// Holds the current reporting mode, that defaults to reporting all information.
    Console::LogMode mLogMode;

    /**
     * Write directly to the log file, should not be used directly.
     *
     * @param   str The string.
     */

    void Log( const std::string &str ) const;

    /**
     * Echoes the given string.
     *
     * @param   str The string.
     */

    void Echo( const std::string &str, Console::LogMode type );

    std::string GetLogFilePath() const;

    bool MayReport( Console::LogMode mode ) const;
};

/// @}

#endif

