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

#include "common/configurationFile.h"

#include "common/string.h"
#include "common/file.h"

#include "engineTest.h"

#include <fstream>
#include <random>

namespace
{

    //! [TestConfiguration Example]

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

    //! [TestConfiguration Example]

    //! [ErrorHandling]
    std::string gConfigName = "";
    std::string gKey = "";
    std::size_t gLineNumber = 0;
    std::string gValue = "";
    ConfigurationFile::ParseError gError = ConfigurationFile::ParseError::None;

    void LogErrors( ConfigurationFile::ParseError error, const std::string &configName,
                    const std::string &key, std::size_t lineNumber, const std::string &value )
    {
        gConfigName = configName;
        gKey = key;
        gLineNumber = lineNumber;
        gValue = value;
        gError = error;
    }
    //! [ErrorHandling]

    void ResetLogErrors()
    {
        // we cannot simply reset to "", since the clr will see the unused memory
        // as a memory leak. Which of course will give us false positives on the tests
        // using this function :(
        gConfigName.clear();
        gConfigName.shrink_to_fit();

        gKey.clear();
        gKey.shrink_to_fit();

        gLineNumber = 0;

        gValue.clear();
        gValue.shrink_to_fit();

        gError = ConfigurationFile::ParseError::None;
    }

    std::string GenerateComment()
    {
        static std::default_random_engine generator;
        static std::uniform_int_distribution<U32> distribution( 1, 5 );

        std::string comment = "";

        for ( U32 i = 0, end = distribution( generator ); i < end; ++i )
        {
            comment += ::Test::GenerateRandomString() + "\n";
        }

        return comment;
    }

    ENGINE_TEST( ConfigurationFile, DefaultConstructible )
    {
        EXPECT_TRUE( std::is_default_constructible<ConfigurationFile>::value );
    }


    ENGINE_TEST( ConfigurationFile, HasKey, String )
    {
        //! [HasKey]
        TestConfiguration config;

        EXPECT_TRUE( config.HasKey( "stringValue" ) );
        //! [HasKey]
    }

    ENGINE_TEST( ConfigurationFile, HasKey, Int )
    {
        TestConfiguration config;

        EXPECT_TRUE( config.HasKey( "intValue" ) );
    }

    ENGINE_TEST( ConfigurationFile, HasKey, Float )
    {
        TestConfiguration config;

        EXPECT_TRUE( config.HasKey( "floatValue" ) );
    }

    ENGINE_TEST( ConfigurationFile, HasKey, Bool )
    {
        TestConfiguration config;

        EXPECT_TRUE( config.HasKey( "boolValue" ) );
    }

    ENGINE_TEST( ConfigurationFile, IsStringKey )
    {
        //! [IsStringKey]
        TestConfiguration config;

        EXPECT_TRUE( config.IsStringKey( "stringValue" ) );
        //! [IsStringKey]
    }

    ENGINE_TEST( ConfigurationFile, IsIntKey )
    {
        //! [IsIntKey]
        TestConfiguration config;

        EXPECT_TRUE( config.IsIntKey( "intValue" ) );
        //! [IsIntKey]
    }

    ENGINE_TEST( ConfigurationFile, IsFloatKey )
    {
        //! [IsFloatKey]
        TestConfiguration config;

        EXPECT_TRUE( config.IsFloatKey( "floatValue" ) );
        //! [IsFloatKey]
    }

    ENGINE_TEST( ConfigurationFile, IsBoolKey )
    {
        //! [IsBoolKey]
        TestConfiguration config;

        EXPECT_TRUE( config.IsBoolKey( "boolValue" ) );
        //! [IsBoolKey]
    }

    ENGINE_TEST( ConfigurationFile, PreParse, StringValue )
    {
        //! [GetString]
        TestConfiguration config;

        EXPECT_EQ( config.stringValue, config.GetString( "stringValue" ) );
        //! [GetString]
    }

    ENGINE_TEST( ConfigurationFile, PreParse, IntValue )
    {
        //! [GetInt]
        TestConfiguration config;

        EXPECT_EQ( config.intValue, config.GetInt( "intValue" ) );
        //! [GetInt]
    }

    ENGINE_TEST( ConfigurationFile, PreParse, FloatValue )
    {
        //! [GetFloat]
        TestConfiguration config;

        EXPECT_EQ( config.floatValue, config.GetFloat( "floatValue" ) );
        //! [GetFloat]
    }

    ENGINE_TEST( ConfigurationFile, PreParse, BoolValue )
    {
        //! [GetBool]
        TestConfiguration config;

        EXPECT_EQ( config.boolValue, config.GetBool( "boolValue" ) );
        //! [GetBool]
    }

    ENGINE_TEST( ConfigurationFile, Parse, TestFile )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        //! [Load]
        TestConfiguration config;

