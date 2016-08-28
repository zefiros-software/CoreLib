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

#include "manager/configurationManager.h"
#include "manager/logManager.h"

#include "common/string.h"
#include "common/file.h"
#include "common/util.h"

#include "preproc/env.h"

#include "api/console.h"

#if OS_IS_WINDOWS
#   include <windows.h>
#endif

LogManager::LogManager()
    : mLogMode( Console::LogMode::All )
{
}

void LogManager::SetMode( Console::LogMode mode )
{
    mLogMode = mode;
}

Console::LogMode LogManager::GetMode() const
{
    return mLogMode;
}

void LogManager::OnInit()
{
#include "warnings/push.h"
#include "warnings/unsafeFunction.h"

    std::time_t t = std::time( nullptr );
    Echo( String::Place( "Log file opened on '{:%Y-%m-%dT%H:%M:%SZ}'", *std::localtime( &t ) ),
          Console::LogMode::Initialisation );

#include "warnings/pop.h"

    const std::string logFile = Path::Get( GetLogFilePath(), Path::Type::Data );

    if ( File::Exists( logFile ) )
    {
        File::Clear( logFile );
    }

    Console::Initp( "Log file opened on '{:%Y-%m-%dT%H:%M:%SZ}'", *std::localtime( &t ) );

    Log( mStartupBuffer.str() );
    mStartupBuffer.clear();
}

void LogManager::OnRelease()
{
    Printf( "Performed a clean exit..." );
}

void LogManager::Printf( const std::string &fmt )
{
    if ( MayReport( Console::LogMode::Print ) )
    {
        Echo( fmt, Console::LogMode::Print );
    }
}

void LogManager::Printf( const std::string &fmt,
                         const boost::any &v1 )
{
    if ( MayReport( Console::LogMode::Print ) )
    {
        Echo( String::Format( fmt, v1 ), Console::LogMode::Print );
    }
}

void LogManager::Printf( const std::string &fmt,
                         const boost::any &v1,
                         const boost::any &v2 )
{
    if ( MayReport( Console::LogMode::Print ) )
    {
        Echo( String::Format( fmt, v1, v2 ), Console::LogMode::Print );
    }
}

void LogManager::Printf( const std::string &fmt,
                         const boost::any &v1,
                         const boost::any &v2,
                         const boost::any &v3 )
{
    if ( MayReport( Console::LogMode::Print ) )
    {
        Echo( String::Format( fmt, v1, v2, v3 ), Console::LogMode::Print );
    }
}

void LogManager::Printf( const std::string &fmt,
                         const boost::any &v1,
                         const boost::any &v2,
                         const boost::any &v3,
                         const boost::any &v4 )
{
    if ( MayReport( Console::LogMode::Print ) )
    {
        Echo( String::Format( fmt, v1, v2, v3, v4 ), Console::LogMode::Print );
    }
}

void LogManager::Printf( const std::string &fmt,
                         const boost::any &v1,
                         const boost::any &v2,
                         const boost::any &v3,
                         const boost::any &v4,
                         const boost::any &v5 )
{
    if ( MayReport( Console::LogMode::Print ) )
    {
        Echo( String::Format( fmt, v1, v2, v3, v4, v5 ), Console::LogMode::Print );
    }
}

void LogManager::Printf( const std::string &fmt,
                         const boost::any &v1,
                         const boost::any &v2,
                         const boost::any &v3,
                         const boost::any &v4,
                         const boost::any &v5,
                         const boost::any &v6 )
{
    if ( MayReport( Console::LogMode::Print ) )
    {
        Echo( String::Format( fmt, v1, v2, v3, v4, v5, v6 ), Console::LogMode::Print );
    }
}

void LogManager::Printf( const std::string &fmt,
                         const boost::any &v1,
                         const boost::any &v2,
                         const boost::any &v3,
                         const boost::any &v4,
                         const boost::any &v5,
                         const boost::any &v6,
                         const boost::any &v7 )
{
    if ( MayReport( Console::LogMode::Print ) )
    {
        Echo( String::Format( fmt, v1, v2, v3, v4, v5, v6, v7 ), Console::LogMode::Print );
    }
}

