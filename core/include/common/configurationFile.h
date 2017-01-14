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
#ifndef __ENGINE_CONFIGURATIONFILE_H__
#define __ENGINE_CONFIGURATIONFILE_H__

#include "common/types.h"

#include <unordered_map>
#include <vector>
#include <string>

/// @addtogroup docConfiguration
/// @{

/// @addtogroup docConfiguration_File
/// @{

/**
 * This class holds the keys and values of a configuration file.
 *
 * The class should be loaded using the configuration manager, which then makes the key-values
 * available to all classes with access to the configuration manager.
 *
 * Configuration files will be parsed by this class. Missing value keys will be added to the
 * existing file, meaning even non existing files will be 'parsed' and a configuration file will
 * be output.
 *
 * An example configuration file:
 * @code
 * # this is a comment
 * stringValue = "this is a string key value pair"
 *
 * intValue = 10
 *
 * floatValue = 1.2
 *
 * boolValue = true# even comments like this are accepted.
 *
 * # this is
 * # a multi line comment
 * stringValue2 = "hello world"
 * @endcode
 *
 * Which can be generated using the following ConfigurationFile class:
 * @anchor TestConfiguration
 * @snippet testConfigurationFile.cpp TestConfiguration Example
 *
 * As you probably noticed, the ConfigurationFile class doesn't initially know its name nor its
 * location. Those properties will be set when the configuration file is loaded.
 *
 * The parser will read only the first key of the same name. Unset keys will be added with the
 * given documentation. Invalid key values will be set to their default values. Invalid lines,
 * and unknown keys, will be ignored while outputting the warnings on how to fix the
 * configuration file.
 *
 * @sa ConfigurationManager
 */

class ConfigurationFile
{
public:

    /// @name Error Handling
    /// @{

    /**
     * Values that represent parse errors. For a more detail description of its usage see '@ref
     * Load()'.
     */

    enum class ParseError
    {
        /// No error has occurred.
        None             = 0x00,

        /// No separator ('=') was found while parsing a line.
        NoSeperator      = 0x01,

        /// A parsed key is not registered.
        NotRegistered    = 0x02,

        /// A particular key was found more than once.
        IsDuplicateKey   = 0x03,

        /// There is a line, where the key is missing.
        NoKey            = 0x04,

        /// There is a line, where the value is missing.
        NoValue          = 0x05,

        /// A string value failed to parse.
        InvalidString    = 0x06,

        /// A bool value failed to parse.
        InvalidBool      = 0x07,

        /// A float value failed to parse.
        InvalidFloat     = 0x08,

        /// An int value failed to parse.
        InvalidInt       = 0x09,

        /// The request key is invalid.
        InvalidKey       = 0x0a,

        /// Fixing a configuration file failed.
        FixingFailed     = 0x0b,

        /// The configuration file could not be opened.
        FailedToOpenFile = 0x0c
    };

    /**
     * An alias for the error handling function we can define in the 'Load()' function.
     *
     * An example of an error handling function can be similar to:
     * @snippet testConfigurationFile.cpp ErrorHandling
     *
     * This of course is a very simple, non functional function, but we hope you get the idea from this.
     */

    typedef void ( *ErrorHandler )( ParseError error, const std::string &configName, const std::string &key,
                                    std::size_t lineNumber, const std::string &value );

    /// @}

    ConfigurationFile() noexcept;

    virtual ~ConfigurationFile() noexcept;

    /// @name Lookup
    /// @{

    /**
     * Gets the string value from a key.
     *
     * @param   key The key.
     *
     * @return The stored string.
     *
     * @pre The given key exists, and is a string key. When not met "" will be returned.
     *
     * @details
     * @examples
     *
     * Given the example '@ref TestConfiguration' class:
     * @snippet testConfigurationFile.cpp GetString
     */

    std::string GetString( const std::string &key ) const noexcept;

    /**
     * Gets the bool from a key.
     *
     * @param   key The key.
     *
     * @return The stored bool.
     *
     * @pre The given key exists, and is a bool key. When not met 'false' will be returned.
     *
     * @details
     * @examples
     *
     * Given the example '@ref TestConfiguration' class:
     * @snippet testConfigurationFile.cpp GetBool
     */

    bool GetBool( const std::string &key ) const noexcept;

    /**
     * Gets the float from a key.
     *
     * @param   key The key.
     *
     * @return The stored float.
     *
     * @pre The given key exists, and is a float key. When not met '0.0f' will be returned.
     *
     * @details
     * @examples
     *
     * Given the example '@ref TestConfiguration' class:
     * @snippet testConfigurationFile.cpp GetFloat
     */

