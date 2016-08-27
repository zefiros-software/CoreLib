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

#include "common/environment.h"
#include "common/directory.h"
#include "common/path.h"

#include "preproc/env.h"

#include "api/console.h"

#include "config.h"

#if OS_IS_WINDOWS
#   include <windows.h>
#else
#   include <unistd.h>
#endif

namespace Path
{
    std::string Get( const std::string &path, Type type /*= Type::Game */ )
    {
        std::string from;
        bool resolve = true;

        switch ( type )
        {
        case Path::Type::Program:
            from = Path::GetProgramDirectory();
            break;

        case Path::Type::Data:
            from = Path::GetProgramDataDirectory();
            break;

        case Path::Type::SharedData:
            from = Path::GetProgramSharedDataDirectory();
            break;

        case Path::Type::Temp:
            from = Path::GetProgramTempDirectory();
            break;

        case Path::Type::Editor:
            from = Path::GetProgramDirectory();
            break;

        case Path::Type::None:
            resolve = false;
            break;
        }

        return resolve ? Path::ResolveRelative( from, path ) : path;
    }

    std::string ResolveRelative( const std::string &cfrom, const std::string &cto, bool sameRoot /*= true */ )
    {
        std::string from = cfrom;
        std::string to = cto;
#if !(OS_IS_WINDOWS)
        from = String::Replace( cfrom, "\\", "/" );
        to = String::Replace( cto, "\\", "/" );
#endif

        if ( sameRoot )
        {
            // Just add the relative path to the from path an normalise :)
            return Canonical( FixStyle( boost::filesystem::path( from ).parent_path().generic_string() ) + to );
        }
        else
        {
            // Cheers - http://stackoverflow.com/questions/10167382/boostfilesystem-get-relative-path

            boost::filesystem::path fsFrom, fsTo, result;

            fsFrom  = boost::filesystem::absolute( from ).parent_path();
            fsTo    = boost::filesystem::absolute( to );

            boost::filesystem::path::const_iterator itrFrom( fsFrom.begin() ), itrTo( fsTo.begin() );

            // Find common base
            /// @todo itrTo != toEnd always true
            for ( boost::filesystem::path::const_iterator toEnd( fsTo.end() ), fromEnd( fsFrom.end() ) ;
                    itrTo != toEnd && itrFrom != fromEnd && *itrFrom == *itrTo; ++itrFrom, ++itrTo );

            // Navigate backwards in directory to reach previously found base
            for ( boost::filesystem::path::const_iterator fromEnd( fsFrom.end() ); itrFrom != fromEnd; ++itrFrom )
            {
                if ( ( *itrFrom ) != "." )
                {
                    result /= "..";
                }
            }

            // Now navigate down the directory branch
            for ( ; itrTo != fsTo.end() ; ++itrTo )
            {
                result /= *itrTo;
            }

            return Canonical( result.generic_string() );
        }
    }


    std::string FixStyle( const std::string &filePath )
    {
        const boost::filesystem::path path( filePath );
        std::string newPath = path.generic_string();

        if ( !path.has_extension() )
        {
            if ( path.empty() || newPath.back() != '/' )
            {
                newPath += '/';
            }
        }

#if !(OS_IS_WINDOWS)
        newPath = String::Replace( newPath, "\\", "/" );
        newPath = String::Replace( newPath, "//", "/" );
#endif
        return newPath;
    }

    std::string Canonical( const std::string &opath, bool absolute /*= false */ )
    {
        std::string path = opath;
#if !(OS_IS_WINDOWS)
        path = String::Replace( path, "\\", "/" );
#endif

        if ( ( path == "." || path == "./" ) && !absolute )
        {
            return path;
        }

        // Again Cheers - http://stackoverflow.com/questions/1746136/how-do-i-normalize-a-pathname-using-boostfilesystem
        boost::filesystem::path fsPath( path );

        if ( absolute )
        {
            fsPath = boost::filesystem::absolute( fsPath );
        }

        boost::filesystem::path result;

        for ( auto it = fsPath.begin(), end = fsPath.end(); it != end; ++it )
        {
            const std::string part = it->generic_string();

            if ( part == ".." )
            {
                if ( result.filename() == ".." )
                {
                    result /= *it;
                }
                else
                {
                    if ( result != "" )
                    {
                        result = result.parent_path();
                    }
                    else
                    {
                        result /= "..";
                    }
                }
            }
            else if ( part == "." )
            {
                // Ignore
            }
            else
            {
                // Just cat other path entries
                result /= *it;
            }
        }

        return FixStyle( result.generic_string() );
    }

    bool IsParent( const std::string &from, const std::string &to )
    {
        const std::string canFrom = Canonical( from, true );
        const std::string canTo = Canonical( to, true );
        const std::string relative = Canonical( canFrom + ResolveRelative( canFrom, canTo, false ), true );

        return relative.find( canFrom ) == 0;
    }

