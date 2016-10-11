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

#include "manager/applicationManager.h"
#include "manager/systemManager.h"

#include "common/program.h"

Program::Program( S32 argc, char **argv ) noexcept
    : mIsInitialised( false ),
      mArgc( argc ),
      mArgv( argv )
{   
    // Create a system manager and provide it for global access
    // using the service locater pattern.
    SystemManager *systemManager = new SystemManager( mArgc, mArgv );

    // Provide our service locator
    SystemManager::Get( systemManager );

    systemManager->RegisterManagers();
}

Program::~Program()
{
    Shutdown();
}

void Program::Update()
{
    if ( !mIsInitialised )
    {
        Init();
    }

    SystemManager::Get()->GetManagers()->system->Update();
}

void Program::Init()
{
    systemManager->Initialise();
    
    mIsInitialised = true;
}

bool Program::IsRunning() const noexcept
{
    if ( !mIsInitialised )
    {
        return true;
    }

    return SystemManager::Get()->GetManagers()->application->IsRunning();
}

std::map<std::string, docopt::value> Program::ParseCLI( const std::string &usage, bool help /*= true*/,
                                                        bool optionsFirst /*= false */ ) const
{
    return docopt::docopt( usage, { mArgv + 1, mArgv + mArgc }, help,
                           SystemManager::Get()->GetManagers()->application->GetVersion(), optionsFirst );
}

void Program::Shutdown()
{
    SystemManager::Get()->Release();
}