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

#include "common/file.h"

#include "engineTest.h"


namespace
{
    ENGINE_TEST( ConfigurationManager, Sanity )
    {
        ConfigurationManager m;
    }

    ENGINE_TEST( ConfigurationManager, GetConfigDirectory )
    {
        ConfigurationManager m;
        EXPECT_EQ( Path::Get( "config", Path::Type::Data ), m.GetConfigurationDirectory() );
    }

    ENGINE_TEST( ConfigurationManager, SetGlobalConfigName )
    {
        ConfigurationManager m;
        m.SetGlobalConfigFileName( "./config.cnf" );
        EXPECT_EQ( "./", m.GetConfigurationDirectory() );
    }

    ENGINE_TEST( ConfigurationManager, OnInit )
    {
        ConfigurationManager m;
        m.SetGlobalConfigFileName( "./config.cnf" );
        m.OnInit();

        EXPECT_TRUE( File::Exists( "config.cnf" ) );

        m.OnRelease();

        File::Delete( "config.cnf" );
        ASSERT_FALSE( File::Exists( "config.cnf" ) );
    }

    ENGINE_TEST( ConfigurationManager, OnInit2 )
    {
        ConfigurationManager m;
        m.SetGlobalConfigFileName( "./subdir/config.cnf" );
        m.OnInit();

        EXPECT_TRUE( File::Exists( "subdir/config.cnf" ) );

        m.OnRelease();

        ::Test::CleanUp( "subdir/" );
    }

    ENGINE_TEST( ConfigurationManager, GetString )
    {
        ConfigurationManager m;
        m.SetGlobalConfigFileName( "./subdir/config.cnf" );
        m.OnInit();

        EXPECT_EQ( "console.log", m.GetString( "ConsoleLog" ) );

        m.OnRelease();

        ::Test::CleanUp( "subdir/" );
    }

    ENGINE_TEST( ConfigurationManager, SetString )
    {
        ConfigurationManager m;
        m.SetGlobalConfigFileName( "./subdir/config.cnf" );
        m.OnInit();

        m.SetString( "ConsoleLog", "test.log" );
        EXPECT_EQ( "test.log", m.GetString( "ConsoleLog" ) );

        m.OnRelease();

        ::Test::CleanUp( "subdir/" );
    }

    class TestConfiguration
        : public ConfigurationFile
    {
    public:

        TestConfiguration()
            : stringValue( ::Test::GenerateRandomString() ),
              floatValue( ::Test::GenerateRandomF32() / 100 ),
              intValue( ::Test::GenerateRandomS32() ),
              boolValue( ::Test::GenerateRandomBool() )
        {
            AddStringKey( "stringValue", stringValue );
            AddIntKey( "intValue", intValue );
            AddFloatKey( "floatValue", floatValue );
            AddBoolKey( "boolValue", boolValue );
        }

        const std::string stringValue;
        const F32 floatValue;
        const S32 intValue;
        const bool boolValue;
    };

    ENGINE_TEST( ConfigurationManager, LoadTwice )
    {
        ASSERT_TRUE( Directory::Create( "subdir/" ) );

        ConfigurationManager m;
        m.Load<TestConfiguration>( "subdir/test.cnf", "Test" );
        TestConfiguration *test = new TestConfiguration();
        EXPECT_ANY_THROW( m.LoadByObj( test, "subdir/test.cnf", "Test" ) );

        m.OnRelease();

        delete test;

        ::Test::CleanUp( "subdir/" );
    }

    ENGINE_TEST( ConfigurationManager, LoadExisting )
    {
        ASSERT_TRUE( Directory::Create( "subdir/" ) );

        {
            std::ofstream f;
            File::WriteOpen( f, "subdir/test.cnf" );
            f << "#test" << std::endl;
            f.close();
        }


        ConfigurationManager m;
        m.Load<TestConfiguration>( "subdir/test.cnf", "Test" );

        m.OnRelease();

        ::Test::CleanUp( "subdir/" );
    }