void LogManager::Printf( const std::string &fmt,
                         const boost::any &v1,
                         const boost::any &v2,
                         const boost::any &v3,
                         const boost::any &v4,
                         const boost::any &v5,
                         const boost::any &v6,
                         const boost::any &v7,
                         const boost::any &v8 )
{
    if ( MayReport( Console::LogMode::Print ) )
    {
        Echo( String::Format( fmt, v1, v2, v3, v4, v5, v6, v7, v8 ), Console::LogMode::Print );
    }
}

void LogManager::Errorf( const std::string &fmt )
{
    const std::string str = fmt;

    if ( MayReport( Console::LogMode::Error ) )
    {
        Echo( str, Console::LogMode::Error );
    }

#ifndef ENGINE_SHIPVERSION
    throw std::runtime_error( str );
#endif
}

void LogManager::Errorf( const std::string &fmt,
                         const boost::any &v1 )
{
    const std::string str = String::Format( fmt, v1 );

    if ( MayReport( Console::LogMode::Error ) )
    {
        Echo( str, Console::LogMode::Error );
    }

#ifndef ENGINE_SHIPVERSION
    throw std::runtime_error( str );
#endif
}

void LogManager::Errorf( const std::string &fmt,
                         const boost::any &v1,
                         const boost::any &v2 )
{
    const std::string str = String::Format( fmt, v1, v2 );

    if ( MayReport( Console::LogMode::Error ) )
    {
        Echo( str, Console::LogMode::Error );
    }

#ifndef ENGINE_SHIPVERSION
    throw std::runtime_error( str );
#endif
}

void LogManager::Errorf( const std::string &fmt,
                         const boost::any &v1,
                         const boost::any &v2,
                         const boost::any &v3 )
{
    const std::string str = String::Format( fmt, v1, v2, v3 );

    if ( MayReport( Console::LogMode::Error ) )
    {
        Echo( str, Console::LogMode::Error );
    }

#ifndef ENGINE_SHIPVERSION
    throw std::runtime_error( str );
#endif
}

void LogManager::Errorf( const std::string &fmt,
                         const boost::any &v1,
                         const boost::any &v2,
                         const boost::any &v3,
                         const boost::any &v4 )
{
    const std::string str = String::Format( fmt, v1, v2, v3, v4 );

    if ( MayReport( Console::LogMode::Error ) )
    {
        Echo( str, Console::LogMode::Error );
    }

#ifndef ENGINE_SHIPVERSION
    throw std::runtime_error( str );
#endif
}

void LogManager::Errorf( const std::string &fmt,
                         const boost::any &v1,
                         const boost::any &v2,
                         const boost::any &v3,
                         const boost::any &v4,
                         const boost::any &v5 )
{
    const std::string str = String::Format( fmt, v1, v2, v3, v4, v5 );

    if ( MayReport( Console::LogMode::Error ) )
    {
        Echo( str, Console::LogMode::Error );
    }

#ifndef ENGINE_SHIPVERSION
    throw std::runtime_error( str );
#endif
}

void LogManager::Errorf( const std::string &fmt,
                         const boost::any &v1,
                         const boost::any &v2,
                         const boost::any &v3,
                         const boost::any &v4,
                         const boost::any &v5,
                         const boost::any &v6 )
{
    const std::string str = String::Format( fmt, v1, v2, v3, v4, v5, v6 );

    if ( MayReport( Console::LogMode::Error ) )
    {
        Echo( str, Console::LogMode::Error );
    }

#ifndef ENGINE_SHIPVERSION
    throw std::runtime_error( str );
#endif
}

void LogManager::Errorf( const std::string &fmt,
                         const boost::any &v1,
                         const boost::any &v2,
                         const boost::any &v3,
                         const boost::any &v4,
                         const boost::any &v5,
                         const boost::any &v6,
                         const boost::any &v7 )
{
    const std::string str = String::Format( fmt, v1, v2, v3, v4, v5, v6, v7 );

    if ( MayReport( Console::LogMode::Error ) )
    {
        Echo( str, Console::LogMode::Error );
    }

#ifndef ENGINE_SHIPVERSION
    throw std::runtime_error( str );
#endif
}

