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

#include "manager/systemManager.h"

#include "api/console.h"
#include "api/system.h"

#include "engineTest.h"

namespace
{
    ENGINE_TEST( SystemManager, Sanity )
    {
        SystemManager m( 0, nullptr );
    }

    ENGINE_TEST( SystemManager, FullCycle )
    {
        System::Release();

        SystemManager *sysmgr = new SystemManager( 0, nullptr );
        SystemManager::Get( sysmgr );

        sysmgr->RegisterManagers();

        Console::SetMode( Console::LogMode::Disabled );

        sysmgr->Initialise();
        sysmgr->Update();
        sysmgr->Release();

        sysmgr = new SystemManager( 0, nullptr );
        SystemManager::Get( sysmgr );
        sysmgr->RegisterManagers();

        Console::SetMode( Console::LogMode::Disabled );
    }

    ENGINE_TEST( SystemManager, ReleaseNS )
    {
        SystemManager m( 0, nullptr );
        m.Release( 1 );
    }

    ENGINE_TEST( SystemManager, GetArgc )
    {
        SystemManager m( 0, nullptr );
        EXPECT_EQ( 0, m.GetArgc() );
    }

    ENGINE_TEST( SystemManager, GetArgv )
    {
        SystemManager m( 0, nullptr );
        EXPECT_EQ( nullptr, m.GetArgv() );
    }
}