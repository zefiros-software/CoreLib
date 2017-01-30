/**
 * @cond ___LICENSE___
 *
 * Copyright (c) 2017 Zefiros Software
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

#include "common/format.h"
#include "common/file.h"

namespace File
{

    void ReadOpen( std::ifstream &stream, const std::string &relFilepath, Path::Type type /*= Type::Game*/,
                   std::ios_base::openmode mode /*= std::ios_base::in */ )
    {
        stream.open( Get( relFilepath, type ), mode );
    }

    void WriteOpen( std::ofstream &stream, const std::string &relFilepath, Path::Type type /*= Type::Game*/,
                    std::ios_base::openmode mode /*= std::ios_base::out */ )
    {
        stream.open( Get( relFilepath, type ), mode );
    }

    void Open( std::fstream &stream, const std::string &relFilepath, Path::Type type /*= Type::Game*/,
               std::ios_base::openmode mode /*= std::ios_base::in | std::ios_base::out */ )
    {
        stream.open( Get( relFilepath, type ), mode );
    }

    std::string TempGet() noexcept
    {
        return String::Format( "%s/%s", Path::GetProgramTempDirectory(), Path::GetUniqueFileName() );
    }

    void TempOpen( std::fstream &stream,
                   std::ios_base::openmode mode /*= std::ios_base::in | std::ios_base::out | std::ios_base::trunc */ )
    {
        Open( stream, TempGet(), Path::Type::None, mode );
    }

    void TempOpen( std::fstream &stream, std::string &filePath,
                   std::ios_base::openmode mode /*= std::ios_base::in | std::ios_base::out | std::ios_base::trunc */ )
    {
        filePath = TempGet();
        Open( stream, filePath, Path::Type::None, mode );
    }

    bool Exists( const std::string &filepath ) noexcept
    {
        return boost::filesystem::exists( filepath ) && boost::filesystem::is_regular_file( filepath );
    }

    U64 Size( const std::string &filepath ) noexcept
    {
        if ( Exists( filepath ) )
        {
            return boost::filesystem::file_size( filepath );
        }

        return 0;
    }

    bool IsEmpty( const std::string &filepath ) noexcept
    {
        return boost::filesystem::is_empty( filepath );
    }

    bool AreEqual( const std::string &filePath1, const std::string &filePath2 )
    {
        if ( !File::Exists( filePath1 ) || !File::Exists( filePath2 ) )
        {
            return false;
        }

        std::fstream file1( filePath1 ), file2( filePath2 );
        std::string string1, string2;

        bool equal = true;

        while ( !file1.eof() && equal )
        {
            std::getline( file1, string1 );
            std::getline( file2, string2 );

            if ( string1 != string2 )
            {
                equal = false;
            }
        }

        return equal;
    }

    bool IsSame( const std::string &filePath1, const std::string &filePath2 )
    {
        if ( File::Exists( filePath1 ) && File::Exists( filePath2 ) )
        {
            return boost::filesystem::equivalent( filePath1, filePath2 );
        }

        return false;
    }

    std::string ReadAllText( const std::string &filepath )
    {
        // Stop if there is nothing to read
        // and just return a null string
        if ( !Exists( filepath ) )
        {
            return "";
        }

        // Open and read the file
        std::ifstream ifs;
        ReadOpen( ifs, filepath, Path::Type::None );
        const std::string content( ( std::istreambuf_iterator< char >( ifs ) ),
                                   ( std::istreambuf_iterator< char >() ) );
        return content;
    }

    std::time_t GetLastModified( const std::string &path ) noexcept
    {
        if ( File::Exists( path ) )
        {
            return boost::filesystem::last_write_time( path );
        }

        return 0;
    }

    std::vector< std::string > List( const std::string &directory, bool recursive /*= false */ ) noexcept
    {
        const std::vector< boost::filesystem::path > contents = Path::List( directory, recursive );
        std::vector< std::string > resultContents;

        for ( const boost::filesystem::path &fsPath : contents )
        {
            if ( boost::filesystem::is_regular_file( fsPath ) )
            {
                resultContents.push_back( fsPath.generic_string() );
            }
        }

        return resultContents;
    }

    void SetLastModified( const std::string &path, std::time_t time )
    {
        if ( File::Exists( path ) )
        {
            boost::filesystem::last_write_time( path, time );
        }
    }

    void Clear( const std::string &filepath ) noexcept
    {
        // only if the file exists
        if ( Exists( filepath ) )
        {
            std::ofstream f( filepath );
            f.close();
        }
    }

    bool Delete( const std::string &filepath ) noexcept
    {
        if ( Exists( filepath ) )
        {
            return boost::filesystem::remove( filepath );
        }

        return true;
    }

    bool Move( const std::string &from, const std::string &to, const bool overwrite /*= false */ )
    {
        if ( Exists( from ) && boost::filesystem::absolute( from ) != boost::filesystem::absolute( to ) )
        {
            const bool exists = Exists( to );

            if ( !( !overwrite && exists ) )
            {
                if ( overwrite && exists )
                {
                    if ( !File::Delete( to ) )
                    {
                        return false;
                    }
                }

                try
                {
                    boost::filesystem::rename( from, to );
                }
                catch ( ... )
                {
                    // rename may fail between systems
                    // in this case we need to copy and delete
                    boost::filesystem::copy( from, to );

                    if ( Exists( to ) )
                    {
                        boost::filesystem::remove( from );
                    }
                    else
                    {
                        return false;
                    }
                }

                return true;
            }
        }

        return false;
    }

    bool Copy( const std::string &from, const std::string &to, bool overwrite /*= false */ )
    {
        if ( Exists( from ) && boost::filesystem::absolute( from ) != boost::filesystem::absolute( to ) )
        {
            const bool exists = Exists( to );

            if ( !exists || overwrite )
            {
                boost::filesystem::copy_file( from, to, boost::filesystem::copy_option::overwrite_if_exists );
                return true;
            }
        }

        return false;
    }

    bool IsSharedLibrary( const std::string &s )
    {
        return ( s.find( ".dll" ) != std::string::npos || s.find( ".so" ) != std::string::npos ||
                 s.find( ".dylib" ) != std::string::npos )
               && s.find( ".lib" ) == std::string::npos
               && s.find( ".exp" ) == std::string::npos
               && s.find( ".pdb" ) == std::string::npos
               && s.find( ".manifest" ) == std::string::npos
               && s.find( ".rsp" ) == std::string::npos
               && s.find( ".obj" ) == std::string::npos
               && s.find( ".a" ) == std::string::npos;
    }

}