    F32 GetFloat( const std::string &key ) const noexcept;

    /**
     * Gets the int from a key.
     *
     * @param   key The key.
     *
     * @return The stored int.
     *
     * @pre The given key exists, and is an int key. When not met '0' will be returned.
     *
     * @details
     * @examples
     *
     * Given the example '@ref TestConfiguration' class:
     * @snippet testConfigurationFile.cpp GetInt
     */

    S32 GetInt( const std::string &key ) const noexcept;

    /// @}

    /// @name Queries
    /// @{
    //

    /**
     * Query if the given key is a string key.
     *
     * @param   key The key.
     *
     * @return true if it is a string key, false if not.
     *
     * @details
     * @examples
     *
     * Given the example '@ref TestConfiguration' class:
     * @snippet testConfigurationFile.cpp IsStringKey
     */

    bool IsStringKey( const std::string &key ) const noexcept;

    /**
     * Query if the given key is a float key.
     *
     * @param   key The key.
     *
     * @return true if it is a float key, false if not.
     *
     * @details
     * @examples
     *
     * Given the example '@ref TestConfiguration' class:
     * @snippet testConfigurationFile.cpp IsFloatKey
     */

    bool IsFloatKey( const std::string &key ) const noexcept;

    /**
     * Query if the given key is a bool key.
     *
     * @param   key The key.
     *
     * @return true if it is a bool key, false if not.
     *
     * @details
     * @examples
     *
     * Given the example '@ref TestConfiguration' class:
     * @snippet testConfigurationFile.cpp IsBoolKey
     */

    bool IsBoolKey( const std::string &key ) const noexcept;

    /**
     * Query if the given key is an int key.
     *
     * @param   key The key.
     *
     * @return true if it is an int key, false if not.
     *
     * @details
     * @examples
     *
     * Given the example '@ref TestConfiguration' class:
     * @snippet testConfigurationFile.cpp IsIntKey
     */

    bool IsIntKey( const std::string &key ) const noexcept;

    /**
     * Query if the given key is a valid key.
     *
     * @param   key The key.
     *
     * @return true if is valid, false if not.
     *
     * @details
     * @examples
     *
     * Given the example '@ref TestConfiguration' class:
     * @snippet testConfigurationFile.cpp HasKey
     */

    bool HasKey( const std::string &key ) const noexcept;

    /// @}

    /// @name Modifiers
    /// @{

    /**
     * Set a string value on a key.
     *
     * @param   key          The key.
     * @param   value        The value.
     * @param   changeInFile Reflect the change in the physical configuration file.
     *
     * @return The parse error code.
     *
     * @pre The key is a registered string key, otherwise this call will be ignored.
     *
     * @post
     * The following conditions hold true:
     * * GetString( key ) returns the new value.
     * * When changeInFile is true, the configuration file is updated to
     *     reflect the new value.
     *
     * @details
     * @examples
     *
     * Given the example '@ref TestConfiguration' class:
     * @snippet testConfigurationFile.cpp NewValues
     */

    ParseError SetString( const std::string &key, const std::string &value, bool changeInFile = true );

    /**
     * Sets a bool value on a key.
     *
     * @param   key          The key.
     * @param   value        The value.
     * @param   changeInFile Reflect the change in the physical configuration file.
     *
     * @return The parse error code.
     *
     * @pre The key is a registered bool key, otherwise this call will be ignored.
     *
     * @post
     * * GetBool( key ) returns the new value.
     * * When changeInFile is true, the configuration file is updated to
     *   reflect the new value.
     *
     * @details
     * @examples
     *
     * Given the example '@ref TestConfiguration' class:
     * @snippet testConfigurationFile.cpp NewValues
     */

    ParseError SetBool( const std::string &key, const bool value, bool changeInFile = true );

    /**
     * Sets a float value on a key.
     *
     * @param   key          The key.
     * @param   value        The value.
     * @param   changeInFile Reflect the change in the physical configuration file.
     *
     * @return The parse error code.
     *
     * @pre The key is a registered float key, otherwise this call will be ignored.
     *
     * @post
     * * GetFloat( key ) returns the new value.
     * * When changeInFile is true, the configuration file is updated to
     *   reflect the new value.
     *
     * @details
     * @examples
     *
     * Given the example '@ref TestConfiguration' class:
     * @snippet testConfigurationFile.cpp NewValues
     */

    ParseError SetFloat( const std::string &key, const F32 value, bool changeInFile = true );

