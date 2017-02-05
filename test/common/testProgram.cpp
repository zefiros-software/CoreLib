/**
 * @cond ___LICENSE___
 *
 * Copyright (c) 2017 Zefiros Software.
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
#include "common/program.h"
#include "api/console.h"

#include "engineTest.h"

namespace
{
    void CleanManagerEnv( std::function<void()> f )
    {
        SystemManager::Get()->Release();

        f();

        SystemManager *sysmgr = new SystemManager( 0, nullptr );
        SystemManager::Get( sysmgr );
        sysmgr->RegisterManagers();
        Console::SetMode( Console::LogMode::Disabled );
    }

    TEST( Program, ParseCLI )
    {
        CleanManagerEnv( []()
        {
            const char *argv[] = { "", "--help" };
            Program program( 2, argv );
            program.Init();
        } );
    }

    TEST( Program, ParseCLI2 )
    {
        CleanManagerEnv( []()
        {
            const char *argv[] = { "", "--help" };
            Program program( 2, argv );
            program.AddOption( "dothings", "Do some things" );
            program.Init();
        } );
    }

    TEST( Program, ParseCLI3 )
    {
        CleanManagerEnv( []()
        {
            const char *argv[] = { "", "--help" };
            Program program( 2, argv );
            program.AddOption<size_t>( "dothings", "Do some things", []( Program::Options< size_t > v )
            {
                return v.Required();
            } );
            program.Init();
        } );
    }

    TEST( Program, ParseCLI, HasOption )
    {
        CleanManagerEnv( []()
        {
            const char *argv[] = { "", "--dothings2" };
            Program program( 2, argv );
            program.AddOption<size_t>( "dothings", "Do some things", []( Program::Options< size_t > v )
            {
                return v.Required();
            } );
            program.Init();
            EXPECT_FALSE( program.HasOption( "dothings" ) );
            EXPECT_FALSE( program.HasOption( "dothings2" ) );
        } );
    }
}
