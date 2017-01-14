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
#ifndef __ENGINE_PATH_H__
#define __ENGINE_PATH_H__

#include "external/filesystem.h"

#include <vector>

/// @addtogroup docFilesystem
/// @{

namespace Path
{
    /// @name Path Location
    /// @{

    /**
     * We use these types to set where a file specifically should be created/read. Doing this for every io operation, we
     * get a working directory independent program, which is much needed for the editors. The base paths are all
     * guaranteed to exists.
     */

    enum class Type
    {
        /// Refers to the game folder, only the editor
        /// should be allowed to write here, the engine
        /// may only read from this folder, since
        /// windows requires admin permissions on the
        /// program files folder we don't know whether
        /// we have the actual write permissions.
        Program        = 0x01,

        /// Should be used for all user specific files.
        Data        = 0x02,

        /// Should be used for all shared files.
        SharedData  = 0x03,

        /// Should be used for temporary files. Please
        /// note that the files should not be considered persistent
        /// between sessions.
        Temp        = 0x04,

        /// Reserved for the editor folder. When we run
        /// the engine as standalone, this folder won't
        /// actually exist.
        Editor      = 0x05,

        /// Uses the folder as given by the uses, without trying
        /// to parse or doing fancy operations on the path.
        None        = 0x06
    };

    /**
     * Convert the user given relative path, to a path to the correct path type. For example when given a relative path,
     * with as type Type::Temp, the path returned will set the path to the temporary folder.
     *
     * All file operations should use this function either directly or indirectly.
     *
     * @param   path The relative path, it defaults to the game directory.
     * @param   type The path type.
     *
     * @return The correct path.
     */

    std::string Get( const std::string &path, Type type = Type::Program ) noexcept;

    /// @}

    /// @name Queries
    /// @{

    /**
     * Resolve a relative path from a directory to an other directory, and return the full relative path. We can choose
     * whether the given to path starts relative from the parent path on the from path.
     *
     * Example:
     * @code
     * std::string from = "module/ZefirosEngine/main.as";
     * std::string to   = "behaviour.as";
     * std::string path = Path::ResolveRelativePath( from, to ); // path holds "module/ZefirosEngine/behaviour.as"
     * std::string path = Path::ResolveRelativePath( from, to, false ); // path holds "../../behaviour.as"
     * @endcode
     *
     * @param   from     The path from which the relative path starts.
     * @param   to       The given relative path.
     * @param   sameRoot Whether to 'to' path starts from the parent path of the 'from' path.
     *
     * @return The relative path from the 'from' path to the 'to' path.
     */

    std::string ResolveRelative( const std::string &from, const std::string &to, bool sameRoot = true ) noexcept;

    /**
     * Fixes the path style to the unix style, since that is the one we use internally. All directories will have a
     * slash appended, so it is recognised as a directory path.
     *
     * @param   filePath The filepath.
     *
     * @return The fixed path style.
     */

    std::string FixStyle( const std::string &filePath ) noexcept;

    /**
     * Normalises the path.
     *
     * Meaning:
     *  * '../' will go one up in the structure.
     *  * './' stays at the same path.
     *
     * We can choose whether the result is given in absolute form, if not we assume the resulting file is at the root so,
     * "../../file.txt" would be the same as "./file.txt".
     *
     * @param   path     Full pathname of the file.
     * @param   absolute True if the resulting path should be absolute.
     *
     * @return A canonicalised (normalised) path.
     */

    std::string Canonical( const std::string &path, bool absolute = false ) noexcept;

    /**
     * Query if 'from' is a parent path of 'to'.
     *
     * @param   from The first path.
     * @param   to   The second path.
     *
     * @return True if parent, false if not.
     */

    bool IsParent( const std::string &from, const std::string &to ) noexcept;

    /**
     * Gets the file name part from a full path.
     *
     * Example:
     * @code
     * FileSystem::GetFileName( "./modules/imaginary.file" ); // returns "imaginary.file"
     * FileSystem::GetFileName( "./modules/imaginary.file", true ); // returns "imaginary"
     * @endcode
     *
     * @param   path           The path to get the file name part from.
     * @param   stripExtension Whether to strip the extension from the returned filename or not.
     *
     * @return The file name part.
     */

    std::string GetFileName( const std::string &path, const bool stripExtension = false ) noexcept;

    /**
     * Gets the directory part from a full path.
     *
     * Example:
     * @code
     * FileSystem::GetDirectory( "./modules/imaginary.file" ); // returns "./modules/"
     * @endcode
     *
     * @param   path    The path to get the directory part from.
     *
     * @return  The directory part.
     */

    std::string GetDirectory( const std::string &path ) noexcept;

    /**
     * Gets an extension, either with or without the ".", from a given file path. When the file has no extension, we
     * will return "".
     *
     * Example:
     * @code
     * FileSystem::GetExtension( "./modules/imaginary.file" ); // returns "file"
     * FileSystem::GetExtension( "./modules/imaginary.file", true ); // returns ".file"
     * @endcode
     *
     * @param   filepath The filepath.
     * @param   addDot   Whether you want a dot to be added to the extension.
     *
     * @return The extension.
     */