    ENGINE_TEST( ConfigurationManager, LoadError )
    {
        ASSERT_TRUE( Directory::Create( "subdir/" ) );

        {
            std::ofstream f;
            File::WriteOpen( f, "subdir/test.cnf" );
            f << "sdfsdf 2323" << std::endl;
            f.close();
        }

        ConfigurationManager m;
        m.Load<TestConfiguration>( "subdir/test.cnf", "Test" );

        m.OnRelease();

        ::Test::CleanUp( "subdir/" );
    }

    ENGINE_TEST( ConfigurationManager, CGetString )
    {
        ASSERT_TRUE( Directory::Create( "subdir/" ) );

        ConfigurationManager m;
        TestConfiguration *test = m.Load<TestConfiguration>( "subdir/test.cnf", "Test" );

        EXPECT_EQ( test->stringValue, m.GetString( "stringValue", "Test" ) );

        m.OnRelease();

        ::Test::CleanUp( "subdir/" );
    }

    ENGINE_TEST( ConfigurationManager, CGetStringNotExist )
    {
        ASSERT_TRUE( Directory::Create( "subdir/" ) );

        ConfigurationManager m;
        m.Load<TestConfiguration>( "subdir/test.cnf", "Test" );

        EXPECT_ANY_THROW( m.GetString( "stringValue", "Test2" ) );

        m.OnRelease();

        ::Test::CleanUp( "subdir/" );
    }

    ENGINE_TEST( ConfigurationManager, CGetStringNotExist2 )
    {
        ASSERT_TRUE( Directory::Create( "subdir/" ) );

        ConfigurationManager m;
        m.Load<TestConfiguration>( "subdir/test.cnf", "Test" );

        EXPECT_ANY_THROW( m.GetString( "stringValue2", "Test" ) );

        m.OnRelease();

        ::Test::CleanUp( "subdir/" );
    }

    ENGINE_TEST( ConfigurationManager, CGetBool )
    {
        ASSERT_TRUE( Directory::Create( "subdir/" ) );

        ConfigurationManager m;
        TestConfiguration *test = m.Load<TestConfiguration>( "subdir/test.cnf", "Test" );

        EXPECT_EQ( test->boolValue, m.GetBool( "boolValue", "Test" ) );

        m.OnRelease();

        ::Test::CleanUp( "subdir/" );
    }

    ENGINE_TEST( ConfigurationManager, CGetBoolNotExist )
    {
        ASSERT_TRUE( Directory::Create( "subdir/" ) );

        ConfigurationManager m;
        m.Load<TestConfiguration>( "subdir/test.cnf", "Test" );

        EXPECT_ANY_THROW( m.GetBool( "boolValue", "Test2" ) );

        m.OnRelease();

        ::Test::CleanUp( "subdir/" );
    }

    ENGINE_TEST( ConfigurationManager, CGetBoolNotExist2 )
    {
        ASSERT_TRUE( Directory::Create( "subdir/" ) );

        ConfigurationManager m;
        m.Load<TestConfiguration>( "subdir/test.cnf", "Test" );

        EXPECT_ANY_THROW( m.GetBool( "boolValue2", "Test" ) );

        m.OnRelease();

        ::Test::CleanUp( "subdir/" );
    }

    ENGINE_TEST( ConfigurationManager, CGetFloat )
    {
        ASSERT_TRUE( Directory::Create( "subdir/" ) );

        ConfigurationManager m;
        TestConfiguration *test = m.Load<TestConfiguration>( "subdir/test.cnf", "Test" );

        EXPECT_FLOAT_EQ( test->floatValue, m.GetFloat( "floatValue", "Test" ) );

        m.OnRelease();

        ::Test::CleanUp( "subdir/" );
    }

    ENGINE_TEST( ConfigurationManager, CGetFloatNotExist )
    {
        ASSERT_TRUE( Directory::Create( "subdir/" ) );

        ConfigurationManager m;
        m.Load<TestConfiguration>( "subdir/test.cnf", "Test" );

        EXPECT_ANY_THROW( m.GetFloat( "floatValue", "Test2" ) );

        m.OnRelease();

        ::Test::CleanUp( "subdir/" );
    }

