/**
 * @cond ___LICENSE___
 *
 * Copyright (c) 2017 Koen Visscher, Paul Visscher and individual contributors.
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

#include <iostream>

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


    std::string result = String::Place( "[{:%Y-%m-%dT%H:%M:%SZ}] {}: {}\n", Util::Now(), Console::GetName( type ), str );

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

    std::cout << str << "\n";
}

std::string LogManager::GetLogFilePath() const
{
    return GetManagers()->configuration->GetString( "ConsoleLog" );
}

bool LogManager::MayReport( Console::LogMode mode ) const
{
    return MayReport( mode, mLogMode );
}

bool LogManager::MayReport( Console::LogMode mode, Console::LogMode current )
{
    return static_cast<U32>( mode ) <= static_cast<U32>( current );
}
