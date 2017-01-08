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

#include "config.h"

#include "engineTest.h"

namespace
{

    TEST( Path, Get, ResolvesCorrectFolders )
    {
        EXPECT_EQ( Path::GetProgramDirectory(), Path::Get( "./", Path::Type::Program ) );
        EXPECT_EQ( Path::GetProgramDataDirectory(), Path::Get( "./", Path::Type::Data ) );
        EXPECT_EQ( Path::GetProgramSharedDataDirectory(), Path::Get( "./", Path::Type::SharedData ) );
        EXPECT_EQ( Path::GetProgramTempDirectory(), Path::Get( "./", Path::Type::Temp ) );
        EXPECT_EQ( "./", Path::Get( "./", Path::Type::None ) );
    }

    TEST( Path, Get, CorrectDefault )
    {
        EXPECT_EQ( Path::GetProgramDirectory(), Path::Get( "./", Path::Type::Program ) );
        EXPECT_EQ( Path::GetProgramDirectory(), Path::Get( "./" ) );
    }

    TEST( Path, Get, DifferentFolders )
    {
        const std::string game = Path::Get( "./", Path::Type::Program );
        const std::string data = Path::Get( "./", Path::Type::Data );
        const std::string sharedData = Path::Get( "./", Path::Type::SharedData );
        const std::string temp = Path::Get( "./", Path::Type::Temp );

        std::unordered_set< std::string > set;
        set.insert( game );
        set.insert( data );
        set.insert( sharedData );
        set.insert( temp );

        EXPECT_EQ( 4, set.size() );
    }

    TEST( Path, ResolveRelative, Default )
    {
        const std::string from = "module/ZefirosEngine/main.as";
        const std::string to = "behaviour.as";
        const std::string path = Path::ResolveRelative( from, to );
        EXPECT_EQ( "module/ZefirosEngine/behaviour.as", path );
    }

    TEST( Path, ResolveRelative, SameRoot )
    {
        const std::string from = "module/ZefirosEngine/main.as";
        const std::string to   = "behaviour.as";
        const std::string path = Path::ResolveRelative( from, to, true );
        EXPECT_EQ( "module/ZefirosEngine/behaviour.as", path );
    }

    TEST( Path, ResolveRelative, SameRoot_Explicit )
    {
        const std::string from = "module/ZefirosEngine/main.as";
        const std::string to   = "./behaviour.as";
        const std::string path = Path::ResolveRelative( from, to, true );
        EXPECT_EQ( "module/ZefirosEngine/behaviour.as", path );
    }

    TEST( Path, ResolveRelative, SameRoot_Windows )
    {
        const std::string from = "module\\ZefirosEngine\\main.as";
        const std::string to   = "behaviour.as";
        const std::string path = Path::ResolveRelative( from, to, true );
        EXPECT_EQ( "module/ZefirosEngine/behaviour.as", path );
    }

    TEST( Path, ResolveRelative, SameRoot_WindowsExplicit )
    {
        const std::string from = "module\\ZefirosEngine\\main.as";
        const std::string to   = ".\\behaviour.as";
        const std::string path = Path::ResolveRelative( from, to, true );
        EXPECT_EQ( "module/ZefirosEngine/behaviour.as", path );
    }

    TEST( Path, ResolveRelative, SameRoot_Up )
    {
        const std::string from = "module/ZefirosEngine/main.as";
        const std::string to   = "../behaviour.as";
        const std::string path = Path::ResolveRelative( from, to, true );
        EXPECT_EQ( "module/behaviour.as", path );
    }

    TEST( Path, ResolveRelative, SameRoot_UpUp )
    {
        const std::string from = "module/ZefirosEngine/main.as";
        const std::string to   = "../../behaviour.as";
        const std::string path = Path::ResolveRelative( from, to, true );
        EXPECT_EQ( "behaviour.as", path );
    }

    TEST( Path, ResolveRelative )
    {
        const std::string from = "module/ZefirosEngine/main.as";
        const std::string to   = "behaviour.as";
        const std::string path = Path::ResolveRelative( from, to, true );
        EXPECT_EQ( "module/ZefirosEngine/behaviour.as", path );
    }

    TEST( Path, ResolveRelative, Explicit )
    {
        const std::string from = "module/ZefirosEngine/main.as";
        const std::string to   = "./behaviour.as";
        const std::string path = Path::ResolveRelative( from, to, false );
        EXPECT_EQ( "../../behaviour.as", path );
    }