        const std::string strContent = config.stringValue.substr( 0, config.stringValue.size() / 2 );
        const S32 intContent = config.intValue / 2 + 1;
        const F32 floatValue = config.floatValue / 2 + 1;
        const bool boolValue = !config.boolValue;

        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file, Path::Type::None );
            stream << "stringValue" << " = \"" << strContent << "\"\n"
                   << "intValue" << " = " << intContent << "\n"
                   << "floatValue" << " = " << String::To( floatValue ) << "\n"
                   << "boolValue" << " = " << ( boolValue ? "true" : "false" );
        }

        EXPECT_TRUE( config.Load( file, "testConfig", &LogErrors ) );

        EXPECT_EQ( strContent, config.GetString( "stringValue" ) );
        EXPECT_EQ( intContent, config.GetInt( "intValue" ) );
        EXPECT_FLOAT_EQ( floatValue, config.GetFloat( "floatValue" ) );
        EXPECT_EQ( boolValue, config.GetBool( "boolValue" ) );

        EXPECT_EQ( ConfigurationFile::ParseError::None, gError );

        //! [Load]

        ResetLogErrors();
        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, Parse, WhiteSpace )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        //! [Load]
        TestConfiguration config;

        const std::string strContent = config.stringValue.substr( 0, config.stringValue.size() / 2 );
        const S32 intContent = config.intValue / 2 + 1;
        const F32 floatValue = config.floatValue / 2 + 1;
        const bool boolValue = !config.boolValue;

        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file, Path::Type::None );
            stream << "stringValue" << " = \"" << strContent << "\"\n"
                   << "\t\t\n"
                   << "intValue" << " = " << intContent << "\n"
                   << "\t\t\n"
                   << "floatValue" << " = " << String::To( floatValue ) << "\n"
                   << "\t\t\n"
                   << "boolValue" << " = " << ( boolValue ? "true" : "false" );
        }

        EXPECT_TRUE( config.Load( file, "testConfig", &LogErrors ) );

        EXPECT_EQ( strContent, config.GetString( "stringValue" ) );
        EXPECT_EQ( intContent, config.GetInt( "intValue" ) );
        EXPECT_FLOAT_EQ( floatValue, config.GetFloat( "floatValue" ) );
        EXPECT_EQ( boolValue, config.GetBool( "boolValue" ) );

        EXPECT_EQ( ConfigurationFile::ParseError::None, gError );

        //! [Load]

        ResetLogErrors();
        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, Parse, StressTest )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        ConfigurationFile config;
        ConfigurationFile configReader;

        const std::size_t cycli = 10;
        const std::string key = "key_";

        std::unordered_map< std::string, std::pair< std::string, std::string > > mStrings;
        std::unordered_map< std::string, std::pair< F32, F32 > > mFloats;
        std::unordered_map< std::string, std::pair< S32, S32 > > mInts;
        std::unordered_map< std::string, std::pair< bool, bool > > mBools;

        for ( size_t i = 0, end = cycli; i < end; ++i )
        {
            mStrings[ key + String::To( i ) ] = std::pair< std::string, std::string >( ::Test::GenerateRandomString(),
                                                                                       ::Test::GenerateRandomString() );

            mFloats[ key + String::To( i + end ) ] = std::pair< F32, F32 >( ::Test::GenerateRandomF32(),
                                                                            ::Test::GenerateRandomF32() );

            mInts[ key + String::To( i + end * 2 ) ] = std::pair< S32, S32 >( ::Test::GenerateRandomS32(),
                                                                              ::Test::GenerateRandomS32() );

            mBools[ key + String::To( i + end * 3 ) ] = std::pair< bool, bool >( ::Test::GenerateRandomBool(),
                                                                                 ::Test::GenerateRandomBool() );
        }

        for ( auto it = mStrings.begin(), end = mStrings.end(); it != end; ++it )
        {
            ASSERT_TRUE( config.AddStringKey( it->first, it->second.first, GenerateComment() ) );
            ASSERT_TRUE( configReader.AddStringKey( it->first, it->second.first ) );
        }

        for ( auto it = mFloats.begin(), end = mFloats.end(); it != end; ++it )
        {
            ASSERT_TRUE( config.AddFloatKey( it->first, it->second.first, GenerateComment() ) );
            ASSERT_TRUE( configReader.AddFloatKey( it->first, it->second.first ) );
        }

        for ( auto it = mInts.begin(), end = mInts.end(); it != end; ++it )
        {
            ASSERT_TRUE( config.AddIntKey( it->first, it->second.first, GenerateComment() ) );
            ASSERT_TRUE( configReader.AddIntKey( it->first, it->second.first ) );
        }

        for ( auto it = mBools.begin(), end = mBools.end(); it != end; ++it )
        {
            ASSERT_TRUE( config.AddBoolKey( it->first, it->second.first, GenerateComment() ) );
            ASSERT_TRUE( configReader.AddBoolKey( it->first, it->second.first ) );
        }

        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );
        }

        EXPECT_TRUE( config.Load( file, "testConfig", &LogErrors ) );
        EXPECT_EQ( ConfigurationFile::ParseError::None, gError );

        for ( auto it = mStrings.begin(), end = mStrings.end(); it != end; ++it )
        {
            EXPECT_EQ( it->second.first, config.GetString( it->first ) );
            config.SetString( it->first, it->second.second );
        }

        for ( auto it = mFloats.begin(), end = mFloats.end(); it != end; ++it )
        {
            EXPECT_FLOAT_EQ( it->second.first, config.GetFloat( it->first ) );
            config.SetFloat( it->first, it->second.second );
        }

        for ( auto it = mInts.begin(), end = mInts.end(); it != end; ++it )
        {
            EXPECT_EQ( it->second.first, config.GetInt( it->first ) );
            config.SetInt( it->first, it->second.second );
        }

        for ( auto it = mBools.begin(), end = mBools.end(); it != end; ++it )
        {
            EXPECT_EQ( it->second.first, config.GetBool( it->first ) );
            config.SetBool( it->first, it->second.second );
        }

        EXPECT_TRUE( configReader.Load( file, "testConfig", &LogErrors ) );
        EXPECT_EQ( ConfigurationFile::ParseError::None, gError );

        for ( auto it = mStrings.begin(), end = mStrings.end(); it != end; ++it )
        {
            EXPECT_EQ( it->second.second, configReader.GetString( it->first ) );
        }

        for ( auto it = mFloats.begin(), end = mFloats.end(); it != end; ++it )
        {
            EXPECT_FLOAT_EQ( it->second.second, configReader.GetFloat( it->first ) );
        }

        for ( auto it = mInts.begin(), end = mInts.end(); it != end; ++it )
        {
            EXPECT_EQ( it->second.second, configReader.GetInt( it->first ) );
        }

        for ( auto it = mBools.begin(), end = mBools.end(); it != end; ++it )
        {
            EXPECT_EQ( it->second.second, configReader.GetBool( it->first ) );
        }

        ResetLogErrors();
        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, Parse, Comments )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        TestConfiguration config;

        const std::string strContent = config.stringValue.substr( 0, config.stringValue.size() / 2 );
        const S32 intContent = config.intValue / 2 + 1;
        const F32 floatValue = config.floatValue / 2 + 1;
        const bool boolValue = !config.boolValue;

        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file, Path::Type::None );
            stream << "#" << ::Test::GenerateRandomString() << "\n"
                   << "stringValue" << " = \"#" << strContent << "\"\n#" << ::Test::GenerateRandomString() << "\n"
                   << "intValue" << " = " << intContent << "\n"
                   << "floatValue" << " = " << String::To( floatValue ) << "\n"
                   << "boolValue" << " = " << ( boolValue ? "true" : "false" );
        }

        EXPECT_TRUE( config.Load( file, "testConfig", &LogErrors ) );

        EXPECT_EQ( "#" + strContent, config.GetString( "stringValue" ) );
        EXPECT_EQ( intContent, config.GetInt( "intValue" ) );
        EXPECT_FLOAT_EQ( floatValue, config.GetFloat( "floatValue" ) );
        EXPECT_EQ( boolValue, config.GetBool( "boolValue" ) );

        EXPECT_EQ( ConfigurationFile::ParseError::None, gError );

        ResetLogErrors();
        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, Parse, Comments2 )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        TestConfiguration config;

        const std::string strContent = config.stringValue.substr( 0, config.stringValue.size() / 2 );
        const S32 intContent = config.intValue / 2 + 1;
        const F32 floatValue = config.floatValue / 2 + 1;
        const bool boolValue = !config.boolValue;

        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file, Path::Type::None );
            stream << "#" << ::Test::GenerateRandomString() << "\n"
                   << "stringValue" << " = \'#"
                   << strContent << "\"\"\'\n#" << ::Test::GenerateRandomString() << "\n"

                   << "intValue" << " = " << intContent << "\n"
                   << "floatValue" << " = " << String::To( floatValue ) << "\n"
                   << "boolValue" << " = " << ( boolValue ? "true" : "false" );
        }

        EXPECT_TRUE( config.Load( file, "testConfig", &LogErrors ) );

        EXPECT_EQ( "#" + strContent + "\"\"", config.GetString( "stringValue" ) );
        EXPECT_EQ( intContent, config.GetInt( "intValue" ) );
        EXPECT_FLOAT_EQ( floatValue, config.GetFloat( "floatValue" ) );
        EXPECT_EQ( boolValue, config.GetBool( "boolValue" ) );

        EXPECT_EQ( ConfigurationFile::ParseError::None, gError );

        ResetLogErrors();
        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, Parse, StringValue )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        const std::string key = "key";

        const std::string content = ::Test::GenerateRandomString();
        const std::string content2 = content.substr( 0, content.size() / 2 );

        {
            ResetLogErrors();

            ASSERT_NE( content, content2 );

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file, Path::Type::None );
            stream << key << " = \"" << content << "\"";
        }

        ConfigurationFile config;
        EXPECT_TRUE( config.AddStringKey( key, content2 ) );
        EXPECT_TRUE( config.Load( file, "testConfig", &LogErrors ) );

        EXPECT_EQ( content, config.GetString( key ) );

        EXPECT_EQ( ConfigurationFile::ParseError::None, gError );

        ResetLogErrors();
        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, Parse, IntValue )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        const std::string key = "key";

        const S32 content = ::Test::GenerateRandomS32();
        const S32 content2 = content / 2  + 1;

        {
            ResetLogErrors();

            ASSERT_NE( content, content2 );

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file, Path::Type::None );
            stream << key << " = " << content;
        }

        ConfigurationFile config;
        EXPECT_TRUE( config.AddIntKey( key, content2 ) );
        EXPECT_TRUE( config.Load( file, "testConfig", &LogErrors ) );

        EXPECT_EQ( content, config.GetInt( key ) );

        EXPECT_EQ( ConfigurationFile::ParseError::None, gError );

        ResetLogErrors();
        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, Parse, FloatValue )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        const std::string key = "key";

        const F32 content = ::Test::GenerateRandomF32();
        const F32 content2 = content / 2  + 1;

        {
            ResetLogErrors();

            ASSERT_NE( content, content2 );

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file, Path::Type::None );
            stream << key << " = " << String::To( content );
        }

        ConfigurationFile config;
        EXPECT_TRUE( config.AddFloatKey( key, content2 ) );
        EXPECT_TRUE( config.Load( file, "testConfig", &LogErrors ) );

        EXPECT_FLOAT_EQ( content, config.GetFloat( key ) );

        EXPECT_EQ( ConfigurationFile::ParseError::None, gError );

        ResetLogErrors();
        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, Parse, BoolValue )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        const std::string key = "key";
        const std::string key2 = "key2";

        const bool content = ::Test::GenerateRandomBool();
        const bool content2 = !content;

        {
            ResetLogErrors();

            ASSERT_NE( content, content2 );

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file, Path::Type::None );
            stream << key << " = " << ( content ? "true" : "false" ) << "\n"
                   << key2 << " = " << content;
        }

        ConfigurationFile config;
        EXPECT_TRUE( config.AddBoolKey( key, content2 ) );
        EXPECT_TRUE( config.AddBoolKey( key2, content2 ) );
        EXPECT_TRUE( config.Load( file, "testConfig", &LogErrors ) );

        EXPECT_EQ( content, config.GetBool( key ) );
        EXPECT_EQ( content, config.GetBool( key2 ) );

        EXPECT_EQ( ConfigurationFile::ParseError::None, gError );

        ResetLogErrors();
        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, Parse, BoolValue2 )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        const std::string key = "key";
        const std::string key2 = "key2";

        const bool content = ::Test::GenerateRandomBool();
        const bool content2 = !content;

        {
            ResetLogErrors();

            ASSERT_NE( content, content2 );

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file, Path::Type::None );
            stream << key << " = " << ( content ? "1" : "0" ) << "\n"
                   << key2 << " = " << ( content ? "0" : "1" );
        }

        ConfigurationFile config;
        EXPECT_TRUE( config.AddBoolKey( key, content2 ) );
        EXPECT_TRUE( config.AddBoolKey( key2, content2 ) );
        EXPECT_TRUE( config.Load( file, "testConfig", &LogErrors ) );

        EXPECT_EQ( content, config.GetBool( key ) );
        EXPECT_EQ( content2, config.GetBool( key2 ) );

        EXPECT_EQ( ConfigurationFile::ParseError::None, gError );

        ResetLogErrors();
        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, Parse, NonExisting )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();
        const std::string file2 = root + Path::GetUniqueFileName();

        TestConfiguration config;
        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file2, Path::Type::None );
            stream << "stringValue = \"" << config.stringValue << "\"\n\n"
                   << "intValue = " << config.intValue << "\n\n"
                   << "floatValue = " << String::To( config.floatValue ) << "\n\n"
                   << "boolValue = " << ( config.boolValue ? "true" : "false" );
        }

        EXPECT_TRUE( config.Load( file, "testConfig", &LogErrors ) );

        EXPECT_TRUE( File::AreEqual( file, file2 ) );

        EXPECT_EQ( ConfigurationFile::ParseError::None, gError );

        ResetLogErrors();
        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, Parse, NonExistingPartial )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();
        const std::string file2 = root + Path::GetUniqueFileName();

        TestConfiguration config;
        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );
            {
                std::ofstream stream;
                File::WriteOpen( stream, file, Path::Type::None );
                stream << "stringValue = \"" << config.stringValue << "\"\n\n"
                       << "intValue = " << config.intValue << "\n\n";
            }
            {
                std::ofstream stream;
                File::WriteOpen( stream, file2, Path::Type::None );
                stream << "stringValue = \"" << config.stringValue << "\"\n\n"
                       << "intValue = " << config.intValue << "\n\n"
                       << "floatValue = " << String::To( config.floatValue ) << "\n\n"
                       << "boolValue = " << ( config.boolValue ? "true" : "false" );
            }
        }

        EXPECT_TRUE( config.Load( file, "testConfig", &LogErrors ) );

        EXPECT_TRUE( File::AreEqual( file, file2 ) );

        EXPECT_EQ( ConfigurationFile::ParseError::None, gError );

        ResetLogErrors();
        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, NewValues, LeaveComment )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();
        const std::string file2 = root + Path::GetUniqueFileName();

        //! [NewValues]

        TestConfiguration config;

        const std::string strContent = config.stringValue.substr( 0, config.stringValue.size() / 2 );
        const S32 intContent = config.intValue / 2 + 1;
        const F32 floatValue = config.floatValue / 2 + 1;
        const bool boolValue = !config.boolValue;

        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            {
                std::ofstream stream;
                File::WriteOpen( stream, file, Path::Type::None );
                stream << "stringValue = \"" << config.stringValue << "\"#comment\n\n"
                       << "intValue = " << config.intValue << "\n\n"
                       << "floatValue = " << String::To( config.floatValue ) << "\n\n"
                       << "boolValue = " << ( config.boolValue ? "true" : "false" );
            }

            {
                std::ofstream stream;
                File::WriteOpen( stream, file2, Path::Type::None );
                stream << "stringValue = \"" << strContent << "\"#comment\n\n"
                       << "intValue = " << intContent << "\n\n"
                       << "floatValue = " << String::To( floatValue ) << "\n\n"
                       << "boolValue = " << ( boolValue ? "true" : "false" );
            }
        }

        EXPECT_TRUE( config.Load( file, "testConfig", &LogErrors ) );

        EXPECT_EQ( ConfigurationFile::ParseError::None, config.SetString( "stringValue", strContent ) );
        EXPECT_EQ( ConfigurationFile::ParseError::None, config.SetInt( "intValue", intContent ) );
        EXPECT_EQ( ConfigurationFile::ParseError::None, config.SetFloat( "floatValue", floatValue ) );
        EXPECT_EQ( ConfigurationFile::ParseError::None, config.SetBool( "boolValue", boolValue ) );

        EXPECT_TRUE( File::AreEqual( file, file2 ) );

        std::cout << File::ReadAllText( file ) << std::endl << std::endl << File::ReadAllText( file2 );

        //! [NewValues]

        EXPECT_EQ( ConfigurationFile::ParseError::None, gError );

        ResetLogErrors();
        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, NewValues )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();
        const std::string file2 = root + Path::GetUniqueFileName();

        //! [NewValues]

        TestConfiguration config;

        const std::string strContent = config.stringValue.substr( 0, config.stringValue.size() / 2 );
        const S32 intContent  = config.intValue / 2  + 1;
        const F32 floatValue  = config.floatValue / 2  + 1;
        const bool boolValue  = !config.boolValue;

        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file2, Path::Type::None );
            stream << "stringValue = \"" << strContent << "\"\n\n"
                   << "intValue = " << intContent << "\n\n"
                   << "floatValue = " << String::To( floatValue ) << "\n\n"
                   << "boolValue = " << ( boolValue ? "true" : "false" );
        }

        EXPECT_TRUE( config.Load( file, "testConfig", &LogErrors ) );

        EXPECT_EQ( ConfigurationFile::ParseError::None, config.SetString( "stringValue", strContent ) );
        EXPECT_EQ( ConfigurationFile::ParseError::None, config.SetInt( "intValue", intContent ) );
        EXPECT_EQ( ConfigurationFile::ParseError::None, config.SetFloat( "floatValue", floatValue ) );
        EXPECT_EQ( ConfigurationFile::ParseError::None, config.SetBool( "boolValue", boolValue ) );

        EXPECT_TRUE( File::AreEqual( file, file2 ) );

        //! [NewValues]

        EXPECT_EQ( ConfigurationFile::ParseError::None, gError );

        ResetLogErrors();
        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, NewValues, NoChange )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();
        const std::string file2 = root + Path::GetUniqueFileName();

        TestConfiguration config;

        const std::string strContent = config.stringValue.substr( 0, config.stringValue.size() / 2 );
        const S32 intContent  = config.intValue / 2  + 1;
        const F32 floatValue  = config.floatValue / 2  + 1;
        const bool boolValue  = !config.boolValue;

        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file2, Path::Type::None );
            stream << "stringValue = \"" << config.stringValue << "\"\n\n"
                   << "intValue = " << config.intValue << "\n\n"
                   << "floatValue = " << String::To( config.floatValue ) << "\n\n"
                   << "boolValue = " << ( config.boolValue ? "true" : "false" );
        }

        EXPECT_TRUE( config.Load( file, "testConfig", &LogErrors ) );

        EXPECT_EQ( ConfigurationFile::ParseError::None, config.SetString( "stringValue", strContent, false ) );
        EXPECT_EQ( ConfigurationFile::ParseError::None, config.SetInt( "intValue", intContent, false ) );
        EXPECT_EQ( ConfigurationFile::ParseError::None, config.SetFloat( "floatValue", floatValue, false ) );
        EXPECT_EQ( ConfigurationFile::ParseError::None, config.SetBool( "boolValue", boolValue, false ) );

        EXPECT_TRUE( File::AreEqual( file, file2 ) );

        EXPECT_EQ( ConfigurationFile::ParseError::None, gError );

        ResetLogErrors();
        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, SetValue, NoInit )
    {
        TestConfiguration config;
        EXPECT_EQ( ConfigurationFile::ParseError::FailedToOpenFile, config.SetString( "stringValue",
                                                                                      ::Test::GenerateRandomString() ) );
        EXPECT_EQ( ConfigurationFile::ParseError::FailedToOpenFile, config.SetInt( "intValue",
                                                                                   ::Test::GenerateRandomS32() ) );
        EXPECT_EQ( ConfigurationFile::ParseError::FailedToOpenFile, config.SetFloat( "floatValue",
                                                                                     ::Test::GenerateRandomF32() ) );
        EXPECT_EQ( ConfigurationFile::ParseError::FailedToOpenFile, config.SetBool( "boolValue",
                                                                                    ::Test::GenerateRandomBool() ) );
    }

    ENGINE_TEST( ConfigurationFile, SetValue, NotExistingString )
    {
        ConfigurationFile config;
        EXPECT_EQ( ConfigurationFile::ParseError::InvalidKey, config.SetString( "key", ::Test::GenerateRandomString() ) );
    }

    ENGINE_TEST( ConfigurationFile, SetValue, NotExistingBool )
    {
        ConfigurationFile config;

        EXPECT_EQ( ConfigurationFile::ParseError::InvalidKey, config.SetBool( "key", ::Test::GenerateRandomBool() ) );
    }

    ENGINE_TEST( ConfigurationFile, SetValue, NotExistingFloat )
    {
        ConfigurationFile config;

        EXPECT_EQ( ConfigurationFile::ParseError::InvalidKey, config.SetFloat( "key", ::Test::GenerateRandomF32() ) );
    }

    ENGINE_TEST( ConfigurationFile, SetValue, NotExistingInt )
    {
        ConfigurationFile config;

        EXPECT_EQ( ConfigurationFile::ParseError::InvalidKey, config.SetInt( "key", ::Test::GenerateRandomS32() ) );
    }

    ENGINE_TEST( ConfigurationFile, SetEmpty )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file, Path::Type::None );
            stream << "";
        }

        const std::string value = ::Test::GenerateRandomString();

        ConfigurationFile config;
        EXPECT_TRUE( config.Load( file, "testConfig" ) );

        EXPECT_FALSE( config.HasKey( "key" ) );
        EXPECT_TRUE( config.AddStringKey( "key", value ) );
        EXPECT_EQ( ConfigurationFile::ParseError::None, config.SetString( "key", value + "$" ) );

        EXPECT_EQ( value + "$", config.GetString( "key" ) );
        EXPECT_TRUE( config.HasKey( "key" ) );
        EXPECT_TRUE( config.IsStringKey( "key" ) );

        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, AddKey, String )
    {
        //! [AddStringKey]
        const std::string value = ::Test::GenerateRandomString();

        ConfigurationFile config;

        EXPECT_FALSE( config.HasKey( "key" ) );
        EXPECT_TRUE( config.AddStringKey( "key", value ) );
        EXPECT_EQ( value, config.GetString( "key" ) );
        EXPECT_TRUE( config.HasKey( "key" ) );
        EXPECT_TRUE( config.IsStringKey( "key" ) );
        //! [AddStringKey]
    }

    ENGINE_TEST( ConfigurationFile, AddKey, Bool )
    {
        //! [AddBoolKey]
        const bool value = ::Test::GenerateRandomBool();

        ConfigurationFile config;

        EXPECT_FALSE( config.HasKey( "key" ) );
        EXPECT_TRUE( config.AddBoolKey( "key", value ) );
        EXPECT_EQ( value, config.GetBool( "key" ) );
        EXPECT_TRUE( config.HasKey( "key" ) );
        EXPECT_TRUE( config.IsBoolKey( "key" ) );
        //! [AddBoolKey]
    }

    ENGINE_TEST( ConfigurationFile, AddKey, Float )
    {
        //! [AddFloatKey]
        const F32 value = ::Test::GenerateRandomF32();

        ConfigurationFile config;

        EXPECT_FALSE( config.HasKey( "key" ) );
        EXPECT_TRUE( config.AddFloatKey( "key", value ) );
        EXPECT_FLOAT_EQ( value, config.GetFloat( "key" ) );
        EXPECT_TRUE( config.HasKey( "key" ) );
        EXPECT_TRUE( config.IsFloatKey( "key" ) );
        //! [AddFloatKey]
    }

    ENGINE_TEST( ConfigurationFile, AddKey, Int )
    {
        //! [AddIntKey]
        const S32 value = ::Test::GenerateRandomS32();

        ConfigurationFile config;

        EXPECT_FALSE( config.HasKey( "key" ) );
        EXPECT_TRUE( config.AddIntKey( "key", value ) );
        EXPECT_EQ( value, config.GetInt( "key" ) );
        EXPECT_TRUE( config.HasKey( "key" ) );
        EXPECT_TRUE( config.IsIntKey( "key" ) );
        //! [AddIntKey]
    }

    ENGINE_TEST( ConfigurationFile, AddEmpty, String )
    {
        const std::string value = ::Test::GenerateRandomString();

        ConfigurationFile config;

        EXPECT_FALSE( config.HasKey( "" ) );
        EXPECT_FALSE( config.AddStringKey( "", value ) );
        EXPECT_EQ( "", config.GetString( "" ) );
        EXPECT_FALSE( config.HasKey( "" ) );
    }

    ENGINE_TEST( ConfigurationFile, AddEmpty, Bool )
    {
        const bool value = ::Test::GenerateRandomBool();

        ConfigurationFile config;

        EXPECT_FALSE( config.HasKey( "" ) );
        EXPECT_FALSE( config.AddBoolKey( "", value ) );
        EXPECT_EQ( false, config.GetBool( "" ) );
        EXPECT_FALSE( config.HasKey( "" ) );
    }

    ENGINE_TEST( ConfigurationFile, AddEmpty, Float )
    {
        const F32 value = ::Test::GenerateRandomF32();

        ConfigurationFile config;

        EXPECT_FALSE( config.HasKey( "" ) );
        EXPECT_FALSE( config.AddFloatKey( "", value ) );
        EXPECT_FLOAT_EQ( 0.0f, config.GetFloat( "" ) );
        EXPECT_FALSE( config.HasKey( "" ) );
    }

    ENGINE_TEST( ConfigurationFile, AddEmpty, Int )
    {
        const S32 value = ::Test::GenerateRandomS32();

        ConfigurationFile config;

        EXPECT_FALSE( config.HasKey( "" ) );
        EXPECT_FALSE( config.AddIntKey( "", value ) );
        EXPECT_EQ( 0u, config.GetInt( "" ) );
        EXPECT_FALSE( config.HasKey( "" ) );
    }


    ENGINE_TEST( ConfigurationFile, GetNotExisting, StringKey )
    {
        ConfigurationFile config;

        EXPECT_EQ( "", config.GetString( "key" ) );
    }

    ENGINE_TEST( ConfigurationFile, GetNotExisting, BoolKey )
    {
        ConfigurationFile config;

        EXPECT_FALSE( config.GetBool( "key" ) );
    }

    ENGINE_TEST( ConfigurationFile, GetNotExisting, FloatKey )
    {
        ConfigurationFile config;

        EXPECT_FLOAT_EQ( 0.0f, config.GetFloat( "key" ) );
    }

    ENGINE_TEST( ConfigurationFile, GetNotExisting, IntKey )
    {
        ConfigurationFile config;

        EXPECT_EQ( 0, config.GetInt( "key" ) );
    }

    ENGINE_TEST( ConfigurationFile, FirstDefinedStays, Bool )
    {
        ConfigurationFile config;

        EXPECT_TRUE( config.AddBoolKey( "key", true ) );
        EXPECT_FALSE( config.AddStringKey( "key", "true" ) );

        EXPECT_TRUE( config.IsBoolKey( "key" ) );
        EXPECT_FALSE( config.IsStringKey( "key" ) );
    }

    ENGINE_TEST( ConfigurationFile, FirstDefinedStays, String )
    {
        ConfigurationFile config;

        EXPECT_TRUE( config.AddStringKey( "key", "true" ) );
        EXPECT_FALSE( config.AddBoolKey( "key", true ) );

        EXPECT_TRUE( config.IsStringKey( "key" ) );
        EXPECT_FALSE( config.IsBoolKey( "key" ) );
    }

    ENGINE_TEST( ConfigurationFile, FirstDefinedStays, Float )
    {
        ConfigurationFile config;

        EXPECT_TRUE( config.AddFloatKey( "key", 0.0f ) );
        EXPECT_FALSE( config.AddIntKey( "key", 0 ) );

        EXPECT_TRUE( config.IsFloatKey( "key" ) );
        EXPECT_FALSE( config.IsIntKey( "key" ) );
    }

    ENGINE_TEST( ConfigurationFile, FirstDefinedStays, Int )
    {
        ConfigurationFile config;

        EXPECT_TRUE( config.AddIntKey( "key", 0 ) );
        EXPECT_FALSE( config.AddFloatKey( "key", 0.0f ) );

        EXPECT_TRUE( config.IsIntKey( "key" ) );
        EXPECT_FALSE( config.IsFloatKey( "key" ) );
    }

    ENGINE_TEST( ConfigurationFile, Error, NoSeperatorNoCallback )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        TestConfiguration config;

        const std::string invalidLine = "invalid \"" + ::Test::GenerateRandomString() + "\"";
        const std::string strContent = config.stringValue.substr( 0, config.stringValue.size() / 2 );
        const S32 intContent = config.intValue / 2 + 1;
        const F32 floatValue = config.floatValue / 2 + 1;
        const bool boolValue = !config.boolValue;

        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file, Path::Type::None );
            stream << invalidLine << "\n"
                   << "stringValue = \"" << strContent << "\"\n"
                   << "intValue = " << intContent << "\n"
                   << "floatValue = " << String::To( floatValue ) << "\n"
                   << "boolValue = " << ( boolValue ? "true" : "false" );
        }

        EXPECT_TRUE( config.Load( file, "testConfig" ) );

        // We continued parsing :)
        EXPECT_EQ( strContent, config.GetString( "stringValue" ) );
        EXPECT_EQ( intContent, config.GetInt( "intValue" ) );
        EXPECT_FLOAT_EQ( floatValue, config.GetFloat( "floatValue" ) );
        EXPECT_EQ( boolValue, config.GetBool( "boolValue" ) );

        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, Error, NoSeperator )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        TestConfiguration config;

        const std::string invalidLine = "invalid \"" + ::Test::GenerateRandomString() + "\"";
        const std::string strContent = config.stringValue.substr( 0, config.stringValue.size() / 2 );
        const S32 intContent = config.intValue / 2 + 1;
        const F32 floatValue = config.floatValue / 2 + 1;
        const bool boolValue = !config.boolValue;

        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file, Path::Type::None );
            stream << invalidLine << "\n"
                   << "stringValue = \"" << strContent << "\"\n"
                   << "intValue = " << intContent << "\n"
                   << "floatValue = " << String::To( floatValue ) << "\n"
                   << "boolValue = " << ( boolValue ? "true" : "false" );
        }

        EXPECT_TRUE( config.Load( file, "testConfig", &LogErrors ) );

        // We continued parsing :)
        EXPECT_EQ( strContent, config.GetString( "stringValue" ) );
        EXPECT_EQ( intContent, config.GetInt( "intValue" ) );
        EXPECT_FLOAT_EQ( floatValue, config.GetFloat( "floatValue" ) );
        EXPECT_EQ( boolValue, config.GetBool( "boolValue" ) );

        EXPECT_EQ( ConfigurationFile::ParseError::NoSeperator, gError );
        EXPECT_EQ( "testConfig", gConfigName );
        EXPECT_EQ( invalidLine, gKey );
        EXPECT_EQ( 1, gLineNumber );
        EXPECT_EQ( "", gValue );

        ResetLogErrors();
        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, Error, NotRegisteredNoCallback )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        TestConfiguration config;

        const std::string invalidLine = "\"" + ::Test::GenerateRandomString() + "\"";
        const std::string strContent = config.stringValue.substr( 0, config.stringValue.size() / 2 );
        const S32 intContent = config.intValue / 2 + 1;
        const F32 floatValue = config.floatValue / 2 + 1;
        const bool boolValue = !config.boolValue;

        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file, Path::Type::None );
            stream << "isNotRegistered = " << invalidLine << "\n"
                   << "stringValue = \"" << strContent << "\"\n"
                   << "intValue = " << intContent << "\n"
                   << "floatValue = " << String::To( floatValue ) << "\n"
                   << "boolValue = " << ( boolValue ? "true" : "false" );
        }

        EXPECT_TRUE( config.Load( file, "testConfig" ) );

        // We continued parsing :)
        EXPECT_EQ( strContent, config.GetString( "stringValue" ) );
        EXPECT_EQ( intContent, config.GetInt( "intValue" ) );
        EXPECT_FLOAT_EQ( floatValue, config.GetFloat( "floatValue" ) );
        EXPECT_EQ( boolValue, config.GetBool( "boolValue" ) );

        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, Error, NotRegistered )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        TestConfiguration config;

        const std::string invalidLine = "\"" + ::Test::GenerateRandomString() + "\"";
        const std::string strContent = config.stringValue.substr( 0, config.stringValue.size() / 2 );
        const S32 intContent = config.intValue / 2 + 1;
        const F32 floatValue = config.floatValue / 2 + 1;
        const bool boolValue = !config.boolValue;

        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file, Path::Type::None );
            stream << "isNotRegistered = " << invalidLine << "\n"
                   << "stringValue = \"" << strContent << "\"\n"
                   << "intValue = " << intContent << "\n"
                   << "floatValue = " << String::To( floatValue ) << "\n"
                   << "boolValue = " << ( boolValue ? "true" : "false" );
        }

        EXPECT_TRUE( config.Load( file, "testConfig", &LogErrors ) );

        // We continued parsing :)
        EXPECT_EQ( strContent, config.GetString( "stringValue" ) );
        EXPECT_EQ( intContent, config.GetInt( "intValue" ) );
        EXPECT_FLOAT_EQ( floatValue, config.GetFloat( "floatValue" ) );
        EXPECT_EQ( boolValue, config.GetBool( "boolValue" ) );

        EXPECT_EQ( ConfigurationFile::ParseError::NotRegistered, gError );
        EXPECT_EQ( "testConfig", gConfigName );
        EXPECT_EQ( "isNotRegistered", gKey );
        EXPECT_EQ( 1, gLineNumber );
        EXPECT_EQ( invalidLine, gValue );

        ResetLogErrors();
        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, Error, IsDuplicateKeyNoCallback )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        TestConfiguration config;

        const std::string invalidLine = "\"" + ::Test::GenerateRandomString() + "\"";
        const std::string strContent = config.stringValue.substr( 0, config.stringValue.size() / 2 );
        const S32 intContent = config.intValue / 2 + 1;
        const F32 floatValue = config.floatValue / 2 + 1;
        const bool boolValue = !config.boolValue;

        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file, Path::Type::None );
            stream << "stringValue = \"" << strContent << "\"\n"
                   << "stringValue = " << invalidLine << "\n"
                   << "intValue = " << intContent << "\n"
                   << "floatValue = " << String::To( floatValue ) << "\n"
                   << "boolValue = " << ( boolValue ? "true" : "false" );
        }

        EXPECT_TRUE( config.Load( file, "testConfig" ) );

        // We continued parsing :)
        EXPECT_EQ( strContent, config.GetString( "stringValue" ) );
        EXPECT_EQ( intContent, config.GetInt( "intValue" ) );
        EXPECT_FLOAT_EQ( floatValue, config.GetFloat( "floatValue" ) );
        EXPECT_EQ( boolValue, config.GetBool( "boolValue" ) );

        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, Error, IsDuplicateKey )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        TestConfiguration config;

        const std::string invalidLine = "\"" + ::Test::GenerateRandomString() + "\"";
        const std::string strContent = config.stringValue.substr( 0, config.stringValue.size() / 2 );
        const S32 intContent = config.intValue / 2 + 1;
        const F32 floatValue = config.floatValue / 2 + 1;
        const bool boolValue = !config.boolValue;

        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file, Path::Type::None );
            stream << "stringValue = \"" << strContent << "\"\n"
                   << "stringValue = " << invalidLine << "\n"
                   << "intValue = " << intContent << "\n"
                   << "floatValue = " << String::To( floatValue ) << "\n"
                   << "boolValue = " << ( boolValue ? "true" : "false" );
        }

        EXPECT_TRUE( config.Load( file, "testConfig", &LogErrors ) );

        // We continued parsing :)
        EXPECT_EQ( strContent, config.GetString( "stringValue" ) );
        EXPECT_EQ( intContent, config.GetInt( "intValue" ) );
        EXPECT_FLOAT_EQ( floatValue, config.GetFloat( "floatValue" ) );
        EXPECT_EQ( boolValue, config.GetBool( "boolValue" ) );

        EXPECT_EQ( ConfigurationFile::ParseError::IsDuplicateKey, gError );
        EXPECT_EQ( "testConfig", gConfigName );
        EXPECT_EQ( "stringValue", gKey );
        EXPECT_EQ( 2, gLineNumber );
        EXPECT_EQ( invalidLine, gValue );

        ResetLogErrors();
        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, Error, NoValueNoCallback )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        TestConfiguration config;
        ASSERT_TRUE( config.AddStringKey( "invalidKey", "" ) );

        const std::string strContent = config.stringValue.substr( 0, config.stringValue.size() / 2 );
        const S32 intContent = config.intValue / 2 + 1;
        const F32 floatValue = config.floatValue / 2 + 1;
        const bool boolValue = !config.boolValue;

        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file, Path::Type::None );
            stream << "invalidKey = \n"
                   << "stringValue = \"" << strContent << "\"\n"
                   << "intValue = " << intContent << "\n"
                   << "floatValue = " << String::To( floatValue ) << "\n"
                   << "boolValue = " << ( boolValue ? "true" : "false" );
        }

        EXPECT_TRUE( config.Load( file, "testConfig" ) );

        // We continued parsing :)
        EXPECT_EQ( strContent, config.GetString( "stringValue" ) );
        EXPECT_EQ( intContent, config.GetInt( "intValue" ) );
        EXPECT_FLOAT_EQ( floatValue, config.GetFloat( "floatValue" ) );
        EXPECT_EQ( boolValue, config.GetBool( "boolValue" ) );

        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, Error, NoValue )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        TestConfiguration config;
        ASSERT_TRUE( config.AddStringKey( "invalidKey", "" ) );

        const std::string strContent = config.stringValue.substr( 0, config.stringValue.size() / 2 );
        const S32 intContent = config.intValue / 2 + 1;
        const F32 floatValue = config.floatValue / 2 + 1;
        const bool boolValue = !config.boolValue;

        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file, Path::Type::None );
            stream << "invalidKey = \n"
                   << "stringValue = \"" << strContent << "\"\n"
                   << "intValue = " << intContent << "\n"
                   << "floatValue = " << String::To( floatValue ) << "\n"
                   << "boolValue = " << ( boolValue ? "true" : "false" );
        }

        EXPECT_TRUE( config.Load( file, "testConfig", &LogErrors ) );

        // We continued parsing :)
        EXPECT_EQ( strContent, config.GetString( "stringValue" ) );
        EXPECT_EQ( intContent, config.GetInt( "intValue" ) );
        EXPECT_FLOAT_EQ( floatValue, config.GetFloat( "floatValue" ) );
        EXPECT_EQ( boolValue, config.GetBool( "boolValue" ) );

        EXPECT_EQ( ConfigurationFile::ParseError::NoValue, gError );
        EXPECT_EQ( "testConfig", gConfigName );
        EXPECT_EQ( "invalidKey", gKey );
        EXPECT_EQ( 1, gLineNumber );
        EXPECT_EQ( "", gValue );

        ResetLogErrors();
        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, Error, NoKeyNoCallback )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        TestConfiguration config;

        const std::string invalidLine = "\"" + ::Test::GenerateRandomString() + "\"";
        const std::string strContent = config.stringValue.substr( 0, config.stringValue.size() / 2 );
        const S32 intContent = config.intValue / 2 + 1;
        const F32 floatValue = config.floatValue / 2 + 1;
        const bool boolValue = !config.boolValue;

        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file, Path::Type::None );
            stream << " = " << invalidLine << "\n"
                   << "stringValue = \"" << strContent << "\"\n"
                   << "intValue = " << intContent << "\n"
                   << "floatValue = " << String::To( floatValue ) << "\n"
                   << "boolValue = " << ( boolValue ? "true" : "false" );
        }

        EXPECT_TRUE( config.Load( file, "testConfig" ) );

        // We continued parsing :)
        EXPECT_EQ( strContent, config.GetString( "stringValue" ) );
        EXPECT_EQ( intContent, config.GetInt( "intValue" ) );
        EXPECT_FLOAT_EQ( floatValue, config.GetFloat( "floatValue" ) );
        EXPECT_EQ( boolValue, config.GetBool( "boolValue" ) );

        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, Error, NoKey )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        TestConfiguration config;

        const std::string invalidLine = "\"" + ::Test::GenerateRandomString() + "\"";
        const std::string strContent = config.stringValue.substr( 0, config.stringValue.size() / 2 );
        const S32 intContent = config.intValue / 2 + 1;
        const F32 floatValue = config.floatValue / 2 + 1;
        const bool boolValue = !config.boolValue;

        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file, Path::Type::None );
            stream << " = " << invalidLine << "\n"
                   << "stringValue = \"" << strContent << "\"\n"
                   << "intValue = " << intContent << "\n"
                   << "floatValue = " << String::To( floatValue ) << "\n"
                   << "boolValue = " << ( boolValue ? "true" : "false" );
        }

        EXPECT_TRUE( config.Load( file, "testConfig", &LogErrors ) );

        // We continued parsing :)
        EXPECT_EQ( strContent, config.GetString( "stringValue" ) );
        EXPECT_EQ( intContent, config.GetInt( "intValue" ) );
        EXPECT_FLOAT_EQ( floatValue, config.GetFloat( "floatValue" ) );
        EXPECT_EQ( boolValue, config.GetBool( "boolValue" ) );

        EXPECT_EQ( ConfigurationFile::ParseError::NoKey, gError );
        EXPECT_EQ( "testConfig", gConfigName );
        EXPECT_EQ( "", gKey );
        EXPECT_EQ( 1, gLineNumber );
        EXPECT_EQ( invalidLine, gValue );

        ResetLogErrors();
        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, Error, InvalidStringNoCallback )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        TestConfiguration config;
        ASSERT_TRUE( config.AddStringKey( "invalidKey", "" ) );

        const std::string invalidLine = "$\"" + ::Test::GenerateRandomString() + "\"";
        const std::string strContent = config.stringValue.substr( 0, config.stringValue.size() / 2 );
        const S32 intContent = config.intValue / 2 + 1;
        const F32 floatValue = config.floatValue / 2 + 1;
        const bool boolValue = !config.boolValue;

        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file, Path::Type::None );
            stream << "invalidKey = " << invalidLine << "\n"
                   << "stringValue = \"" << strContent << "\"\n"
                   << "intValue = " << intContent << "\n"
                   << "floatValue = " << String::To( floatValue ) << "\n"
                   << "boolValue = " << ( boolValue ? "true" : "false" );
        }

        EXPECT_FALSE( config.Load( file, "testConfig" ) );

        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, Error, InvalidString )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        TestConfiguration config;
        ASSERT_TRUE( config.AddStringKey( "invalidKey", "" ) );

        const std::string invalidLine = "$\"" + ::Test::GenerateRandomString() + "\"";
        const std::string strContent = config.stringValue.substr( 0, config.stringValue.size() / 2 );
        const S32 intContent = config.intValue / 2 + 1;
        const F32 floatValue = config.floatValue / 2 + 1;
        const bool boolValue = !config.boolValue;

        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file, Path::Type::None );
            stream << "invalidKey = " << invalidLine << "\n"
                   << "stringValue = \"" << strContent << "\"\n"
                   << "intValue = " << intContent << "\n"
                   << "floatValue = " << String::To( floatValue ) << "\n"
                   << "boolValue = " << ( boolValue ? "true" : "false" );
        }

        EXPECT_FALSE( config.Load( file, "testConfig", &LogErrors ) );

        EXPECT_EQ( ConfigurationFile::ParseError::InvalidString, gError );
        EXPECT_EQ( "testConfig", gConfigName );
        EXPECT_EQ( "invalidKey", gKey );
        EXPECT_EQ( 1, gLineNumber );
        EXPECT_EQ( invalidLine, gValue );

        ResetLogErrors();
        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, Error, InvalidString2 )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        TestConfiguration config;
        ASSERT_TRUE( config.AddStringKey( "invalidKey", "" ) );

        const std::string invalidLine = "\"" + ::Test::GenerateRandomString() + "\'";
        const std::string strContent = config.stringValue.substr( 0, config.stringValue.size() / 2 );
        const S32 intContent = config.intValue / 2 + 1;
        const F32 floatValue = config.floatValue / 2 + 1;
        const bool boolValue = !config.boolValue;

        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file, Path::Type::None );
            stream << "invalidKey = " << invalidLine << "\n"
                   << "stringValue = \"" << strContent << "\"\n"
                   << "intValue = " << intContent << "\n"
                   << "floatValue = " << String::To( floatValue ) << "\n"
                   << "boolValue = " << ( boolValue ? "true" : "false" );
        }

        EXPECT_FALSE( config.Load( file, "testConfig", &LogErrors ) );

        EXPECT_EQ( ConfigurationFile::ParseError::InvalidString, gError );
        EXPECT_EQ( "testConfig", gConfigName );
        EXPECT_EQ( "invalidKey", gKey );
        EXPECT_EQ( 1, gLineNumber );
        EXPECT_EQ( invalidLine, gValue );

        ResetLogErrors();
        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, Error, InvalidString3 )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        TestConfiguration config;
        ASSERT_TRUE( config.AddStringKey( "invalidKey", "" ) );

        const std::string invalidLine = "\'" + ::Test::GenerateRandomString() + "\"";
        const std::string strContent = config.stringValue.substr( 0, config.stringValue.size() / 2 );
        const S32 intContent = config.intValue / 2 + 1;
        const F32 floatValue = config.floatValue / 2 + 1;
        const bool boolValue = !config.boolValue;

        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file, Path::Type::None );
            stream << "invalidKey = " << invalidLine << "\n"
                   << "stringValue = \"" << strContent << "\"\n"
                   << "intValue = " << intContent << "\n"
                   << "floatValue = " << String::To( floatValue ) << "\n"
                   << "boolValue = " << ( boolValue ? "true" : "false" );
        }

        EXPECT_FALSE( config.Load( file, "testConfig", &LogErrors ) );

        EXPECT_EQ( ConfigurationFile::ParseError::InvalidString, gError );
        EXPECT_EQ( "testConfig", gConfigName );
        EXPECT_EQ( "invalidKey", gKey );
        EXPECT_EQ( 1, gLineNumber );
        EXPECT_EQ( invalidLine, gValue );

        ResetLogErrors();
        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, Error, InvalidBoolNoCallback )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        TestConfiguration config;
        ASSERT_TRUE( config.AddBoolKey( "invalidKey", false ) );

        const std::string strContent = config.stringValue.substr( 0, config.stringValue.size() / 2 );
        const S32 intContent = config.intValue / 2 + 1;
        const F32 floatValue = config.floatValue / 2 + 1;
        const bool boolValue = !config.boolValue;
        const std::string invalidValue = ( boolValue ? "$trueorsomething" : "@falseorsomething" );

        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file, Path::Type::None );
            stream << "invalidKey = " << invalidValue << "\n"
                   << "stringValue = \"" << strContent << "\"\n"
                   << "intValue = " << intContent << "\n"
                   << "floatValue = " << String::To( floatValue ) << "\n"
                   << "boolValue = " << ( boolValue ? "true" : "false" );
        }

        EXPECT_FALSE( config.Load( file, "testConfig" ) );

        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, Error, InvalidBool )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        TestConfiguration config;
        ASSERT_TRUE( config.AddBoolKey( "invalidKey", false ) );

        const std::string strContent = config.stringValue.substr( 0, config.stringValue.size() / 2 );
        const S32 intContent = config.intValue / 2 + 1;
        const F32 floatValue = config.floatValue / 2 + 1;
        const bool boolValue = !config.boolValue;
        const std::string invalidValue = ( boolValue ? "$trueorsomething" : "@falseorsomething" );

        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file, Path::Type::None );
            stream << "invalidKey = " << invalidValue << "\n"
                   << "stringValue = \"" << strContent << "\"\n"
                   << "intValue = " << intContent << "\n"
                   << "floatValue = " << String::To( floatValue ) << "\n"
                   << "boolValue = " << ( boolValue ? "true" : "false" );
        }

        EXPECT_FALSE( config.Load( file, "testConfig", &LogErrors ) );

        EXPECT_EQ( ConfigurationFile::ParseError::InvalidBool, gError );
        EXPECT_EQ( "testConfig", gConfigName );
        EXPECT_EQ( "invalidKey", gKey );
        EXPECT_EQ( 1, gLineNumber );
        EXPECT_EQ( invalidValue, gValue );

        ResetLogErrors();
        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, Error, InvalidFloatNoCallback )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        TestConfiguration config;
        ASSERT_TRUE( config.AddFloatKey( "invalidKey", 0.0f ) );

        const std::string strContent = config.stringValue.substr( 0, config.stringValue.size() / 2 );
        const S32 intContent = config.intValue / 2 + 1;
        const F32 floatValue = config.floatValue / 2 + 1;
        const bool boolValue = !config.boolValue;
        const std::string invalidValue = "$" + String::To( floatValue );

        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file, Path::Type::None );
            stream << "invalidKey = " << invalidValue << "\n"
                   << "stringValue = \"" << strContent << "\"\n"
                   << "intValue = " << intContent << "\n"
                   << "floatValue = " << String::To( floatValue ) << "\n"
                   << "boolValue = " << ( boolValue ? "true" : "false" );
        }

        EXPECT_FALSE( config.Load( file, "testConfig" ) );

        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, Error, InvalidFloat )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        TestConfiguration config;
        ASSERT_TRUE( config.AddFloatKey( "invalidKey", 0.0f ) );

        const std::string strContent = config.stringValue.substr( 0, config.stringValue.size() / 2 );
        const S32 intContent = config.intValue / 2 + 1;
        const F32 floatValue = config.floatValue / 2 + 1;
        const bool boolValue = !config.boolValue;
        const std::string invalidValue = "$" + String::To( floatValue );

        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file, Path::Type::None );
            stream << "invalidKey = " << invalidValue << "\n"
                   << "stringValue = \"" << strContent << "\"\n"
                   << "intValue = " << intContent << "\n"
                   << "floatValue = " << String::To( floatValue ) << "\n"
                   << "boolValue = " << ( boolValue ? "true" : "false" );
        }

        EXPECT_FALSE( config.Load( file, "testConfig", &LogErrors ) );

        EXPECT_EQ( ConfigurationFile::ParseError::InvalidFloat, gError );
        EXPECT_EQ( "testConfig", gConfigName );
        EXPECT_EQ( "invalidKey", gKey );
        EXPECT_EQ( 1, gLineNumber );
        EXPECT_EQ( invalidValue, gValue );

        ResetLogErrors();
        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, Error, InvalidIntNoCallback )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        TestConfiguration config;
        ASSERT_TRUE( config.AddIntKey( "invalidKey", 0 ) );

        const std::string strContent = config.stringValue.substr( 0, config.stringValue.size() / 2 );
        const S32 intContent = config.intValue / 2 + 1;
        const F32 floatValue = config.floatValue / 2 + 1;
        const bool boolValue = !config.boolValue;
        const std::string invalidValue = "$" + String::To( intContent );

        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file, Path::Type::None );
            stream << "invalidKey = " << invalidValue << "\n"
                   << "stringValue = \"" << strContent << "\"\n"
                   << "intValue = " << intContent << "\n"
                   << "floatValue = " << String::To( floatValue ) << "\n"
                   << "boolValue = " << ( boolValue ? "true" : "false" );
        }

        EXPECT_FALSE( config.Load( file, "testConfig" ) );

        ::Test::CleanUp( root );
    }

    ENGINE_TEST( ConfigurationFile, Error, InvalidInt )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        TestConfiguration config;
        ASSERT_TRUE( config.AddIntKey( "invalidKey", 0 ) );

        const std::string strContent = config.stringValue.substr( 0, config.stringValue.size() / 2 );
        const S32 intContent = config.intValue / 2 + 1;
        const F32 floatValue = config.floatValue / 2 + 1;
        const bool boolValue = !config.boolValue;
        const std::string invalidValue = "$" + String::To( intContent );

        {
            ResetLogErrors();

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream stream;
            File::WriteOpen( stream, file, Path::Type::None );
            stream << "invalidKey = " << invalidValue << "\n"
                   << "stringValue = \"" << strContent << "\"\n"
                   << "intValue = " << intContent << "\n"
                   << "floatValue = " << String::To( floatValue ) << "\n"
                   << "boolValue = " << ( boolValue ? "true" : "false" );
        }

        EXPECT_FALSE( config.Load( file, "testConfig", &LogErrors ) );

        EXPECT_EQ( ConfigurationFile::ParseError::InvalidInt, gError );
        EXPECT_EQ( "testConfig", gConfigName );
        EXPECT_EQ( "invalidKey", gKey );
        EXPECT_EQ( 1, gLineNumber );
        EXPECT_EQ( invalidValue, gValue );

        ResetLogErrors();
        ::Test::CleanUp( root );
    }

}