    ENGINE_TEST( ConfigurationManager, CGetFloatNotExist2 )
    {
        ASSERT_TRUE( Directory::Create( "subdir/" ) );

        ConfigurationManager m;
        m.Load<TestConfiguration>( "subdir/test.cnf", "Test" );

        EXPECT_ANY_THROW( m.GetFloat( "floatValue2", "Test" ) );

        m.OnRelease();

        ::Test::CleanUp( "subdir/" );
    }

    ENGINE_TEST( ConfigurationManager, CGetInt )
    {
        ASSERT_TRUE( Directory::Create( "subdir/" ) );

        ConfigurationManager m;
        TestConfiguration *test = m.Load<TestConfiguration>( "subdir/test.cnf", "Test" );

        EXPECT_EQ( test->intValue, m.GetInt( "intValue", "Test" ) );

        m.OnRelease();

        ::Test::CleanUp( "subdir/" );
    }

    ENGINE_TEST( ConfigurationManager, CGetIntNotExist )
    {
        ASSERT_TRUE( Directory::Create( "subdir/" ) );

        ConfigurationManager m;
        m.Load<TestConfiguration>( "subdir/test.cnf", "Test" );

        EXPECT_ANY_THROW( m.GetInt( "intValue", "Test2" ) );

        m.OnRelease();

        ::Test::CleanUp( "subdir/" );
    }

    ENGINE_TEST( ConfigurationManager, CGetIntNotExist2 )
    {
        ASSERT_TRUE( Directory::Create( "subdir/" ) );

        ConfigurationManager m;
        m.Load<TestConfiguration>( "subdir/test.cnf", "Test" );

        EXPECT_ANY_THROW( m.GetInt( "intValue2", "Test" ) );

        m.OnRelease();

        ::Test::CleanUp( "subdir/" );
    }

    ENGINE_TEST( ConfigurationManager, CSetString )
    {
        ASSERT_TRUE( Directory::Create( "subdir/" ) );

        ConfigurationManager m;
        m.Load<TestConfiguration>( "subdir/test.cnf", "Test" );

        m.SetString( "stringValue", "lol", true, "Test" );
        EXPECT_EQ( "lol", m.GetString( "stringValue", "Test" ) );

        m.OnRelease();

        ::Test::CleanUp( "subdir/" );
    }

    ENGINE_TEST( ConfigurationManager, CSetStringNotExists )
    {
        ASSERT_TRUE( Directory::Create( "subdir/" ) );

        ConfigurationManager m;
        TestConfiguration *test = m.Load<TestConfiguration>( "subdir/test.cnf", "Test" );
        m.SetString( "stringValue", "lol", false, "Test2" );

        EXPECT_EQ( test->stringValue, m.GetString( "stringValue", "Test" ) );

        m.OnRelease();

        ::Test::CleanUp( "subdir/" );
    }

    ENGINE_TEST( ConfigurationManager, CSetStringNotExists2 )
    {
        ASSERT_TRUE( Directory::Create( "subdir/" ) );

        ConfigurationManager m;
        m.Load<TestConfiguration>( "subdir/test.cnf", "Test" );

        EXPECT_ANY_THROW( m.SetString( "stringValue2", "lol", false, "Test" ) );

        m.OnRelease();

        ::Test::CleanUp( "subdir/" );
    }

    ENGINE_TEST( ConfigurationManager, CSetBool )
    {
        ASSERT_TRUE( Directory::Create( "subdir/" ) );

        ConfigurationManager m;
        TestConfiguration *test = m.Load<TestConfiguration>( "subdir/test.cnf", "Test" );

        m.SetBool( "boolValue", !test->boolValue, true, "Test" );
        EXPECT_EQ( !test->boolValue, m.GetBool( "boolValue", "Test" ) );

        m.OnRelease();

        ::Test::CleanUp( "subdir/" );
    }