    /**
     * Sets an int value on a key.
     *
     * @param   key          The key.
     * @param   value        The value.
     * @param   changeInFile Reflect the change in the physical configuration file.
     *
     * @return The parse error code.
     *
     * @pre The key is a registered int key, otherwise this call will be ignored.
     *
     * @post
     * * GetInt( key ) returns the new value.
     * * When changeInFile is true, the configuration file is updated to
     *   reflect the new value.
     *
     * @details
     * @examples
     *
     * Given the example '@ref TestConfiguration' class:
     * @snippet testConfigurationFile.cpp NewValues
     */

    ParseError SetInt( const std::string &key, const S32 value, bool changeInFile = true );

    /// @}

    /// @name Adding Keys
    /// @{

    /**
     * Adds a string key. Multi line comments will be converted automatically.
     *
     * @param   key          The key.
     * @param   defaultValue The default value.
     * @param   comment      The comment that will be written in the configuration file, if the key gets fixed.
     *
     * @return true if it succeeds, false if it fails.
     *
     * @pre The keys is not yet registered, otherwise this call will be ignored.
     *
     * @post
     * * HasKey( key ) returns true.
     * * IsStringKey( key ) returns true.
     * * GetString( key ) returns the default value.
     *
     * @details
     * @examples
     *
     * Given the example '@ref TestConfiguration' class:
     * @snippet testConfigurationFile.cpp AddStringKey
     */

    bool AddStringKey( const std::string &key, const std::string &defaultValue, const std::string &comment = "" ) noexcept;

    /**
     * Adds a bool key. Multi line comments will be converted automatically.
     *
     * @param   key          The key.
     * @param   defaultValue The default value.
     * @param   comment      The comment that will be written in the configuration file, if the key gets fixed.
     *
     * @return true if it succeeds, false if it fails.
     *
     * @pre The keys is not yet registered, otherwise this call will be ignored.
     *
     * @post
     * * HasKey( key ) returns true.
     * * IsBoolKey( key ) returns true.
     * * GetBool( key ) returns the default value.
     *
     * @details
     * @examples
     *
     * Given the example '@ref TestConfiguration' class:
     * @snippet testConfigurationFile.cpp AddBoolKey
     */

    bool AddBoolKey( const std::string &key, const bool defaultValue, const std::string &comment = "" ) noexcept;

    /**
     * Adds a float key. Multi line comments will be converted automatically.
     *
     * @param   key          The key.
     * @param   defaultValue The default value.
     * @param   comment      The comment that will be written in the configuration file, if the key gets fixed.
     *
     * @return true if it succeeds, false if it fails.
     *
     * @pre The keys is not yet registered, otherwise this call will be ignored.
     *
     * @post
     * * HasKey( key ) returns true.
     * * IsFloatKey( key ) returns true.
     * * GetFloat( key ) returns the default value.
     *
     * @details
     * @examples
     *
     * Given the example '@ref TestConfiguration' class:
     * @snippet testConfigurationFile.cpp AddFloatKey
     */

    bool AddFloatKey( const std::string &key, const F32 defaultValue, const std::string &comment = "" ) noexcept;

    /**
     * Adds an int key. Multi line comments will be converted automatically.
     *
     * @param   key          The key.
     * @param   defaultValue The default value.
     * @param   comment      The comment that will be written in the configuration file, if the key gets fixed.
     *
     * @return true if it succeeds, false if it fails.
     *
     * @pre The keys is not yet registered, otherwise this call will be ignored.
     *
     * @post
     * * HasKey( const std::string &key ) returns true.
     * * IsIntKey( const std::string &key ) returns true.
     * * GetInt( const std::string &key ) returns the default value.
     *
     * @details
     * @examples
     *
     * Given the example '@ref TestConfiguration' class:
     * @snippet testConfigurationFile.cpp AddIntKey
     */

    bool AddIntKey( const std::string &key, const S32 defaultValue, const std::string &comment = "" ) noexcept;

    /// @}

    /// @name Loading
    /// @{