void LogManager::Errorf( const std::string &fmt,
                         const boost::any &v1,
                         const boost::any &v2,
                         const boost::any &v3,
                         const boost::any &v4,
                         const boost::any &v5,
                         const boost::any &v6,
                         const boost::any &v7,
                         const boost::any &v8 )
{
    const std::string str = String::Format( fmt, v1, v2, v3, v4, v5, v6, v7, v8 );

    if ( MayReport( Console::LogMode::Error ) )
    {
        Echo( str, Console::LogMode::Error );
    }

#ifndef ENGINE_SHIPVERSION
    throw std::runtime_error( str );
#endif
}

void LogManager::Warningf( const std::string &fmt )
{
    if ( MayReport( Console::LogMode::Warning ) )
    {
        Echo( fmt, Console::LogMode::Warning );
    }
}

void LogManager::Warningf( const std::string &fmt,
                           const boost::any &v1 )
{
    if ( MayReport( Console::LogMode::Warning ) )
    {
        Echo( String::Format( fmt, v1 ), Console::LogMode::Warning );
    }
}

void LogManager::Warningf( const std::string &fmt,
                           const boost::any &v1,
                           const boost::any &v2 )
{
    if ( MayReport( Console::LogMode::Warning ) )
    {
        Echo( String::Format( fmt, v1, v2 ), Console::LogMode::Warning );
    }
}

void LogManager::Warningf( const std::string &fmt,
                           const boost::any &v1,
                           const boost::any &v2,
                           const boost::any &v3 )
{
    if ( MayReport( Console::LogMode::Warning ) )
    {
        Echo( String::Format( fmt, v1, v2, v3 ), Console::LogMode::Warning );
    }
}

void LogManager::Warningf( const std::string &fmt,
                           const boost::any &v1,
                           const boost::any &v2,
                           const boost::any &v3,
                           const boost::any &v4 )
{
    if ( MayReport( Console::LogMode::Warning ) )
    {
        Echo( String::Format( fmt, v1, v2, v3, v4 ), Console::LogMode::Warning );
    }
}

void LogManager::Warningf( const std::string &fmt,
                           const boost::any &v1,
                           const boost::any &v2,
                           const boost::any &v3,
                           const boost::any &v4,
                           const boost::any &v5 )
{
    if ( MayReport( Console::LogMode::Warning ) )
    {
        Echo( String::Format( fmt, v1, v2, v3, v4, v5 ), Console::LogMode::Warning );
    }
}

void LogManager::Warningf( const std::string &fmt,
                           const boost::any &v1,
                           const boost::any &v2,
                           const boost::any &v3,
                           const boost::any &v4,
                           const boost::any &v5,
                           const boost::any &v6 )
{
    if ( MayReport( Console::LogMode::Warning ) )
    {
        Echo( String::Format( fmt, v1, v2, v3, v4, v5, v6 ), Console::LogMode::Warning );
    }
}

void LogManager::Warningf( const std::string &fmt,
                           const boost::any &v1,
                           const boost::any &v2,
                           const boost::any &v3,
                           const boost::any &v4,
                           const boost::any &v5,
                           const boost::any &v6,
                           const boost::any &v7 )
{
    if ( MayReport( Console::LogMode::Warning ) )
    {
        Echo( String::Format( fmt, v1, v2, v3, v4, v5, v6, v7 ), Console::LogMode::Warning );
    }
}

void LogManager::Warningf( const std::string &fmt,
                           const boost::any &v1,
                           const boost::any &v2,
                           const boost::any &v3,
                           const boost::any &v4,
                           const boost::any &v5,
                           const boost::any &v6,
                           const boost::any &v7,
                           const boost::any &v8 )
{
    if ( MayReport( Console::LogMode::Warning ) )
    {
        Echo( String::Format( fmt, v1, v2, v3, v4, v5, v6, v7, v8 ), Console::LogMode::Warning );
    }
}

void LogManager::Initf( const std::string &fmt )
{
    if ( MayReport( Console::LogMode::Initialisation ) )
    {
        Echo( fmt, Console::LogMode::Initialisation );
    }
}

void LogManager::Initf( const std::string &fmt,
                        const boost::any &v1 )
{
    if ( MayReport( Console::LogMode::Initialisation ) )
    {
        Echo( String::Format( fmt, v1 ), Console::LogMode::Initialisation );
    }
}

