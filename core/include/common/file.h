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

#pragma once
#ifndef __ENGINE_FILE_H__
#define __ENGINE_FILE_H__

#include "common/types.h"
#include "common/path.h"

#include <iosfwd>

/// @addtogroup docFilesystem
/// @{

/**
 * %Namespace collection of function that handles all file(system) specific functions.
 */

namespace File
{

    /// @name File opening
    /// @{

    /**
     * Opens a read only file on the correct path type.
     *
     * @param [in,out]  stream The stream.
     * @param   relFilepath    The relative filepath.
     * @param   type           The type, it defaults to the game directory.
     * @param   mode           The mode (defaults to read only).
     *
     * @pre File::Exists( path ) == true.
     *
     * @details
     * @examples
     *
     * @snippet testFile.cpp ReadOpen
     */

    void ReadOpen( std::ifstream &stream, const std::string &relFilepath, Path::Type type = Path::Type::Program,
                   std::ios_base::openmode mode = std::ios_base::in );

    /**
     * Opens a write file stream on the correct path type.
     *
     * @param [in,out]  stream The stream.
     * @param   relFilepath    The relative filepath.
     * @param   type           the type, it defaults to the game directory.
     * @param   mode           the mode (defaults to write).
     *
     * @pre Path::Exists( Path::GetDirectory( path ) ) == true.
     *
     * @details
     * @examples
     *
     * @snippet testFile.cpp WriteOpen
     */

    void WriteOpen( std::ofstream &stream, const std::string &relFilepath, Path::Type type = Path::Type::Program,
                    std::ios_base::openmode mode = std::ios_base::out );

    /**
     * Opens a file stream on the correct path type, with the default std::fstream modes.
     *
     * @param [in,out]  stream The stream.
     * @param   relFilepath    The relative filepath.
     * @param   type           the type, it defaults to the game directory.
     * @param   mode           the mode.
     *
     * @details
     * @examples
     *
     * @snippet testFile.cpp Open
     */

    void Open( std::fstream &stream, const std::string &relFilepath, Path::Type type = Path::Type::Program,
               std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out );

    /**
     * Generates a temporary file name, statistically guaranteed to be (almost) unique.
     *
     * @return The temporary file path.
     *
     * @details
     * @examples
     *
     * @snippet testFile.cpp TempGet
     *
     * @note To the one who finds a bug here, damn.... You are unlucky.
     */

    std::string TempGet() noexcept;

    /**
     * Gets a write/read file stream to a new temporary file.
     *
     * @param [in,out]  stream The stream.
     * @param   mode           the mode.
     *
     * @details
     * @examples
     *
     * @snippet testFile.cpp TempOpen
     */

    void TempOpen( std::fstream &stream, std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out |
                                                                        std::ios_base::trunc );

    /**
     * Gets a write/read file stream to a new temporary file, the user also gets the path of the located temporary file.
     *
     * @param [in,out]  stream   The stream.
     * @param [in,out]  filePath Full pathname of the file.
     * @param   mode             the mode.
     *
     * @details
     * @examples
     *
     * @snippet testFile.cpp TempOpenPath
     */

    void TempOpen( std::fstream &stream, std::string &filePath,
                   std::ios_base::openmode mode = std::ios_base::in | std::ios_base::out | std::ios_base::trunc );

    /// @}

    /// @name Queries
    /// @{

    /**
     * Queries if a given file exists, requires to point to a file and not a directory since it will return false
     * otherwise.
     *
     * @param   filepath The pathname of the file.
     *
     * @return true if it exists, false otherwise.
     *
     * @pre
     * The following conditions should be met:
     * * The path has no trailing slash ('/').
     * * The path is given in unix style.
     *
     * @details
     * @examples
     *
     * @snippet testFile.cpp Exists
     */

    bool Exists( const std::string &filepath ) noexcept;

    /**
     * Gets the file size, if it exists, in bytes. If the file doesn't exists, the size returned will be 0.
     *
     * @param   filepath The filepath.
     *
     * @return The file size in bytes.
     *
     * @pre File::Exists(filePath) == true.
     *
     * @details
     * @examples
     *
     * @snippet testFile.cpp GetSize
     */

    U64 Size( const std::string &filepath ) noexcept;

    /**
     * Query if the file on the given filepath is empty. When the file doesn't exist, it will return true.
     *
     * @param   filepath The filepath.
     *
     * @return true if empty, false if not.
     *
     * @pre File::Exists(filePath) == true.
     *
     * @details
     * @examples
     *
     * @snippet testFile.cpp IsEmpty
     */

    bool IsEmpty( const std::string &filepath ) noexcept;

    /**
     * Check whether the contents of two files are equal, however this function is only meant to work on text files.
     *
     * @param   filePath1 The first file path.
     * @param   filePath2 The second file path.
     *
     * @return true if the files are equal, false otherwise.
     *
     * @pre
     * The following conditions should be met:
     * * filePath1 @f$ \neq @f$ filePath2.
     * * File::Exists( filePath1 ) == true && File::Exists( filePath2 ) == true.
     *
     * @details
     * @examples
     *
     * @snippet testFile.cpp AreEqual
     */

