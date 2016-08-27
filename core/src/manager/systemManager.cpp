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

#include "manager/threadingVariableManager.h"

#include "manager/configurationManager.h"
#include "manager/applicationManager.h"
#include "manager/profilerManager.h"
#include "manager/factoryManager.h"
#include "manager/memoryManager.h"
#include "manager/systemManager.h"
#include "manager/poolManager.h"
#include "manager/logManager.h"

#include "common/directory.h"
#include "common/path.h"

#include "api/profiler.h"



SystemManager::SystemManager( S32 argc, char **argv )
    : mArgc( argc ),
      mArgv( argv )
{
    const std::string tempDir = Path::GetProgramTempDirectory();

    if ( !Directory::Exists( tempDir ) )
    {
        Directory::CreateAll( tempDir );
    }

    const std::string dataDir = Path::GetProgramDataDirectory();

    if ( !Directory::Exists( dataDir ) )
    {
        Directory::CreateAll( dataDir );
    }

    const std::string sharedDataDir = Path::GetProgramSharedDataDirectory();

    if ( !Directory::Exists( sharedDataDir ) )
    {
        Directory::CreateAll( sharedDataDir );
    }
}

SystemManager::~SystemManager()
{
}

void SystemManager::Initialise()
{
    mManagerHolder.system = this;

    Console::PrintTitle( "Pre-Initialising" );
    PreInitialiseManagers();

    Console::PrintTitle( "Initialising" );
    InitialiseManagers();

    Console::PrintTitle( "Post-Initialising" );
    PostInitialiseManagers();

    Console::PrintTitle( "Running" );
}

void SystemManager::Release()
{
    Console::PrintTitle( "Pre-Releasing" );
    PreReleaseManagers();

    Console::PrintTitle( "Releasing" );
    ReleaseManagers();

    PostReleaseManagers();

    DeleteManagers();

    delete this;
}

void SystemManager::Release( Namespace ns )
{
    ReleaseManagers( ns );
}

void SystemManager::Update()
{
    ProcessEventsManagers();

    PreUpdateManagers();

    UpdateManagers();

    PostUpdateManagers();

    SynchroniseManagers();
}

ManagerHolder *const SystemManager::GetManagers()
{
    return &mManagerHolder;
}

void SystemManager::RegisterManagers()
{
    AddManager< MemoryManager >( &mManagerHolder.memory, "Memory",
                                 static_cast< U32 >( AbstractManager::Type::Critical ) );

    AddManager< ProfilerManager >( &mManagerHolder.profile, "Profile",
                                   static_cast<U32>( AbstractManager::Type::Critical ) );

    AddManager< ApplicationManager >( &mManagerHolder.application, PROGRAM_NAME,
                                      static_cast<U32>( AbstractManager::Type::Critical ) );


    AddManager< ConfigurationManager >( &mManagerHolder.configuration, "Configuration",
                                        static_cast< U32 >( AbstractManager::Type::Critical ) );

    AddManager< LogManager >( &mManagerHolder.log, "Log",
                              static_cast< U32 >( AbstractManager::Type::Critical ) );

    AddManager< FactoryManager >( &mManagerHolder.factory, "Factory" );
    AddManager< PoolManager >( &mManagerHolder.pool, "Pool" );

    AddManager< EventManager >( &mManagerHolder.event, "Event" );
    AddManager< ThreadingVariableManager >( &mManagerHolder.threadingVariable, "Threading Variable" );
}

SystemManager *SystemManager::Get( SystemManager *systemManager /* = nullptr */ )
{
    static SystemManager *mSystemManager;

    if ( systemManager )
    {
        mSystemManager = systemManager;
    }

    return mSystemManager;
}

const S32 &SystemManager::GetArgc() const
{
    return mArgc;
}

S32 &SystemManager::GetArgc()
{
    return mArgc;
}

char **SystemManager::GetArgv() const
{
    return mArgv;
}

void SystemManager::InitialiseManagers()
{
    for ( auto it = mCrititicalManagersList.begin(); it != mCrititicalManagersList.end() &&
            mManagerHolder.application->IsRunning(); ++it )
    {
        InitialiseManager( *it );
    }

    for ( auto it = mManagersList.begin(); it != mManagersList.end() && mManagerHolder.application->IsRunning(); ++it )
    {
        InitialiseManager( *it );
    }

    Console::PrintTitle( "Finished initializing" );
}

void SystemManager::PreInitialiseManagers()
{
    for ( auto it = mCrititicalManagersList.begin(); it != mCrititicalManagersList.end(); ++it )
    {
        ( *it )->PreInitialise();
    }

    for ( auto it = mManagersList.begin(); it != mManagersList.end(); ++it )
    {
        ( *it )->PreInitialise();
    }
}

void SystemManager::PostInitialiseManagers()
{
    for ( auto it = mCrititicalManagersList.begin(); it != mCrititicalManagersList.end(); ++it )
    {
        ( *it )->PostInitialise();
    }

    for ( auto it = mManagersList.begin(); it != mManagersList.end(); ++it )
    {
        ( *it )->PostInitialise();
    }
}

void SystemManager::ReleaseManagers()
{
    for ( auto it = mManagersList.rbegin(); it != mManagersList.rend(); ++it )
    {
        ( *it )->Release();
    }

    for ( auto it = mCrititicalManagersList.rbegin(); it != mCrititicalManagersList.rend(); ++it )
    {
        ( *it )->Release();
    }
}

void SystemManager::ReleaseManagers( Namespace ns )
{
    for ( auto it = mManagersList.rbegin(); it != mManagersList.rend(); ++it )
    {
        ( *it )->OnRelease( ns );
    }

    for ( auto it = mCrititicalManagersList.rbegin(); it != mCrititicalManagersList.rend(); ++it )
    {
        ( *it )->OnRelease( ns );
    }
}