    ENGINE_TEST( ConfigurationManager, CSetBoolNotExists )
    {
        ASSERT_TRUE( Directory::Create( "subdir/" ) );

        ConfigurationManager m;
        TestConfiguration *test = m.Load<TestConfiguration>( "subdir/test.cnf", "Test" );
        m.SetBool( "boolValue", !test->boolValue, false, "Test2" );

        EXPECT_EQ( test->boolValue, m.GetBool( "boolValue", "Test" ) );

        m.OnRelease();

        ::Test::CleanUp( "subdir/" );
    }

    ENGINE_TEST( ConfigurationManager, CSetBoolNotExists2 )
    {
        ASSERT_TRUE( Directory::Create( "subdir/" ) );

        ConfigurationManager m;
        TestConfiguration *test = m.Load<TestConfiguration>( "subdir/test.cnf", "Test" );

        EXPECT_ANY_THROW( m.SetBool( "boolValue2", !test->boolValue, false, "Test" ) );

        m.OnRelease();

        ::Test::CleanUp( "subdir/" );
    }

    ENGINE_TEST( ConfigurationManager, CSetFloat )
    {
        ASSERT_TRUE( Directory::Create( "subdir/" ) );

        ConfigurationManager m;
        TestConfiguration *test = m.Load<TestConfiguration>( "subdir/test.cnf", "Test" );

        m.SetFloat( "floatValue", test->floatValue + 1, true, "Test" );
        EXPECT_FLOAT_EQ( test->floatValue + 1, m.GetFloat( "floatValue", "Test" ) );

        m.OnRelease();

        ::Test::CleanUp( "subdir/" );
    }

    ENGINE_TEST( ConfigurationManager, CSetFloatNotExists )
    {
        ASSERT_TRUE( Directory::Create( "subdir/" ) );

        ConfigurationManager m;
        TestConfiguration *test = m.Load<TestConfiguration>( "subdir/test.cnf", "Test" );
        m.SetFloat( "floatValue", test->floatValue + 1, false, "Test2" );

        EXPECT_FLOAT_EQ( test->floatValue, m.GetFloat( "floatValue", "Test" ) );

        m.OnRelease();

        ::Test::CleanUp( "subdir/" );
    }

    ENGINE_TEST( ConfigurationManager, CSetFloatNotExists2 )
    {
        ASSERT_TRUE( Directory::Create( "subdir/" ) );

        ConfigurationManager m;
        TestConfiguration *test = m.Load<TestConfiguration>( "subdir/test.cnf", "Test" );

        EXPECT_ANY_THROW( m.SetFloat( "floatValue2", test->floatValue + 1, false, "Test" ) );

        m.OnRelease();

        ::Test::CleanUp( "subdir/" );
    }

    ENGINE_TEST( ConfigurationManager, CSetInt )
    {
        ASSERT_TRUE( Directory::Create( "subdir/" ) );

        ConfigurationManager m;
        TestConfiguration *test = m.Load<TestConfiguration>( "subdir/test.cnf", "Test" );

        m.SetInt( "intValue", test->intValue + 1, true, "Test" );
        EXPECT_EQ( test->intValue + 1, m.GetInt( "intValue", "Test" ) );

        m.OnRelease();

        ::Test::CleanUp( "subdir/" );
    }

    ENGINE_TEST( ConfigurationManager, CSetIntNotExists )
    {
        ASSERT_TRUE( Directory::Create( "subdir/" ) );

        ConfigurationManager m;
        TestConfiguration *test = m.Load<TestConfiguration>( "subdir/test.cnf", "Test" );
        m.SetInt( "intValue", test->intValue + 1, false, "Test2" );

        EXPECT_EQ( test->intValue, m.GetInt( "intValue", "Test" ) );

        m.OnRelease();

        ::Test::CleanUp( "subdir/" );
    }

    ENGINE_TEST( ConfigurationManager, CSetIntNotExists2 )
    {
        ASSERT_TRUE( Directory::Create( "subdir/" ) );

        ConfigurationManager m;
        TestConfiguration *test = m.Load<TestConfiguration>( "subdir/test.cnf", "Test" );

        EXPECT_ANY_THROW( m.SetInt( "intValue2", test->intValue + 1, false, "Test" ) );

        m.OnRelease();

        ::Test::CleanUp( "subdir/" );
    }
}