void LogManager::Initf( const std::string &fmt,
                        const boost::any &v1,
                        const boost::any &v2 )
{
    if ( MayReport( Console::LogMode::Initialisation ) )
    {
        Echo( String::Format( fmt, v1, v2 ), Console::LogMode::Initialisation );
    }
}

void LogManager::Initf( const std::string &fmt,
                        const boost::any &v1,
                        const boost::any &v2,
                        const boost::any &v3 )
{
    if ( MayReport( Console::LogMode::Initialisation ) )
    {
        Echo( String::Format( fmt, v1, v2, v3 ), Console::LogMode::Initialisation );
    }
}

void LogManager::Initf( const std::string &fmt,
                        const boost::any &v1,
                        const boost::any &v2,
                        const boost::any &v3,
                        const boost::any &v4 )
{
    if ( MayReport( Console::LogMode::Initialisation ) )
    {
        Echo( String::Format( fmt, v1, v2, v3, v4 ), Console::LogMode::Initialisation );
    }
}

void LogManager::Initf( const std::string &fmt,
                        const boost::any &v1,
                        const boost::any &v2,
                        const boost::any &v3,
                        const boost::any &v4,
                        const boost::any &v5 )
{
    if ( MayReport( Console::LogMode::Initialisation ) )
    {
        Echo( String::Format( fmt, v1, v2, v3, v4, v5 ), Console::LogMode::Initialisation );
    }
}

void LogManager::Initf( const std::string &fmt,
                        const boost::any &v1,
                        const boost::any &v2,
                        const boost::any &v3,
                        const boost::any &v4,
                        const boost::any &v5,
                        const boost::any &v6 )
{
    if ( MayReport( Console::LogMode::Initialisation ) )
    {
        Echo( String::Format( fmt, v1, v2, v3, v4, v5, v6 ), Console::LogMode::Initialisation );
    }
}

void LogManager::Initf( const std::string &fmt,
                        const boost::any &v1,
                        const boost::any &v2,
                        const boost::any &v3,
                        const boost::any &v4,
                        const boost::any &v5,
                        const boost::any &v6,
                        const boost::any &v7 )
{
    if ( MayReport( Console::LogMode::Initialisation ) )
    {
        Echo( String::Format( fmt, v1, v2, v3, v4, v5, v6, v7 ), Console::LogMode::Initialisation );
    }
}

void LogManager::Initf( const std::string &fmt,
                        const boost::any &v1,
                        const boost::any &v2,
                        const boost::any &v3,
                        const boost::any &v4,
                        const boost::any &v5,
                        const boost::any &v6,
                        const boost::any &v7,
                        const boost::any &v8 )
{
    if ( MayReport( Console::LogMode::Initialisation ) )
    {
        Echo( String::Format( fmt, v1, v2, v3, v4, v5, v6, v7, v8 ), Console::LogMode::Initialisation );
    }
}

void LogManager::Log( const std::string &str ) const
{
    std::lock_guard< std::recursive_mutex > lock( mMutex );

    std::ofstream pFile;
    File::WriteOpen( pFile, GetLogFilePath(), Path::Type::Data, std::ios::out | std::ios::app );

    pFile << str;

    pFile.close();
}

void LogManager::Echo( const std::string &str, Console::LogMode type )
{
    std::lock_guard< std::recursive_mutex > lock( mMutex );

#include "warnings/push.h"
#include "warnings/unsafeFunction.h"

    std::time_t t = std::time( nullptr );
    std::string result =
        String::Place( "[{:%Y-%m-%dT%H:%M:%SZ}] {}: {}\n", *std::localtime( &t ), Console::GetName( type ), str );

#include "warnings/pop.h"

    if ( !IsInitialised() )
    {
        mStartupBuffer << result;
    }
    else
    {
        Log( result );
    }

#if defined( _MSC_VER ) && IS_DEBUG

    //output to the visual studio output log
    OutputDebugStringW( String::ToWString( result ).c_str() );

#endif
}

std::string LogManager::GetLogFilePath() const
{
    return GetManagers()->configuration->GetString( "ConsoleLog" );
}

bool LogManager::MayReport( Console::LogMode mode ) const
{
    return static_cast< U32 >( mode ) <= static_cast< U32 >( mLogMode );
}