    /**
     * Loads the configuration file in memory and parses it.
     *
     * @param   filePath     Full pathname of the file.
     * @param   configName   Name of the configuration, which will be used in the error handler calls.
     * @param   errorHandler The error handler, when nullptr no function will be called on errors.
     *
     * @return true if it succeeds, false if it fails.
     *
     * @pre The following conditions should be met:
     *      * All required keys are registered.
     *      * Load() hasn't been called before.
     *
     * @pre Here follows the list of possible errors we can capture, and how they occur:
     *      * ParseError::NoSeperator, is given when a line without seperator ('=') is found. This
     *        error is non-critical and the line will be ignored.
     *      * ParseError::NotRegistered, is given when a line with a key name, that is not
     *        registered within the ConfigurationFile is given. The error is non-critical and the line will be ignored.
     *        This is especially useful when you remove the need of old configuration settings.
     *      * ParseError::IsDuplicateKey, the current keyname was already read in the file. We
     *        ignore this line and the error is non-critical.
     *      * ParseError::NoKey, we read a line where no key was given, the error is non-critical and
     *        we ignore this line.
     *      * ParseError::NoValue, we read a line where no value was given, the error is non-critical
     *        and we ignore this line.
     *      * ParseError::InvalidString, we expected to read a string, but the found value did not
     *        correspond. The error is non-critical and we ignore this line.
     *      * ParseError::InvalidBool, we expected to read a bool, but the found value did not
     *        correspond. The error is non-critical and we ignore this line.
     *      * ParseError::InvalidFloat, we expected to read a float, but the found value did not
     *        correspond. The error is non-critical and we ignore this line.
     *      * ParseError::InvalidInt, we expected to read a integer, but the found value did not
     *        correspond. The Error is non-critical and we ignore this line.
     *      * ParseError::InvalidKey, we tried to set a key that did not correspond to the given type,
     *        or did not exist at all. The error is non-critical, and no value will be altered/set.
     *      * ParseError::FixingFailed, we tried to fix the configuration file and update it with the
     *        correct key/value pairs, but we failed. ParseError::FixingFailed is currently not used.
     *      * ParseError::FailedToOpenFile, we could not open the configuration file. We will only
     *        read the default values.
     *
     * @post
     * The following conditions are met on a successful load:
     * * When the configuration file didn't exist, a new one with
     *   the given default values is written.
     * * When the configuration file did exist:
     *      - When uncorrupted, the values from the configuration file
     *        will be loaded on our keys.
     *      - When corrupted, we try to fix the corrupt key value pairs.
     *        If the value is too corrupt to read, we will stop loading, otherwise we re-add the key with the
     *        default value at the end of the file.
     *
     * @details
     * @examples
     *
     * Given the example @ref TestConfiguration class:
     * @snippet testConfigurationFile.cpp Load
     */

    bool Load( const std::string &filePath, const std::string &configName, ErrorHandler errorHandler = nullptr );

    /// @}

private:

    /// The configuration name
    std::string mConfigName;

    /// The path to the physical representation of the
    /// configuration file
    std::string mFilePath;

    /// Holds the float keys and values
    std::unordered_map< std::string, F32 > mFloats;

    /// Holds the int keys and values
    std::unordered_map< std::string, S32 > mInts;

    /// Holds the bool keys and values
    std::unordered_map< std::string, bool > mBools;

    /// Holds the string keys and their values
    std::unordered_map< std::string, std::string > mStrings;

    /// Maps whether keys are used so we can decide later on how we can fix unset keys, also it hold
    /// whether a key is read or not.
    std::unordered_map< std::string, bool > mKeysUsed;

    /// Holds the keys and their comments to describe in the physical configuration file what a key
    /// does in the engine.
    std::unordered_map< std::string, std::string >  mComments;

    /// Holds all the keys we stored
    std::vector< std::string > mKeys;

    /// Make the class un-assignable
    ConfigurationFile &operator=( const ConfigurationFile & );
    ConfigurationFile( const ConfigurationFile & );

    /**
     * Parses the given line and add it to the stored configuration.
     *
     * @param   line         The line.
     * @param   lineNumber   The line number.
     * @param   commentStart The comment start.
     * @param   errorHandler The error handler.
     *
     * @return Whether we can continue parsing.
     */

    bool ParseLine( const std::string &line, const size_t lineNumber, const size_t commentStart,
                    ErrorHandler errorHandler );

    /**
     * Change value on the given key.
     *
     * @param   key   The key.
     * @param   value The value.
     *
     * @return true if it succeeds, false if it fails.
     */

    bool ChangeKey( const std::string &key, const std::string &value ) const;

    /**
     * Adds unset keys in the physical configuration file with their default value.
     *
     * @param   lastLine  The last line of the configuration file.
     * @param   lineCount Number of lines in the configuration file.
     *
     * @return true if it succeeds, false if it fails.
     */

    bool FixUnsetKeys( const std::string &lastLine );

    /**
     * Gets the position of the comment in the given line.
     *
     * @param   line The line.
     *
     * @return The comment position, std::string::npos if the line doensn't have a comment.
     */

    static size_t GetCommentPosition( const std::string &line ) noexcept;

    /**
     * Removes the comment in the given line.
     *
     * @param [in,out]  line The line.
     *
     * @return The position of the comment, std::string::npos if the line didn't have a comment.
     */

    static size_t RemoveComment( std::string &line ) noexcept;

};

/// @}

/// @}

#endif