    bool AreEqual( const std::string &filePath1, const std::string &filePath2 );

    /**
     * Query if whether both given file paths point to the same physical file.
     *
     * @param   filePath1 The first file path.
     * @param   filePath2 The second file path.
     *
     * @return true if same, false if not.
     *
     * @pre File::Exists( filePath1 ) == true && File::Exists( filePath2 ) == true.
     *
     * @details
     * @examples
     *
     * @snippet file.cpp IsSame
     */

    bool IsSame( const std::string &filePath1, const std::string &filePath2 );

    /**
     * Reads a whole text file and puts it in a string. If the file doesn't exist it will return an empty string.
     *
     * @param   filepath The filepath.
     *
     * @return The file contents in a string.
     *
     * @pre File::Exists( filePath ) == true.
     *
     * @details
     * @examples
     *
     * @snippet testFile.cpp ReadAllText
     */

    std::string ReadAllText( const std::string &filepath );

    /**
     * Gets the timestamp on which the file was last modified. Returns 0 when the file doesn't exist.
     *
     * @param   path Full pathname of the file.
     *
     * @return The timestamp.
     *
     * @pre File::Exists( filePath ) == true.
     *
     * @details
     * @examples
     *
     * @snippet testFile.cpp ReadAllText
     */

    std::time_t GetLastModified( const std::string &path ) noexcept;

    /**
     * List al files in the given directory, either recursively or not recursive as indicated by the user. (defaults to
     * non-recursive)
     *
     * @param   directory Pathname of the directory.
     * @param   recursive whether the directory scan should be recursive.
     *
     * @return A vector with all the files in the directory.
     *
     * @pre Directory::Exists( directory ) == true.
     *
     * @post Since this function only gives a snapshot of the directory, it can be changed while processed.
     *
     * @details
     * @examples
     *
     * @snippet testFile.cpp List
     */

    std::vector< std::string > List( const std::string &directory, bool recursive = false ) noexcept;

    /// @}

    /// @name Modifiers
    /// @{

    /**
     * Sets the time on which the file is last modified.
     *
     * @param   path Full pathname of the file.
     * @param   time The time.
     *
     * @pre File::Exists( path ) == false.
     *
     * @post File::GetLastModified( path ) @f$ \approx @f$ time.
     *
     * @note Takes the filepath as-is.
     */

    void SetLastModified( const std::string &path, std::time_t time );

    /**
     * Clears the file from all data, if the file on the filepath exists, otherwise it won't do anything.
     *
     * @param   filepath The path to the file.
     *
     * @pre File::Exists( path ) == true.
     *
     * @details
     * @examples
     *
     * @snippet testFile.cpp Clear
     */

    void Clear( const std::string &filepath ) noexcept;

    /**
     * Deletes the file, if it exists, described by filepath.
     *
     * @param   filepath The filepath.
     *
     * @pre File::Exists( path ) == true.
     *
     * @details
     * @examples
     *
     * @snippet testFile.cpp Delete
     */

    void Delete( const std::string &filepath ) noexcept;

    /**
     * Moves a file from the given path to the new path. When there is already a file on the new path, the overwrite
     * parameter decides whether the old file will be overwritten or not. Also the from path and the to path can't be
     * the same.
     *
     * @param   from      The source of the file.
     * @param   to        Where to move the file to.
     * @param   overwrite Overwrite the file if it already exists.
     *
     * @return true if it succeeds, false if it fails.
     *
     * @pre
     * The following conditions will hold true:
     * * File::Exists( from ) == true.
     * * from @f$ \neq @f$ to.
     *
     * @post
     * The following conditions will hold true:
     * * File::Exists( from ) == false.
     * * File::Exists( to ) == true.
     *
     * @details
     * @examples
     *
     * @snippet testFile.cpp Move
     */

    bool Move( const std::string &from, const std::string &to, const bool overwrite = false );

    /**
     * Copies a file from the given path to other path. When there is already a file on the new path, the overwrite
     * parameter decides whether the old file will be overwritten or not. Also the from path and the to path can't be
     * the same.
     *
     * @param   from      The source of the file.
     * @param   to        Where to copy the file to.
     * @param   overwrite Overwrite the file if it already exists.
     *
     * @return true if it succeeds, false if it fails.
     *
     * @pre
     * The following conditions will hold true:
     * * File::Exists( from ) == true.
     * * from @f$ \neq @f$ to.
     *
     * @post
     * The following conditions will hold true:
     * * File::Exists( from ) == true.
     * * File::Exists( to ) == true.
     *
     * @details
     * @examples
     *
     * @snippet testFile.cpp Move
     *
     * @note Takes the filepath as-is.
     */

    bool Copy( const std::string &from, const std::string &to, bool overwrite = false );

    /// @}


    bool IsSharedLibrary( const std::string &s );

}

/// @}

#endif