    TEST( Path, ResolveRelative, ExplicitRelative )
    {
        const std::string from = ".././other/";
        const std::string to = "../other/";
        const std::string path = Path::ResolveRelative( from, to, false );
        EXPECT_EQ( "../other/", path );
    }

    TEST( Path, ResolveRelative, Windows )
    {
        const std::string from = "module\\ZefirosEngine\\main.as";
        const std::string to   = "behaviour.as";
        const std::string path = Path::ResolveRelative( from, to, false );
        EXPECT_EQ( "../../behaviour.as", path );
    }

    TEST( Path, ResolveRelative, WindowsExplicit )
    {
        const std::string from = "module\\ZefirosEngine\\main.as";
        const std::string to   = ".\\behaviour.as";
        const std::string path = Path::ResolveRelative( from, to, false );
        EXPECT_EQ( "../../behaviour.as", path );
    }

    TEST( Path, ResolveRelative, Up )
    {
        const std::string from = "module/ZefirosEngine/main.as";
        const std::string to   = "../behaviour.as";
        const std::string path = Path::ResolveRelative( from, to, false );
        EXPECT_EQ( "../../../behaviour.as", path );
    }

    TEST( Path, ResolveRelative, UpUp )
    {
        const std::string from = "module/ZefirosEngine/main.as";
        const std::string to   = "../../behaviour.as";
        const std::string path = Path::ResolveRelative( from, to, false );
        EXPECT_EQ( "../../../../behaviour.as", path );
    }

    TEST( Path, FixStyle, Unix )
    {
        const std::string path = Path::FixStyle( "modules/imaginary/file" );
        EXPECT_EQ( "modules/imaginary/file/", path );
    }

    TEST( Path, FixStyle, UnixFile )
    {
        const std::string path = Path::FixStyle( "modules/imaginary/file.txt" );
        EXPECT_EQ( "modules/imaginary/file.txt", path );
    }

    TEST( Path, FixStyle, Windows )
    {
        const std::string path = Path::FixStyle( ".\\modules/imaginary\\file" );
        EXPECT_EQ( "./modules/imaginary/file/", path );
    }

    TEST( Path, FixStyle, WindowsRoot )
    {
        const std::string path = Path::FixStyle( "C:\\Windows\\" );
        EXPECT_EQ( "C:/Windows/", path );
    }

    TEST( Path, FixStyle, Root )
    {
        const std::string path = Path::FixStyle( "/" );
        EXPECT_EQ( "/", path );
    }

    TEST( Path, FixStyle, Empty )
    {
        const std::string path = Path::FixStyle( "" );
        EXPECT_EQ( "/", path );
    }

    TEST( Path, Canonical, Self )
    {
        const std::string path = "/root/folder/file.ext";
        EXPECT_EQ( path, Path::Canonical( path ) );
    }

    TEST( Path, Canonical, Up )
    {
        const std::string path = "/root/../file.ext";
        EXPECT_EQ( "/file.ext", Path::Canonical( path ) );
    }

    TEST( Path, Canonical, Same )
    {
        const std::string path = "/root/./file.ext";
        EXPECT_EQ( "/root/file.ext", Path::Canonical( path ) );
    }

    TEST( Path, Canonical, Combined )
    {
        const std::string path = "/root/.././file.ext";
        EXPECT_EQ( "/file.ext", Path::Canonical( path ) );
    }

    TEST( Path, Canonical, UpUpRoot )
    {
        const std::string path = "../../file.ext";
        EXPECT_EQ( path, Path::Canonical( path ) );
    }

    TEST( Path, Canonical, UpUpFromRoot )
    {
        const std::string path = "module/../../file.ext";
        EXPECT_EQ( "../file.ext", Path::Canonical( path ) );
    }

    TEST( Path, Canonical, Current )
    {
        EXPECT_EQ( "./", Path::Canonical( "./" ) );
        EXPECT_EQ( ".", Path::Canonical( "." ) );
    }

    TEST( Path, Canonical, Absolute )
    {
        const std::string path = "../../file.ext";
        const std::string abs  = boost::filesystem::absolute( path ).generic_string();
        EXPECT_EQ( "file.ext", Path::ResolveRelative( Path::Canonical( path, true ), abs, false ) );
    }