    std::string GetExtension( const std::string &filepath, const bool addDot = false ) noexcept;

    /**
     * Query if the 'filepath' has an extension.
     *
     * @param   filepath The filepath.
     *
     * @return True if it has an extension, false if not.
     */

    bool HasExtension( const std::string &filepath ) noexcept;

    /**
     * Generates a statistically guaranteed unique file name, with the given extension. (insert Murhpy's law joke here)
     *
     * @param   extension The extension.
     *
     * @return The unique file name.
     *
     * @note When an empty extension is given, "", the function will return a directory path.
     */

    std::string GetUniqueFileName( const std::string &extension = ".tmp" ) noexcept;

    /**
     * Generates a statistically guaranteed unique directory.
     *
     * @return The unique directory.
     */

    std::string GetUniqueDirectory() noexcept;

    /**
     * Gets the OS provided temporary directory, please note that files are not to be expect to be kept between two
     * sessions.
     *
     * On Windows:
     *      C:/windows/temp/
     *
     * On OSX:
     *      /tmp/
     *
     * On Linux:
     *      /tmp/.
     *
     * @return The temporary directory.
     */

    std::string GetTempDirectory() noexcept;

    /**
     * Gets the directory from where the exe resides. Do not use this function as alternative to the GetGameDirectory()
     * function, since that will break the editors, and unit tests.
     *
     * @return The executable directory.
     */

    std::string GetExeDirectory() noexcept;

    /**
     * Gets the full path to the exe. Do not use this function as alternative to the GetGameDirectory() function, since
     * that will break the editors, and unit tests.
     *
     * @return The executable directory.
     */

    std::string GetExeFile() noexcept;

    /**
     * Gets the directory provided by the OS wherein we should store all user specific files.
     *
     * On Windows:
     *      C:/Users/<User name>/AppData/Roaming/
     *
     * On OSX:
     *      ~/Library/Application Support/
     *
     * On Linux:
     *      $XDG_DATA_HOME.
     *
     * @return The data directory.
     */

    std::string GetDataDirectory() noexcept;

    /**
     * Gets the directory provided by the OS wherein we should store all shared data.
     *
     * On Windows:
     *      C:/ProgramData/
     *
     * On OSX:
     *      /Library/Application Support/
     *
     * On Linux:
     *      /var/games/.
     *
     * @return The shared data directory.
     */

    std::string GetSharedDataDirectory() noexcept;

    /**
     * Gets the root directory of our game.
     *
     * @return The game directory.
     *
     * @note
     * Use this folder only for read-only actions, since windows doesn't like programs to write in the program files
     * folder when UAC is enabled. Writing should be done in either the Data or the Shared Data folders. An
     * exception should be made for the editors, because we require the editors not to be installed in the program
     * files directory. TL;DR - Windows Sucks.
     */

    std::string GetProgramDirectory() noexcept;

    /**
     * Gets the game specific temporary directory.
     *
     * The path is constructed like:
     *      <Temporary Directory>/<Game Company>/<Game Name>/.
     *
     * @return The game temporary directory.
     */

    std::string GetProgramTempDirectory() noexcept;

    /**
     * Gets the game specific data directory.
     *
     * The path is constructed like:
     *      <Data Directory>/<Game Company>/<Game Name>/
     *
     * @return  The game data directory.
     */

    std::string GetProgramDataDirectory() noexcept;

    /**
     * Gets the game specific shared data directory.
     *
     * The path is constructed like:
     *      <Shared Data Directory>/<Game Company>/<Game Name>/.
     *
     * @return The game shared data directory.
     */

    std::string GetProgramSharedDataDirectory() noexcept;

    /**
     * List all content, directories and files, in the given directory.
     *
     * @param   directory Pathname of the directory.
     * @param   recursive True to process recursively, false to process locally only.
     *
     * @return A vector with all the files and directories in the directory.
     *
     * @note
     * The content can be removed/altered while you process them, so make sure the file does exists if you do
     * anything with it.
     */

    std::vector< boost::filesystem::path > List( const std::string &directory, bool recursive = false ) noexcept;

    /**
     * List directory content, directories and files, in a given directory.
     *
     * @param   directory Pathname of the directory.
     * @param   recursive True to process recursively, false to process locally only.
     *
     * @return A vector with all the files and directories strings in the directory.
     *
     * @note
     * The content can be removed/altered while you process them, so make sure the file does exists if you do
     * anything with it.
     */

    std::vector< std::string > ListContent( const std::string &directory, bool recursive = false ) noexcept;

    /**
     * Gets the current working directory.
     *
     * @return  The working directory.
     */

    std::string GetWorkingDirectory() noexcept;

    /// @}

    /// @name Modifiers
    /// @{

    /**
     * Sets the current working directory.
     *
     * @param   workingDirectory Pathname of the working directory.
     *
     * @return true if it succeeds, false if it fails.
     */

    bool SetWorkingDirectory( const std::string &workingDirectory ) noexcept;

    /**
     * Recursively deletes the given path, deleting both files and directories.
     *
     * @param   path Full pathname of the file.
     */

    void DeleteAll( const std::string &path ) noexcept;

    /// @}
}

/// @}

#endif