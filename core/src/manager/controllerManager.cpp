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

#include "manager/controllerManager.h"

void ControllerManager::OnInit()
{
    for ( auto &cont : mControllerCache )
    {
        cont->OnInit();
    }
}

void ControllerManager::OnPostInit()
{
    for ( auto &cont : mControllerCache )
    {
        cont->OnPostInit();
    }
}

void ControllerManager::OnPreUpdate()
{
    for ( auto &cont : mControllerCache )
    {
        cont->OnPreUpdate();
    }
}

void ControllerManager::OnUpdate()
{
    for ( auto &cont : mControllerCache )
    {
        cont->OnUpdate();
    }
}

void ControllerManager::OnPostUpdate()
{
    for ( auto &cont : mControllerCache )
    {
        cont->OnPostUpdate();
    }
}

void ControllerManager::OnRelease()
{
    for ( auto &cont : mControllerCache )
    {
        cont->OnRelease();
    }
}

void ControllerManager::Add( std::type_index typeID, AbstractManager *mngr, Namespace ns /*= 0U */ )
{
    if ( !mControllers.Has( typeID ) )
    {
        mControllers.Add( mngr, typeID, ns );
        mControllerCache.push_back( mngr );
    }
    else
    {
        Console::Errorf( LOG( "Controller already registered." ) );
    }
}

void ControllerManager::OnPostRelease()
{
    for ( auto &cont : mControllerCache )
    {
        cont->OnPostRelease();
    }
}

void ControllerManager::OnRelease( Namespace ns )
{
    std::vector< std::type_index > removed = mControllers.GetNames( ns );

    // cache values
    for ( auto cont : std::vector< AbstractManager * >( mControllerCache ) )
    {
        for ( auto id : removed )
        {
            if ( cont == mControllers.Get( id ) )
            {
                cont->OnRelease();
                cont->OnPostRelease();
                mControllerCache.erase( std::remove( mControllerCache.begin(), mControllerCache.end(), cont ), mControllerCache.end() );

            }
        }
    }

    mControllers.Clear( ns );
}