    TEST( Path, IsParent )
    {
        EXPECT_TRUE( Path::IsParent( "./", "./" ) );
        EXPECT_TRUE( Path::IsParent( "../", "./" ) );
        EXPECT_TRUE( Path::IsParent( "../", "././" ) );
        EXPECT_TRUE( Path::IsParent( "base/", "base/child" ) );


        EXPECT_FALSE( Path::IsParent( "./", "../" ) );
        EXPECT_FALSE( Path::IsParent( "base/child", "base/" ) );
    }

    TEST( Path, GetFileName )
    {
        const std::string file = Path::GetFileName( "./modules/imaginary.file" );
        EXPECT_EQ( "imaginary.file", file );
    }

    TEST( Path, GetFileName, OnlyExtension )
    {
        const std::string file = Path::GetFileName( "./modules/.file" );
        EXPECT_EQ( ".file", file );
    }

    TEST( Path, GetFileName, Windows )
    {
        const std::string file = Path::GetFileName( ".\\modules\\imaginary.file" );
        EXPECT_EQ( "imaginary.file", file );
    }

    TEST( Path, GetFileName, StripExtension )
    {
        const std::string file = Path::GetFileName( "./modules/imaginary", true );
        EXPECT_EQ( ".", file );
    }

    TEST( Path, GetFileName, StripExtension2 )
    {
        const std::string file = Path::GetFileName( "./modules/imaginary.ext", true );
        EXPECT_EQ( "imaginary", file );
    }

    TEST( Path, GetDirectory )
    {
        const std::string dir = Path::GetDirectory( "./modules/imaginary.file" );
        EXPECT_EQ( "./modules/", dir );
    }

    TEST( Path, GetDirectory, NoExtension )
    {
        const std::string dir = Path::GetDirectory( "./modules/imaginary" );
        EXPECT_EQ( "./modules/", dir );
    }

    TEST( Path, GetExtension )
    {
        const std::string ext = Path::GetExtension( "./modules/imaginary.file" );
        EXPECT_EQ( "file", ext );
    }

    TEST( Path, GetExtension, NoExtension1 )
    {
        const std::string ext = Path::GetExtension( "./modules/imaginary." );
        EXPECT_EQ( "", ext );
    }

    TEST( Path, GetExtension, NoExtension2 )
    {
        const std::string ext = Path::GetExtension( "./modules/imaginary" );
        EXPECT_EQ( "", ext );
    }

    TEST( Path, GetExtension, WithDot )
    {
        const std::string ext = Path::GetExtension( "./modules/imaginary.file", true );
        EXPECT_EQ( ".file", ext );
    }

    TEST( Path, GetExtension, NoExtension )
    {
        const std::string ext = Path::GetExtension( "./modules/imaginary", true );
        EXPECT_EQ( "", ext );
    }

    TEST( Path, HasExtension )
    {
        EXPECT_TRUE( Path::HasExtension( "./modules/imaginary.ext" ) );
    }

    TEST( Path, HasExtension, HiddenNoExtension )
    {
        EXPECT_TRUE( Path::HasExtension( "./modules/.ext" ) );
    }

    TEST( Path, HasExtension, Hidden2 )
    {
        EXPECT_TRUE( Path::HasExtension( "./modules/.ext.ext" ) );
    }

    TEST( Path, HasExtension, NoExtension1 )
    {
        EXPECT_TRUE( Path::HasExtension( "./modules/imaginary." ) );
    }

    TEST( Path, HasExtension, NoExtension2 )
    {
        EXPECT_FALSE( Path::HasExtension( "./modules/." ) );
    }

    TEST( Path, HasExtension, NoExtension3 )
    {
        EXPECT_FALSE( Path::HasExtension( "./modules/file" ) );
    }

    TEST( Path, GetUniqueFileName )
    {
        // Yet another lottery, may the odds be ever in your favour.
        EXPECT_NE( Path::GetUniqueFileName(), Path::GetUniqueFileName() );
    }

    TEST( Path, GetUniqueFileName, CorrectExtension )
    {
        const std::string path = Path::GetUniqueFileName( ".lol" );
        EXPECT_TRUE( Path::HasExtension( path ) );
        EXPECT_EQ( "lol", Path::GetExtension( path ) );
    }

    TEST( Path, GetUniqueFileName, CorrectDefaultExtension )
    {
        const std::string path = Path::GetUniqueFileName();
        EXPECT_TRUE( Path::HasExtension( path ) );
        EXPECT_EQ( "tmp", Path::GetExtension( path ) );
    }

