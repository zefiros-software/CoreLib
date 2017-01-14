/**
 * @cond ___LICENSE___
 *
 * Copyright (c) 2017 Koen Visscher, Paul Visscher and individual contributors.
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
#ifndef __ENGINE_DIRECTORY_H__
#define __ENGINE_DIRECTORY_H__

#include <vector>
#include <string>

/// @addtogroup docFilesystem
/// @{

/**
 * %Namespace collection of functions that handles all the directory (filesystem) specific functions.
 */

namespace Directory
{
    /// @name Queries
    /// @{

    /**
     * Queries if a given directory exists, it will return false on file paths.
     *
     * @param   directory Pathname of the directory.
     *
     * @return true if it exists, false if it doesn't.
     *
     * @pre
     * The following conditions should be met:
     * * The path is given in unix style.
     *
     * @details
     * @examples
     *
     * @snippet testDirectory.cpp Exists
     */

    bool Exists( const std::string &directory ) noexcept;

    /**
     * Queries if a given directory is empty.
     *
     * @param   directory          Pathname of the directory.
     * @param   includeDirectories true if we want to include directories in the query.
     *
     * @return true if empty, false if not.
     *
     * @pre Directory::Exists( directory ) == true.
     *
     * @details
     * @examples
     *
     * @snippet testDirectory.cpp IsEmpty
     */

    bool IsEmpty( const std::string &directory, bool includeDirectories = true ) noexcept;

    /**
     * List directories in a given directory.
     *
     * @param   directory Path of the directory we want to scan.
     * @param   recursive Whether we want the scan to be recursively or not, default is false.
     *
     * @return A vector with all the directories in the directory.
     *
     * @pre Directory::Exists( directory ) == true.
     *
     * @post Since this function only gives a snapshot of the directory, it can be changed while processed.
     *
     * @details
     * @examples
     *
     * @snippet testDirectory.cpp List
     */

    std::vector< std::string > List( const std::string &directory, bool recursive = false ) noexcept;

    /// @}

    /// @name Modifiers
    /// @{

    /**
     * Deletes the given filepath, does nothing when the directory didn't exist. Naturally it won't delete file paths.
     *
     * @param   directory The path we want to delete.
     *
     * @pre
     * The following conditions should be met:
     * * Directory::Exists( directory ) == true.
     * * Path::List( directory ).empty() == true.
     *
     * @post Directory::Exists( directory ) == false.
     *
     * @details
     * @examples
     *
     * @snippet testDirectory.cpp Delete
     */

    bool Delete( const std::string &directory ) noexcept;

    /**
     * Deletes the given directory recursively, does nothing when the directory didn't exist. Naturally it won't delete
     * file paths.
     *
     * @param   directory The path we want to delete.
     *
     * @return true if it succeeds, false if it fails.
     *
     * @pre Directory::Exists( directory ) == true.
     *
     * @post Directory::Exists( directory ) == false.
     *
     * @details
     * @examples
     *
     * @snippet testDirectory.cpp DeleteAll
     */

    bool DeleteAll( const std::string &directory ) noexcept;

    /**
     * Creates the given directory. The uppermost parent path should exist, for the directory to be created.
     *
     * @param   directory Pathname of the directory.
     *
     * @return true if it succeeds, false if it fails.
     *
     * @pre
     * The following conditions should be met:
     * * Directory::Exists( directory ) == false.
     * * Directory::Exists( parent( directory ) ) == true.
     *
     * @post Directory::Exists( directory ) == true.
     *
     * @details
     * @examples
     *
     * @snippet testDirectory.cpp Create
     */

    bool Create( const std::string &directory ) noexcept;

    /**
     * Creates all directories in the given path.
     *
     * @param   directories The directory path, which we should fully create.
     *
     * @return true if it succeeds, false if it fails.
     *
     * @pre Directory::Exists( directory ) == false.
     *
     * @post Directory::Exists( directory ) == true.
     *
     * @details
     * @examples
     *
     * @snippet testDirectory.cpp CreateAll
     */

    bool CreateAll( const std::string &directories ) noexcept;

    /**
     * Copies the given directory, to an other directory. Won't do anything when the from directory doesn't exist, or
     * when the 'to' directory already exists.
     *
     * @param   from The source of the directory.
     * @param   to   Where to copy the directory to.
     *
     * @return true if it succeeds, false if it fails.
     *
     * @pre
     * The following conditions should be met:
     * * Directory::Exists( from ) == true.
     * * Directory::Exists( to ) == false.
     * * from @f$ \neq @f$ to.
     *
     * @post
     * The following conditions are met:
     * * Directory::Exists( to ) == true.
     * * Directory::IsEmpty( to, true ) == true.
     *
     * @details
     * @examples
     *
     * @snippet testDirectory.cpp Copy
     */

    bool Copy( const std::string &from, const std::string &to ) noexcept;

    /**
     * Moves the given directory, and all of its contents, to an other directory. Won't do anything when the from
     * directory doesn't exist, or when the 'to' directory already exists.
     *
     * @param   from The source of the directory.
     * @param   to   Where to move the directory to.
     *
     * @return true if it succeeds, false if it fails.
     *
     * @pre
     * The following conditions should be met:
     *  * Directory::Exists( from ) == true.
     *  * Directory::Exists( to ) == false.
     *  * from @f$ \neq @f$ to.
     *
     * @post
     * The following conditions are met:
     *  * Directory::Exists( to ) == true.
     *  * if Directory::IsEmpty( from, true ) == false:
     *       Directory::IsEmpty( to, true )
     *    otherwise:
     *       Directory::IsEmpty( to, true ) == true.
     *
     * @details
     * @examples
     *
     * @snippet testDirectory.cpp Move
     */

    bool Move( const std::string &from, const std::string &to ) noexcept;

    /// @}

}

/// @}

#endif