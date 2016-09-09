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

namespace slack
{
    namespace _detail
    {
        class Slacking;
    }
}

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


    template <typename... Args>
    void Printf( Args &&... args )
    {
        if ( MayReport( Console::LogMode::Print ) )
        {
            Echo( String::Place( std::forward<Args>( args )... ), Console::LogMode::Print );
        }
    }

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

    template <typename... Args>
    void Errorf( const std::string &fmt, Args &&... args )
    {
        const std::string str = String::Format( fmt, std::forward<Args>( args )... );

        if ( MayReport( Console::LogMode::Error ) )
        {
            Echo( str, Console::LogMode::Error );
        }

#ifndef ENGINE_SHIPVERSION
        throw std::runtime_error( str );
#endif
    }

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

    template <typename... Args>
    void Warningf( Args &&... args )
    {
        if ( MayReport( Console::LogMode::Warning ) )
        {
            Echo( String::Place( std::forward<Args>( args )... ), Console::LogMode::Warning );
        }
    }

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

    template <typename... Args>
    void Initf( Args &&... args )
    {
        if ( MayReport( Console::LogMode::Initialisation ) )
        {
            Echo( String::Place( std::forward<Args>( args )... ), Console::LogMode::Initialisation );
        }
    }

    /// @}

private:

    std::stringstream mStartupBuffer;
    mutable std::recursive_mutex mMutex;

    slack::_detail::Slacking *mSlacking;

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

    void Slack( Console::LogMode type, const std::string &str );

    std::string GetLogFilePath() const;

    std::string GetSlackToken() const;

    std::string GetSlackChannel() const;

    Console::LogMode GetSlackLogLevel() const;

    bool MayReport( Console::LogMode mode ) const;

    static bool MayReport( Console::LogMode mode, Console::LogMode current );
};

/// @}

#endif