    TEST( Path, GetUniqueFileName, NoExtension )
    {
        const std::string path = Path::GetUniqueFileName( "" );
        EXPECT_FALSE( Path::HasExtension( path ) );
        EXPECT_EQ( '/', path.back() );
    }

    TEST( Path, GetUniqueFileName, NotEmpty )
    {
        // Yet another lottery, may the odds be ever in your favour.
        EXPECT_NE( "", Path::GetUniqueFileName() );
    }

    TEST( Path, GetUniqueDirectory )
    {
        // Yet another lottery, may the odds be ever in your favour.
        EXPECT_NE( Path::GetUniqueDirectory(), Path::GetUniqueDirectory() );
    }

    TEST( Path, GetUniqueDirectory, NotEmpty )
    {
        // Yet another lottery, may the odds be ever in your favour.
        EXPECT_NE( "", Path::GetUniqueDirectory() );
    }

    TEST( Path, OSFolders, Unique )
    {
        const std::string data =  Path::GetDataDirectory();
        const std::string sharedData =  Path::GetSharedDataDirectory();
        const std::string temp =  Path::GetTempDirectory();

        std::unordered_set< std::string > set;
        set.insert( data );
        set.insert( sharedData );
        set.insert( temp );

        EXPECT_EQ( 3, set.size() );
    }

    TEST( Path, ProgramTempDirectory, Structure )
    {
        const std::string dir = Path::GetProgramTempDirectory();
        const std::string tmp = Path::GetTempDirectory();
        EXPECT_EQ( "../../", Path::ResolveRelative( dir, tmp, false ) );
        EXPECT_EQ( tmp + std::string( PROGRAM_COMPANY ) + "/" + std::string( PROGRAM_NAME ) + "/", dir );
    }

    TEST( Path, GetProgramDataDirectory, Structure )
    {
        const std::string dir = Path::GetProgramDataDirectory();
        const std::string data = Path::GetDataDirectory();
        EXPECT_EQ( "../../", Path::ResolveRelative( dir, data, false ) );
        EXPECT_EQ( data + std::string( PROGRAM_COMPANY ) + "/" + std::string( PROGRAM_NAME ) + "/", dir );
    }

    TEST( Path, GetProgramSharedDataDirectory, Structure )
    {
        const std::string dir = Path::GetProgramSharedDataDirectory();
        const std::string sharedData = Path::GetSharedDataDirectory();
        EXPECT_EQ( "../../", Path::ResolveRelative( dir, sharedData, false ) );
        EXPECT_EQ( sharedData + std::string( PROGRAM_COMPANY ) + "/" + std::string( PROGRAM_NAME ) + "/", dir );
    }

    TEST( Path, List )
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

        const std::vector< boost::filesystem::path > contents = Path::List( root );

        std::vector< std::string > contentsStr;

        for ( auto it = contents.begin(), end = contents.end(); it != end; ++it )
        {
            contentsStr.push_back( Path::FixStyle( it->generic_string() ) );
        }

        std::vector< std::string > test;
        test.push_back( dirA );
        test.push_back( dirB );
        test.push_back( fileA );
        test.push_back( fileB );

        EXPECT_EQ( 4, contents.size() );
        EXPECT_TRUE( std::is_permutation( contentsStr.begin(), contentsStr.end(), test.begin() ) );


