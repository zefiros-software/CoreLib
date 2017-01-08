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

#include "common/file.h"

#include "engineTest.h"


namespace
{

    TEST( File, ReadOpen, NotExisting )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        {
            ASSERT_FALSE( File::Exists( file ) );
        }

        std::ifstream stream;
        File::ReadOpen( stream, file, Path::Type::None );
        EXPECT_FALSE( stream.is_open() );

        ::Test::CleanUp( root );
    }

    TEST( File, ReadOpen, Exists )
    {
        //! [ReadOpen]
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();
        const std::string content = ::Test::GenerateRandomString();

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file, content );
        }

        std::string readContent;
        std::ifstream stream;
        File::ReadOpen( stream, file, Path::Type::None );

        EXPECT_TRUE( stream.is_open() );

        std::getline( stream, readContent );

        EXPECT_EQ( content, readContent );
        //! [ReadOpen]

        stream.close();

        ::Test::CleanUp( root );
    }

    TEST( File, WriteOpen, NotExisting )
    {
        //! [WriteOpen]
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();
        const std::string content = ::Test::GenerateRandomString();

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ASSERT_FALSE( File::Exists( file ) );
        }

        std::ofstream stream;
        File::WriteOpen( stream, file, Path::Type::None );
        EXPECT_TRUE( stream.is_open() );

        stream << content;
        //! [WriteOpen]

        stream.close();

        EXPECT_EQ( content, File::ReadAllText( file ) );

        ::Test::CleanUp( root );
    }

    TEST( File, WriteOpen, Exists )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file );
        }

        std::ofstream stream;
        File::WriteOpen( stream, file, Path::Type::None );
        EXPECT_TRUE( stream.is_open() );
        stream.close();

        ::Test::CleanUp( root );
    }

    TEST( File, Open, ReadExists )
    {
        //! [Open]
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();
        const std::string content = ::Test::GenerateRandomString();

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file, content );
        }

        std::string readContent;
        std::fstream stream;
        File::Open( stream, file, Path::Type::None );

        EXPECT_TRUE( stream.is_open() );

        std::getline( stream, readContent );

        EXPECT_EQ( content, readContent );
        //! [Open]

        stream.close();

        ::Test::CleanUp( root );
    }

    TEST( File, Open, WriteNotExisting )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();
        const std::string content = ::Test::GenerateRandomString();

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ASSERT_FALSE( File::Exists( file ) );
        }

        std::fstream stream;
        File::Open( stream, file, Path::Type::None );
        stream << content;
        stream.close();

        EXPECT_FALSE( File::Exists( file ) );

        ::Test::CleanUp( root );
    }

    TEST( File, Open, WriteExists )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file );
        }

        std::fstream stream;
        File::Open( stream, file, Path::Type::None );
        EXPECT_TRUE( stream.is_open() );
        stream.close();

        ::Test::CleanUp( root );
    }

    TEST( File, TempGet, CorrectDirectory )
    {
        const std::string temp = File::TempGet();
        EXPECT_EQ( ".", Path::ResolveRelative( temp, Path::GetProgramTempDirectory(), false ) );
    }

    TEST( File, TempGet, IsUnique )
    {
        //! [TempGet]
        const std::string temp = File::TempGet();
        //! [TempGet]

        // Feeling like winning the lottery, eh?!
        EXPECT_NE( temp, File::TempGet() );
    }

    TEST( File, TempOpen, CanWrite )
    {
        //! [TempOpen]
        const std::string content = "successfully written";
        std::fstream stream;
        File::TempOpen( stream );
        EXPECT_TRUE( stream.is_open() );

        stream << content;
        //! [TempOpen]

        stream.close();
    }

    TEST( File, TempOpen, CorrectDirectory )
    {
        //! [TempOpenPath]
        std::string path;
        std::fstream stream;
        File::TempOpen( stream, path );

        EXPECT_TRUE( stream.is_open() );
        //! [TempOpenPath]

        stream.close();

        EXPECT_EQ( ".", Path::ResolveRelative( path, Path::GetProgramTempDirectory(), false ) );
    }

    TEST( File, TempOpen, DidWrite )
    {
        const std::string content = "successfully written";
        std::string path;
        std::fstream stream;
        File::TempOpen( stream, path );
        EXPECT_TRUE( stream.is_open() );

        stream << content;
        stream.close();

        EXPECT_EQ( content, File::ReadAllText( path ) );
    }

    TEST( File, Exists, ValidFile )
    {
        //! [Exists]
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file );
        }

        EXPECT_TRUE( File::Exists( file ) );
        //! [Exists]

        ::Test::CleanUp( root );
    }

    TEST( File, Exists, ValidFileNoExtension )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        std::string file = root + Path::GetUniqueFileName( "" );
        file.pop_back();
        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            std::ofstream fileStr;
            File::WriteOpen( fileStr, file, Path::Type::None );
            fileStr << ::Test::GenerateRandomString();
            fileStr.close();
        }

        EXPECT_TRUE( File::Exists( file ) );

        ::Test::CleanUp( root );
    }

    TEST( File, Exists, InvalidFile )
    {
        const std::string file = Path::GetUniqueFileName();

        EXPECT_FALSE( File::Exists( file ) );
    }

    TEST( File, Exists, Directory )
    {
        std::string root = ::Test::GenerateDirectoryName( "common" );
        EXPECT_FALSE( File::Exists( root ) );
    }

    TEST( File, GetSize, ValidFile )
    {
        //! [GetSize]
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();
        const std::string content = ::Test::GenerateRandomString();

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file, content );
        }

        EXPECT_EQ( content.size(), File::Size( file ) );
        //! [GetSize]

        ::Test::CleanUp( root );
    }

    TEST( File, GetSize, InvalidFile )
    {
        const std::string file = Path::GetUniqueFileName();

        ASSERT_FALSE( File::Exists( file ) );

        EXPECT_EQ( 0, File::Size( file ) );
    }

    TEST( File, IsEmpty, NotExisting )
    {
        const std::string file = Path::GetUniqueFileName();

        ASSERT_FALSE( File::Exists( file ) );

        EXPECT_EQ( 0, File::Size( file ) );
    }

    TEST( File, IsEmpty, Exists )
    {
        //! [IsEmpty]
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file );
        }

        EXPECT_FALSE( File::IsEmpty( file ) );
        //! [IsEmpty]

        ::Test::CleanUp( root );
    }

    TEST( File, IsEmpty, ExistsCleared )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file );

            File::Clear( file );
        }

        EXPECT_TRUE( File::IsEmpty( file ) );

        ::Test::CleanUp( root );
    }

    TEST( File, AreEqual, Self )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file );
        }

        EXPECT_TRUE( File::AreEqual( file, file ) );

        ::Test::CleanUp( root );
    }

    TEST( File, AreEqual, SameFile )
    {
        const std::string root  = ::Test::GenerateDirectoryName( "common" );
        const std::string file  = root + Path::GetUniqueFileName();
        const std::string file2 = root + Path::GetUniqueFileName();
        const std::string content = ::Test::GenerateRandomString();

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file, content );
            ::Test::GenerateRandomFile( file2, content );
        }

        EXPECT_TRUE( File::AreEqual( file, file2 ) );

        ::Test::CleanUp( root );
    }

    TEST( File, AreEqual, OtherFile )
    {
        //! [AreEqual]
        const std::string root  = ::Test::GenerateDirectoryName( "common" );
        const std::string file  = root + Path::GetUniqueFileName();
        const std::string file2 = root + Path::GetUniqueFileName();

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file );
            // We don't want it to fail with P = 1/58 ;)
            ::Test::GenerateRandomFile( file2, "Test string\n or something" );
        }

        EXPECT_FALSE( File::AreEqual( file, file2 ) );
        //! [AreEqual]

        ::Test::CleanUp( root );
    }

    TEST( File, AreEqual, ANotExisting )
    {
        const std::string root  = ::Test::GenerateDirectoryName( "common" );
        const std::string file  = root + Path::GetUniqueFileName();
        const std::string file2 = root + Path::GetUniqueFileName();

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file );
        }

        EXPECT_FALSE( File::AreEqual( file, file2 ) );

        ::Test::CleanUp( root );
    }

    TEST( File, AreEqual, BNotExisting )
    {
        const std::string root  = ::Test::GenerateDirectoryName( "common" );
        const std::string file  = root + Path::GetUniqueFileName();
        const std::string file2 = root + Path::GetUniqueFileName();

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file2 );
        }

        EXPECT_FALSE( File::AreEqual( file, file2 ) );

        ::Test::CleanUp( root );
    }

    TEST( File, AreEqual, ABNotExisting )
    {
        const std::string file  = Path::GetUniqueFileName();
        const std::string file2 = Path::GetUniqueFileName();

        EXPECT_FALSE( File::AreEqual( file, file2 ) );
    }

    TEST( File, IsSame, Self )
    {
        const std::string root  = ::Test::GenerateDirectoryName( "common" );
        const std::string file  = root + Path::GetUniqueFileName();

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file );
        }

        EXPECT_TRUE( File::IsSame( file, file ) );

        ::Test::CleanUp( root );
    }

    TEST( File, IsSame, Self2 )
    {
        //! [IsSame]
        const std::string root  = ::Test::GenerateDirectoryName( "common" );
        const std::string file  = root + Path::GetUniqueFileName();

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file );
        }

        EXPECT_TRUE( File::IsSame( file, "common/../" + file ) );
        //! [IsSame]

        ::Test::CleanUp( root );
    }

    TEST( File, IsSame, Other )
    {
        const std::string root  = ::Test::GenerateDirectoryName( "common" );
        const std::string file  = root + Path::GetUniqueFileName();
        const std::string file2  = root + Path::GetUniqueFileName();

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file );
            ::Test::GenerateRandomFile( file2 );
        }

        EXPECT_FALSE( File::IsSame( file, file2 ) );

        ::Test::CleanUp( root );
    }

    TEST( File, IsSame, ANotExisting )
    {
        const std::string root  = ::Test::GenerateDirectoryName( "common" );
        const std::string file  = root + Path::GetUniqueFileName();
        const std::string file2 = root + Path::GetUniqueFileName();

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file );
        }

        EXPECT_FALSE( File::IsSame( file, file2 ) );

        ::Test::CleanUp( root );
    }

    TEST( File, IsSame, BNotExisting )
    {
        const std::string root  = ::Test::GenerateDirectoryName( "common" );
        const std::string file  = root + Path::GetUniqueFileName();
        const std::string file2 = root + Path::GetUniqueFileName();

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file2 );
        }

        EXPECT_FALSE( File::IsSame( file, file2 ) );

        ::Test::CleanUp( root );
    }

    TEST( File, IsSame, ABNotExisting )
    {
        const std::string file  = Path::GetUniqueFileName();
        const std::string file2 = Path::GetUniqueFileName();

        EXPECT_FALSE( File::IsSame( file, file2 ) );
    }

    TEST( File, ReadAllText, ValidFile )
    {
        //! [ReadAllText]
        const std::string root  = ::Test::GenerateDirectoryName( "common" );
        const std::string file  = root + Path::GetUniqueFileName();
        const std::string content = ::Test::GenerateRandomString();

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file, content );
        }

        EXPECT_EQ( content, File::ReadAllText( file ) );
        //! [ReadAllText]

        ::Test::CleanUp( root );
    }

    TEST( File, ReadAllText, InvalidFile )
    {
        const std::string file  = Path::GetUniqueFileName();

        EXPECT_EQ( "", File::ReadAllText( file ) );
    }

    TEST( File, GetLastModified )
    {
        const std::string root  = ::Test::GenerateDirectoryName( "common" );
        const std::string file  = root + Path::GetUniqueFileName();

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file );
        }

        // Cheers boost unit tests :)

        std::time_t ft = File::GetLastModified( file );

        std::tm *tmp = std::localtime( &ft );
        File::SetLastModified( file, std::mktime( tmp ) );

        EXPECT_EQ( ft, File::GetLastModified( file ) );

        File::SetLastModified( file, ft );

        double time_diff = std::difftime( ft,  File::GetLastModified( file ) );
        EXPECT_TRUE( time_diff >= -60.0 && time_diff <= 60.0 );

        ::Test::CleanUp( root );
    }

    TEST( File, GetLastModified, NotExists )
    {
        const std::string root  = ::Test::GenerateDirectoryName( "common" );
        const std::string file  = root + Path::GetUniqueFileName();

        {
            ASSERT_FALSE( Directory::Exists( root ) );
        }

        EXPECT_EQ( 0, File::GetLastModified( file ) );

        ::Test::CleanUp( root );
    }

    TEST( File, List, NonRecursive )
    {
        //! [List]
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string dirA = root + ::Test::GenerateDirectoryName();
        const std::string dirB = root + ::Test::GenerateDirectoryName();
        const std::string fileA = root + Path::GetUniqueFileName();
        const std::string fileB = root + Path::GetUniqueFileName();
        const std::string fileC = dirA + Path::GetUniqueFileName();

        {
            ASSERT_TRUE( Directory::Create( root ) );

            ASSERT_TRUE( Directory::Exists( root ) );

            ASSERT_TRUE( Directory::Create( dirA ) );
            ASSERT_TRUE( Directory::Create( dirB ) );

            ASSERT_TRUE( Directory::Exists( dirA ) );
            ASSERT_TRUE( Directory::Exists( dirB ) );

            ::Test::GenerateRandomFile( fileA );
            ::Test::GenerateRandomFile( fileB );
            ::Test::GenerateRandomFile( fileC );
        }

        const std::vector< std::string > contents = File::List( root, false );

        std::vector< boost::filesystem::path > test;
        test.push_back( fileA );
        test.push_back( fileB );

        EXPECT_EQ( 2, contents.size() );
        EXPECT_TRUE( std::is_permutation( contents.begin(), contents.end(), test.begin() ) );
        //! [List]

        ::Test::CleanUp( root );
    }

    TEST( File, List, Recursive )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string dirA = root + ::Test::GenerateDirectoryName();
        const std::string dirB = root + ::Test::GenerateDirectoryName();
        const std::string fileA = root + Path::GetUniqueFileName();
        const std::string fileB = root + Path::GetUniqueFileName();
        const std::string fileC = dirA + Path::GetUniqueFileName();

        {
            ASSERT_TRUE( Directory::Create( root ) );

            ASSERT_TRUE( Directory::Exists( root ) );

            ASSERT_TRUE( Directory::Create( dirA ) );
            ASSERT_TRUE( Directory::Create( dirB ) );

            ASSERT_TRUE( Directory::Exists( dirA ) );
            ASSERT_TRUE( Directory::Exists( dirB ) );

            ::Test::GenerateRandomFile( fileA );
            ::Test::GenerateRandomFile( fileB );
            ::Test::GenerateRandomFile( fileC );
        }

        const std::vector< std::string > contents = File::List( root, true );
        std::vector< boost::filesystem::path > test;
        test.push_back( fileA );
        test.push_back( fileB );
        test.push_back( fileC );

        EXPECT_EQ( 3, contents.size() );
        EXPECT_TRUE( std::is_permutation( contents.begin(), contents.end(), test.begin() ) );

        ::Test::CleanUp( root );
    }

    TEST( File, List, Default )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string dirA = root + ::Test::GenerateDirectoryName();
        const std::string dirB = root + ::Test::GenerateDirectoryName();
        const std::string fileA = root + Path::GetUniqueFileName();
        const std::string fileB = root + Path::GetUniqueFileName();
        const std::string fileC = dirA + Path::GetUniqueFileName();

        {
            ASSERT_TRUE( Directory::Create( root ) );

            ASSERT_TRUE( Directory::Exists( root ) );

            ASSERT_TRUE( Directory::Create( dirA ) );
            ASSERT_TRUE( Directory::Create( dirB ) );

            ASSERT_TRUE( Directory::Exists( dirA ) );
            ASSERT_TRUE( Directory::Exists( dirB ) );

            ::Test::GenerateRandomFile( fileA );
            ::Test::GenerateRandomFile( fileB );
            ::Test::GenerateRandomFile( fileC );
        }

        const std::vector< std::string > contents = File::List( root, false );

        std::vector< boost::filesystem::path > test;
        test.push_back( fileA );
        test.push_back( fileB );

        EXPECT_EQ( 2, contents.size() );
        EXPECT_TRUE( std::is_permutation( contents.begin(), contents.end(), test.begin() ) );

        ::Test::CleanUp( root );
    }

    TEST( File, List, Empty )
    {
        const std::string directory = ::Test::GenerateDirectoryName( "common" );

        {
            ASSERT_TRUE( Directory::Create( directory ) );
            ASSERT_TRUE( Directory::Exists( directory ) );
        }

        const std::vector< std::string > contents = File::List( directory );
        EXPECT_EQ( 0, contents.size() );

        ::Test::CleanUp( directory );
    }

    TEST( File, List, NotExisting )
    {
        const std::string directory = ::Test::GenerateDirectoryName( "common" );

        const std::vector< std::string > contents = File::List( directory );
        EXPECT_EQ( 0, contents.size() );
    }

    TEST( File, SetLastModified, NotExisting )
    {
        const std::string file = ::Path::GetUniqueFileName();

        ::File::SetLastModified( file, 0 );
    }

    TEST( File, Clear )
    {
        //! [Clear]
        const std::string root  = ::Test::GenerateDirectoryName( "common" );
        const std::string file  = root + Path::GetUniqueFileName();
        const std::string content = ::Test::GenerateRandomString();

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file, content );
        }

        File::Clear( file );

        EXPECT_TRUE( File::Exists( file ) );
        EXPECT_EQ( "", File::ReadAllText( file ) );
        EXPECT_NE( "", content );
        //! [Clear]

        ::Test::CleanUp( root );
    }


    TEST( File, Clear, NotExisting )
    {
        const std::string root  = ::Test::GenerateDirectoryName( "common" );
        const std::string file  = root + Path::GetUniqueFileName();

        {
            ASSERT_FALSE( Directory::Exists( root ) );
        }

        File::Clear( file );

        EXPECT_FALSE( File::Exists( file ) );

        ::Test::CleanUp( root );
    }

    TEST( File, Delete )
    {
        //! [Delete]
        const std::string root  = ::Test::GenerateDirectoryName( "common" );
        const std::string file  = root + Path::GetUniqueFileName();

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file );
        }

        File::Delete( file );

        EXPECT_FALSE( File::Exists( file ) );
        //! [Delete]

        ::Test::CleanUp( root );
    }

    TEST( File, Delete, NotExisting )
    {
        const std::string root  = ::Test::GenerateDirectoryName( "common" );
        const std::string file  = root + Path::GetUniqueFileName();

        {
            ASSERT_FALSE( Directory::Exists( root ) );
        }

        File::Delete( file );

        EXPECT_FALSE( File::Exists( file ) );

        ::Test::CleanUp( root );
    }

    TEST( File, Move )
    {
        //! [Move]
        const std::string root  = ::Test::GenerateDirectoryName( "common" );
        const std::string file  = root + Path::GetUniqueFileName();
        const std::string file2 = root + Path::GetUniqueFileName();

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file );

            ASSERT_FALSE( File::Exists( file2 ) );
        }

        EXPECT_TRUE( File::Move( file, file2 ) );

        EXPECT_FALSE( File::Exists( file ) );
        EXPECT_TRUE( File::Exists( file2 ) );

        EXPECT_TRUE( File::Move( file2, file ) );

        EXPECT_TRUE( File::Exists( file ) );
        EXPECT_FALSE( File::Exists( file2 ) );
        //! [Move]

        ::Test::CleanUp( root );
    }

    TEST( File, Move, FromIsTrue )
    {
        const std::string root  = ::Test::GenerateDirectoryName( "common" );
        const std::string file  = root + Path::GetUniqueFileName();
        const std::string file2 = file;

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file );

            ASSERT_TRUE( File::Exists( file2 ) );
        }

        EXPECT_FALSE( File::Move( file, file2, true ) );

        EXPECT_TRUE( File::Exists( file ) );
        EXPECT_TRUE( File::Exists( file2 ) );

        ::Test::CleanUp( root );
    }

    TEST( File, Move, ToAlreadyExists )
    {
        const std::string root  = ::Test::GenerateDirectoryName( "common" );
        const std::string file  = root + Path::GetUniqueFileName();
        const std::string file2 = root + Path::GetUniqueFileName();
        const std::vector< std::string > content  = ::Test::GenerateUniqueStrings< 2 >();

        {
            ASSERT_NE( content[0], content[1] );

            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file, content[0] );
            ::Test::GenerateRandomFile( file2, content[1] );

            ASSERT_NE( File::ReadAllText( file ), File::ReadAllText( file2 ) );
        }

        EXPECT_TRUE( File::Move( file, file2, true ) );

        EXPECT_EQ( content[0], File::ReadAllText( file2 ) );
        EXPECT_FALSE( File::Exists( file ) );

        EXPECT_TRUE( File::Move( file2, file ) );

        EXPECT_EQ( content[0], File::ReadAllText( file ) );
        EXPECT_FALSE( File::Exists( file2 ) );

        ::Test::CleanUp( root );
    }

    TEST( File, Move, ToAlreadyExistsNoOverwrite )
    {
        const std::string root  = ::Test::GenerateDirectoryName( "common" );
        const std::string file  = root + Path::GetUniqueFileName();
        const std::string file2 = root + Path::GetUniqueFileName();
        const std::string content  = ::Test::GenerateRandomString();
        const std::string content2 = ::Test::GenerateRandomString();

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file, content );
            ::Test::GenerateRandomFile( file2, content2 );

            ASSERT_NE( File::ReadAllText( file ), File::ReadAllText( file2 ) );
        }

        EXPECT_FALSE( File::Move( file, file2 ) );

        EXPECT_EQ( content2, File::ReadAllText( file2 ) );
        EXPECT_TRUE( File::Exists( file ) );

        ::Test::CleanUp( root );
    }

    TEST( File, Move, NotExisting )
    {
        const std::string file  = Path::GetUniqueFileName();
        const std::string file2 = Path::GetUniqueFileName();

        EXPECT_FALSE( File::Move( file, file2 ) );
    }

    TEST( File, Move, Self )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string file = root + Path::GetUniqueFileName();

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file );
        }

        EXPECT_FALSE( File::Move( file, file ) );

        ::Test::CleanUp( root );
    }

    TEST( File, Copy )
    {
        const std::string root  = ::Test::GenerateDirectoryName( "common" );
        const std::string file  = root + Path::GetUniqueFileName();
        const std::string file2 = root + Path::GetUniqueFileName();

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file );

            ASSERT_FALSE( File::Exists( file2 ) );
        }

        EXPECT_TRUE( File::Copy( file, file2 ) );

        EXPECT_TRUE( File::AreEqual( file, file2 ) );

        ::Test::CleanUp( root );
    }

    TEST( File, Copy, ToAlreadyExists )
    {
        const std::string root  = ::Test::GenerateDirectoryName( "common" );
        const std::string file  = root + Path::GetUniqueFileName();
        const std::string file2 = root + Path::GetUniqueFileName();
        const std::string content  = ::Test::GenerateRandomString();
        const std::string content2 = ::Test::GenerateRandomString();

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file, content );
            ::Test::GenerateRandomFile( file2, content2 );

            ASSERT_NE( File::ReadAllText( file ), File::ReadAllText( file2 ) );
        }

        EXPECT_TRUE( File::Copy( file, file2, true ) );

        EXPECT_EQ( content, File::ReadAllText( file2 ) );
        EXPECT_EQ( content, File::ReadAllText( file ) );

        ::Test::CleanUp( root );
    }

    TEST( File, Copy, ToAlreadyExistsNoOverwrite )
    {
        const std::string root  = ::Test::GenerateDirectoryName( "common" );
        const std::string file  = root + Path::GetUniqueFileName();
        const std::string file2 = root + Path::GetUniqueFileName();
        const std::string content  = ::Test::GenerateRandomString();
        const std::string content2 = ::Test::GenerateRandomString();

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file, content );
            ::Test::GenerateRandomFile( file2, content2 );

            ASSERT_NE( File::ReadAllText( file ), File::ReadAllText( file2 ) );
        }

        EXPECT_FALSE( File::Copy( file, file2 ) );

        EXPECT_EQ( content2, File::ReadAllText( file2 ) );
        EXPECT_EQ( content, File::ReadAllText( file ) );

        ::Test::CleanUp( root );
    }

    TEST( File, Copy, NotExisting )
    {
        const std::string file  = Path::GetUniqueFileName();
        const std::string file2 = Path::GetUniqueFileName();

        EXPECT_FALSE( File::Copy( file, file2 ) );
    }

    TEST( File, Copy, FromIsTrue )
    {
        const std::string root  = ::Test::GenerateDirectoryName( "common" );
        const std::string file  = root + Path::GetUniqueFileName();
        const std::string file2 = file;

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );

            ::Test::GenerateRandomFile( file );

            ASSERT_TRUE( File::Exists( file2 ) );
        }

        EXPECT_FALSE( File::Copy( file, file2, true ) );

        EXPECT_TRUE( File::Exists( file ) );
        EXPECT_TRUE( File::Exists( file2 ) );

        ::Test::CleanUp( root );
    }

}