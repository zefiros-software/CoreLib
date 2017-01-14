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

#include "manager/applicationManager.h"

#include "events/applicationEvents.h"

#include "common/path.h"

#include "api/console.h"

#include "config.h"


ApplicationManager::ApplicationManager()
    : mApplicationIsRunning( true ),
      mApplicationIsActive( true )
{
}

void ApplicationManager::OnInit()
{
    // Show what version we are running so we
    // can more effectively debug the different
    // versions of the engine when we only have
    // a console log.
    Console::Initf( PROGRAM_NAME " build version '%s'", GetVersion() );

    Console::Initf( "Working directory:\t\t'%s'.", Path::GetWorkingDirectory() );
    Console::Initf( "Temporary directory:\t'%s'.", Path::GetProgramTempDirectory() );
    Console::Initf( "Data directory:\t\t\t'%s'.", Path::GetProgramDataDirectory() );
    Console::Initf( "Shared data directory:\t'%s'.", Path::GetProgramSharedDataDirectory() );
}

bool ApplicationManager::IsRunning() const
{
    std::lock_guard< std::mutex > lock( mMutex );

    return mApplicationIsRunning;
}

bool ApplicationManager::IsActive() const
{
    std::lock_guard< std::mutex > lock( mMutex );

    return mApplicationIsActive;
}

void ApplicationManager::Quit()
{
    std::lock_guard< std::mutex > lock( mMutex );

    mApplicationIsRunning = false;
}

bool ApplicationManager::IsDebug()
{
#if IS_DEBUG
    return true;
#else
    return false;
#endif
}

std::string ApplicationManager::GetVersion() const
{
    return String::Format( "%i.%i.%i", PROGRAM_VERSION_MAJOR, PROGRAM_VERSION_MINOR, PROGRAM_VERSION_REVISION );
}