        ::Test::CleanUp( root );
    }

    TEST( Path, ListRecursive )
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

        const std::vector< boost::filesystem::path > contents = Path::List( root, true );

        std::vector< std::string > contentsStr;

        for ( auto it = contents.begin(), end = contents.end(); it != end; ++it )
        {
            contentsStr.push_back( Path::FixStyle( it->generic_string() ) );
        }

        std::vector< std::string > test;
        test.push_back( dirA );
        test.push_back( dirB );
        test.push_back( fileA );
        test.push_back( fileB );
        test.push_back( fileC );

        EXPECT_EQ( 5, contents.size() );
        EXPECT_TRUE( std::is_permutation( contentsStr.begin(), contentsStr.end(), test.begin() ) );


        ::Test::CleanUp( root );
    }

    TEST( Path, List, NotExisting )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );

        const std::vector< boost::filesystem::path > contents = Path::List( root );
        EXPECT_EQ( contents.size(), 0 );
    }

    TEST( Path, List, Empty )
    {
        const std::string path = ::Test::GenerateDirectoryName( "common" );

        {
            ASSERT_TRUE( Directory::Create( path ) );
            ASSERT_TRUE( Directory::Exists( path ) );
        }

        const std::vector< boost::filesystem::path > contents = Path::List( path );
        EXPECT_EQ( 0, contents.size() );

        ::Test::CleanUp( path );
    }

    TEST( Path, ListContent )
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

        const std::vector< std::string > contents = Path::ListContent( root );
        std::vector< std::string > test;
        test.push_back( dirA );
        test.push_back( dirB );
        test.push_back( fileA );
        test.push_back( fileB );

        EXPECT_EQ( 4, contents.size() );
        EXPECT_TRUE( std::is_permutation( contents.begin(), contents.end(), test.begin() ) );

        ::Test::CleanUp( root );
    }

    TEST( Path, ListContent, Recursive )
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

        const std::vector< std::string > contents = Path::ListContent( root, true );
        std::vector< std::string > test;
        test.push_back( dirA );
        test.push_back( dirB );
        test.push_back( fileA );
        test.push_back( fileB );
        test.push_back( fileC );

        EXPECT_EQ( 5, contents.size() );
        EXPECT_TRUE( std::is_permutation( contents.begin(), contents.end(), test.begin() ) );

        ::Test::CleanUp( root );
    }

    TEST( Path, GetWorkingDirectory )
    {
        EXPECT_EQ( Path::Canonical( "./", true ), Path::GetWorkingDirectory() );
    }

    TEST( Path, SetWorkingDirectory )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string fileName = Path::GetUniqueFileName();
        const std::string file = root + fileName;

        {
            ASSERT_TRUE( Directory::Create( root ) );
            ASSERT_TRUE( Directory::Exists( root ) );
            ::Test::GenerateRandomFile( file );
        }

        const bool changedA = Path::SetWorkingDirectory( root );

        EXPECT_TRUE( changedA );
        EXPECT_TRUE( File::Exists( fileName ) );

        const bool changedB = Path::SetWorkingDirectory( "../../" );

        EXPECT_TRUE( changedB );

        ::Test::CleanUp( root );
    }

    TEST( Path, SetWorkingDirectory, NonExisting )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );

        {
            ASSERT_FALSE( Directory::Exists( root ) );
        }

        EXPECT_FALSE( Path::SetWorkingDirectory( root ) );
    }

    TEST( Path, DeleteAll, SingleDirectory )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string dir = root + ::Test::GenerateDirectoryName();

        {
            ASSERT_TRUE( Directory::CreateAll( dir ) );

            ASSERT_TRUE( Directory::Exists( root ) );
            ASSERT_TRUE( Directory::Exists( dir ) );
        }

        Path::DeleteAll( dir );

        EXPECT_TRUE( Directory::Exists( root ) );
        EXPECT_FALSE( Directory::Exists( dir ) );

        ::Test::CleanUp( root );
    }

    TEST( Path, DeleteAll, MultipleDirectories )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );
        const std::string dir = root + ::Test::GenerateDirectoryName();

        {
            ASSERT_TRUE( Directory::CreateAll( dir ) );

            ASSERT_TRUE( Directory::Exists( root ) );
            ASSERT_TRUE( Directory::Exists( dir ) );
        }

        Path::DeleteAll( root );

        EXPECT_FALSE( Directory::Exists( root ) );
        EXPECT_FALSE( Directory::Exists( dir ) );

        ::Test::CleanUp( root );
    }


    TEST( Path, DeleteAll, File )
    {
        const std::string root = ::Test::GenerateDirectoryName( "common" );

        const std::string dirA = root + ::Test::GenerateDirectoryName();
        const std::string fileA = root + Path::GetUniqueFileName();

        {
            ASSERT_TRUE( Directory::CreateAll( dirA ) );

            ASSERT_TRUE( Directory::Exists( root ) );
            ASSERT_TRUE( Directory::Exists( dirA ) );

            ::Test::GenerateRandomFile( fileA );
        }

        Path::DeleteAll( fileA );

        EXPECT_TRUE( Directory::Exists( root ) );
        EXPECT_TRUE( Directory::Exists( dirA ) );
        EXPECT_FALSE( File::Exists( fileA ) );

        ::Test::CleanUp( root );
    }
}