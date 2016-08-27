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

#include "manager/abstract/abstractManager.h"

#include "api/console.h"

AbstractManager::AbstractManager()
    : mInitialised( false ),
      mFlags( 0 ),
      mName( "" ),
      mManagerHolder( nullptr )
{
}

AbstractManager::~AbstractManager()
{
}

void AbstractManager::Initialise()
{
    // Prevent the manager to be initialised from more than once.
    if ( !mInitialised )
    {
        OnInit();
        mInitialised = true;
    }
}

void AbstractManager::Release()
{
    OnRelease();
}

void AbstractManager::PreInitialise()
{
    OnPreInit();
}

void AbstractManager::PostInitialise()
{
    OnPostInit();
}

void AbstractManager::PreRelease()
{
    OnPreRelease();
}

void AbstractManager::PostRelease()
{
    OnPostRelease();
}

void AbstractManager::PreUpdate()
{
    OnPreUpdate();
}

void AbstractManager::Update()
{
    OnUpdate();
}

void AbstractManager::PostUpdate()
{
    OnPostUpdate();
}

void AbstractManager::Synchronise()
{
    OnSynchronise();
}

void AbstractManager::ProcessEvents()
{
    OnProcessEvents();
}

void AbstractManager::OnUpdate()
{
}

void AbstractManager::OnPostUpdate()
{
}

void AbstractManager::OnSynchronise()
{
}

void AbstractManager::OnProcessEvents()
{
}

void AbstractManager::SetName( const std::string &name )
{
    mName = name;
}

const std::string &AbstractManager::GetName()
{
    return mName;
}

void AbstractManager::SetManagers( ManagerHolder *const managerHolder )
{
    mManagerHolder = managerHolder;
}

ManagerHolder *AbstractManager::GetManagers() const
{
    return mManagerHolder;
}

bool AbstractManager::IsInitialised() const
{
    return mInitialised;
}

void AbstractManager::OnInit()
{

}

void AbstractManager::OnRelease()
{

}

void AbstractManager::OnRelease( Namespace )
{

}

void AbstractManager::OnPreInit()
{

}

void AbstractManager::OnPostInit()
{

}

void AbstractManager::OnPreRelease()
{

}

void AbstractManager::OnPreUpdate()
{

}

void AbstractManager::OnPostRelease()
{

}

bool AbstractManager::IsFlagSet( const U32 flag ) const
{
    return IS_SET( mFlags, flag );
}

void AbstractManager::UnsetFlag( const U32 flag )
{
    mFlags &= ~flag;
}

void AbstractManager::SetFlag( const U32 flag )
{
    mFlags |= flag;
}