void SystemManager::PreReleaseManagers()
{
    for ( auto it = mManagersList.rbegin(); it != mManagersList.rend(); ++it )
    {
        ( *it )->PreRelease();
    }

    for ( auto it = mCrititicalManagersList.rbegin(); it != mCrititicalManagersList.rend(); ++it )
    {
        ( *it )->PreRelease();
    }
}

void SystemManager::PostReleaseManagers()
{
    for ( auto it = mManagersList.rbegin(); it != mManagersList.rend(); ++it )
    {
        ( *it )->PostRelease();
    }

    for ( auto it = mCrititicalManagersList.rbegin(); it != mCrititicalManagersList.rend(); ++it )
    {
        ( *it )->PostRelease();
    }
}

void SystemManager::DeleteManagers()
{
    for ( auto it = mManagersList.rbegin(); it != mManagersList.rend(); ++it )
    {
        delete *it;
    }

    for ( auto it = mCrititicalManagersList.rbegin(); it != mCrititicalManagersList.rend(); ++it )
    {
        delete *it;
    }
}

void SystemManager::PreUpdateManagers()
{
    ProfileStart( "SystemManager::PreUpdateManagers()" );

    for ( auto it = mCrititicalManagersList.begin(); it != mCrititicalManagersList.end(); ++it )
    {
        AbstractManager *manager = *it;
        manager->PreUpdate();

        ProfileWaypoint( "SystemManager::PreUpdateManagers()", manager->GetName() );
    }

    ProfileWaypoint( "SystemManager::PreUpdateManagers()", "CriticalManagers" );

    for ( auto it = mManagersList.begin(); it != mManagersList.end(); ++it )
    {
        AbstractManager *manager = *it;
        manager->PreUpdate();

        ProfileWaypoint( "SystemManager::PreUpdateManagers()", manager->GetName() );
    }

    ProfileEnd( "SystemManager::PreUpdateManagers()" );
}

void SystemManager::UpdateManagers()
{
    ProfileStart( "SystemManager::UpdateManagers()" );

    for ( auto it = mCrititicalManagersList.begin(); it != mCrititicalManagersList.end(); ++it )
    {
        AbstractManager *manager = *it;
        manager->Update();

        ProfileWaypoint( "SystemManager::UpdateManagers()", manager->GetName() );
    }

    ProfileWaypoint( "SystemManager::UpdateManagers()", "CriticalManagers" );

    for ( auto it = mManagersList.begin(); it != mManagersList.end(); ++it )
    {
        AbstractManager *manager = *it;
        manager->Update();

        ProfileWaypoint( "SystemManager::UpdateManagers()", manager->GetName() );
    }

    ProfileEnd( "SystemManager::UpdateManagers()" );
}

void SystemManager::PostUpdateManagers()
{
    ProfileStart( "SystemManager::PostUpdateManagers()" );

    for ( auto it = mCrititicalManagersList.begin(); it != mCrititicalManagersList.end(); ++it )
    {
        AbstractManager *manager = *it;
        manager->PostUpdate();

        ProfileWaypoint( "SystemManager::PostUpdateManagers()", manager->GetName() );
    }

    ProfileWaypoint( "SystemManager::PostUpdateManagers()", "CriticalManagers" );

    for ( auto it = mManagersList.begin(); it != mManagersList.end(); ++it )
    {
        AbstractManager *manager = *it;
        manager->PostUpdate();

        ProfileWaypoint( "SystemManager::PostUpdateManagers()", manager->GetName() );
    }

    ProfileEnd( "SystemManager::PostUpdateManagers()" );
}

void SystemManager::SynchroniseManagers()
{
    ProfileStart( "SystemManager::SynchroniseManagers()" );

    for ( auto it = mCrititicalManagersList.begin(); it != mCrititicalManagersList.end(); ++it )
    {
        AbstractManager *manager = *it;
        manager->Synchronise();

        ProfileWaypoint( "SystemManager::SynchroniseManagers()", manager->GetName() );
    }

    ProfileWaypoint( "SystemManager::SynchroniseManagers()", "CriticalManagers" );

    for ( auto it = mManagersList.begin(); it != mManagersList.end(); ++it )
    {
        AbstractManager *manager = *it;
        manager->Synchronise();

        ProfileWaypoint( "SystemManager::SynchroniseManagers()", manager->GetName() );
    }

    ProfileEnd( "SystemManager::SynchroniseManagers()" );
}

void SystemManager::ProcessEventsManagers()
{
    ProfileStart( "SystemManager::SynchroniseManagers()" );

    for ( auto it = mCrititicalManagersList.begin(); it != mCrititicalManagersList.end(); ++it )
    {
        AbstractManager *manager = *it;
        manager->ProcessEvents();

        ProfileWaypoint( "SystemManager::SynchroniseManagers()", manager->GetName() );
    }

    ProfileWaypoint( "SystemManager::SynchroniseManagers()", "CriticalManagers" );

    for ( auto it = mManagersList.begin(); it != mManagersList.end(); ++it )
    {
        AbstractManager *manager = *it;
        manager->ProcessEvents();

        ProfileWaypoint( "SystemManager::SynchroniseManagers()", manager->GetName() );
    }

    ProfileEnd( "SystemManager::SynchroniseManagers()" );
}

void SystemManager::InitialiseManager( AbstractManager *const manager )
{
    const std::string name = manager->GetName();

    if ( name != "" )
    {
        Console::PrintTitle( name + " Manager" );
    }

    manager->Initialise();
}