    std::string GetFileName( const std::string &path, const bool stripExtension /*= false */ )
    {
        const boost::filesystem::path fsPath( FixStyle( path ) );
        std::string filename;

        if ( !stripExtension )
        {
            filename = fsPath.filename().generic_string();
        }
        else
        {
            filename = fsPath.stem().generic_string();
        }

        return filename;
    }

    std::string GetDirectory( const std::string &path )
    {
        return FixStyle( boost::filesystem::path( path ).parent_path().generic_string() );
    }

    std::string GetExtension( const std::string &filepath, const bool addDot /*= false */ )
    {
        const std::string fullExtension = boost::filesystem::path( filepath ).extension().generic_string();

        return addDot || fullExtension == "" ? fullExtension : fullExtension.substr( 1 );
    }

    bool HasExtension( const std::string &filepath )
    {
        return boost::filesystem::path( filepath ).has_extension();
    }

    std::string GetUniqueFileName( const std::string &extension /*= ".tmp" */ )
    {
        return FixStyle( boost::filesystem::unique_path( "%%%%-%%%%-%%%%-%%%%" + extension ).generic_string() );
    }

    std::string GetUniqueDirectory()
    {
        return FixStyle( boost::filesystem::unique_path( "%%%%-%%%%-%%%%" ).generic_string() );
    }

    std::string GetTempDirectory()
    {
        return FixStyle( boost::filesystem::temp_directory_path().generic_string() );
    }

    std::string GetExeDirectory()
    {
        return FixStyle( boost::filesystem::path( GetExeFile() ).parent_path().generic_string() );
    }

    std::string GetExeFile()
    {
#if OS_IS_WINDOWS
        char result[ MAX_PATH ];
        return Canonical( std::string( result, GetModuleFileNameA( nullptr, result, MAX_PATH ) ) );
#elif OS_IS_LINUX
        char result[ PATH_MAX ];
        size_t count = readlink( "/proc/self/exe", result, PATH_MAX );
        return std::string( result, ( count > 0 ) ? count : 0 );
#else
#error
#endif
    }

    std::string GetDataDirectory()
    {
#if OS_IS_WINDOWS
        return FixStyle( Environment::GetVariable( "APPDATA" ) );
#elif OS_IS_LINUX
        return FixStyle( "~/local/share/" );
#elif OS_IS_MACOS
        return "~/Library/Application Support/";
#endif
    }

    std::string GetSharedDataDirectory()
    {
#if OS_IS_WINDOWS
        return FixStyle( Environment::GetVariable( "ALLUSERSPROFILE" ) );
#elif OS_IS_LINUX
        return "/usr/local/";
#elif OS_IS_MACOS
        return "/usr/local/";
#endif
    }

    std::string GetProgramDirectory()
    {
        return GetExeDirectory();
    }

    std::string GetProgramTempDirectory()
    {
        return String::Format( "%s%s/%s/", GetTempDirectory(), std::string( PROGRAM_COMPANY ), std::string( PROGRAM_NAME ) );
    }

    std::string GetProgramDataDirectory()
    {
        return String::Format( "%s%s/%s/", GetDataDirectory(), std::string( PROGRAM_COMPANY ), std::string( PROGRAM_NAME ) );
    }

    std::string GetProgramSharedDataDirectory()
    {
        return String::Format( "%s%s/%s/", GetSharedDataDirectory(),
                               std::string( PROGRAM_COMPANY ),
                               std::string( PROGRAM_NAME ) );
    }

    std::vector< boost::filesystem::path > List( const std::string &directory, bool recursive /*= false */ )
    {
        std::vector< boost::filesystem::path > contents;

        if ( !Directory::Exists( directory ) )
        {
            return contents;
        }

        if ( recursive )
        {
            boost::filesystem::recursive_directory_iterator it( directory ), end;

            for ( ; it != end; ++it )
            {
                contents.push_back( *it );
            }
        }
        else
        {
            boost::filesystem::directory_iterator it( directory ), end;

            for ( ; it != end; ++it )
            {
                contents.push_back( *it );
            }
        }

        return contents;
    }

    std::vector< std::string > ListContent( const std::string &directory, bool recursive /*= false */ )
    {
        const std::vector< boost::filesystem::path > contents = List( directory, recursive );
        std::vector< std::string > result;
        result.reserve( contents.size() );

        for ( auto it = contents.begin(), end = contents.end(); it != end; ++it )
        {
            result.push_back( FixStyle( it->generic_string() ) );
        }

        return result;
    }

    std::string GetWorkingDirectory()
    {
        return Path::FixStyle( boost::filesystem::current_path().generic_string() );
    }

    bool SetWorkingDirectory( const std::string &workingDirectory )
    {
        bool success = true;

        try
        {
            boost::filesystem::current_path( workingDirectory );
        }
        catch ( boost::filesystem::filesystem_error & )
        {
            success = false;
        }

        return success;
    }

    void DeleteAll( const std::string &path )
    {
        boost::filesystem::remove_all( path );
    }

}