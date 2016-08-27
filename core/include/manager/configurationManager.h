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

#pragma once
#ifndef __ENGINE_CONFIGURATIONMANAGER_H__
#define __ENGINE_CONFIGURATIONMANAGER_H__

#include "manager/abstract/abstractManager.h"

#include "common/configurationFile.h"

#include <unordered_set>

/// @addtogroup Configuration
/// @{

class ConfigurationManager
    : public AbstractManager
{
public:

    ConfigurationManager();

    /// @name Events
    /// @{

    void OnInit();
    void OnRelease();

    /// @}

    /// @name Get Value
    /// @{

    /**
     * Gets a string by a key from the configuration file with the given name.
     *
     * @param   key         The key.
     * @param   configName  (Optional) name of the configuration file, default is the main configuration
     *                      file.
     *
     * @return  The string, when the key does not exist it returns "".
     */

    std::string GetString( const std::string &key, const std::string &configName = "" );

    /**
     * Gets a bool by a key from the configuration file with the given name.
     *
     * @param   key         The key.
     * @param   configName  (Optional) name of the configuration.
     *
     * @return  The bool, when the key does not exist it returns false.
     */

    bool GetBool( const std::string &key, const std::string &configName = "" );

    /**
     * Gets a float by a key from the configuration file with the given name.
     *
     * @param   key         The key.
     * @param   configName  (Optional) name of the configuration file, default is the main configuration
     *                      file.
     *
     * @return  The float, when the key does not exist it returns 0.0f.
     */

    F32 GetFloat( const std::string &key, const std::string &configName = "" );

    /**
     * Gets an int by a key from the configuration file with the given name.
     *
     * @param   key         The key.
     * @param   configName  (Optional) name of the configuration file, default is the main configuration
     *                      file.
     *
     * @return  The int, when the key does not exist it returns 0.
     */

    S32 GetInt( const std::string &key, const std::string &configName = "" );

    /// @}

    /// @name Set Value
    /// @{

    /**
     * Sets a string value in the give configuration file on the given key.
     *
     * @param   key             The key.
     * @param   value           The value.
     * @param   changeInFile    Whether to reflect the changes in the configuration file.
     * @param   configName      (Optional) name of the configuration file, default is the main
     *                          configuration file.
     *
     * @return  true if it succeeds, false if it fails.
     */

    bool SetString( const std::string &key, const std::string &value, bool changeInFile = true,
                    const std::string &configName = "" );

    /**
     * Sets a bool value in the give configuration file on the given key.
     *
     * @param   key             The key.
     * @param   value           The value.
     * @param   changeInFile    Whether to reflect the changes in the configuration file.
     * @param   configName      (Optional) name of the configuration file, default is the main
     *                          configuration file.
     *
     * @return  true if it succeeds, false if it fails.
     */

    bool SetBool( const std::string &key, bool value, bool changeInFile = true,
                  const std::string &configName = "" );

    /**
     * Sets a float value in the give configuration file on the given key.
     *
     * @param   key             The key.
     * @param   value           The value.
     * @param   changeInFile    Whether to reflect the changes in the configuration file.
     * @param   configName      (Optional) name of the configuration file, default is the main
     *                          configuration file.
     *
     * @return  true if it succeeds, false if it fails.
     */

    bool SetFloat( const std::string &key, F32 value, bool changeInFile = true,
                   const std::string &configName = "" );

    /**
     * Sets an int value in the give configuration file on the given key.
     *
     * @param   key             The key.
     * @param   value           The value.
     * @param   changeInFile    Whether to reflect the changes in the configuration file.
     * @param   configName      (Optional) name of the configuration file, default is the main
     *                          configuration file.
     *
     * @return  true if it succeeds, false if it fails.
     */

    bool SetInt( const std::string &key, S32 value, bool changeInFile = true,
                 const std::string &configName = "" );

    /// @}

    /// @name Load Configuration
    /// @{

    /**
     * Loads the given configuration file type. This function will automatically create a new object and store it
     * correctly. It will also automaticaly call ConfigurationFile::OnRegisterKeys() and then ConfigurationFile::Load()
     *
     * @tparam  T   Class type that should be a derivative of ConfigurationFile.
     * @param   filePath    Full pathname of the file.
     * @param   configName  (Optional) name of the configuration, the default is the main configuration file.
     *
     * @return  null if it fails, else the created configuration file object.
     */

    template< class tT >
    tT *Load( const std::string &filePath, const std::string &configName = "" )
    {
        tT *file = new tT();
        LoadByObj( file, filePath, configName );
        return file;
    }

    /**
     * Loads a configuration file by an already created object. It will also automatically call
     * ConfigurationFile::OnRegisterKeys() and then ConfigurationFile::Load()
     *
     * @param [in,out]  conf    If non-null, the conf.
     * @param   filePath        Full pathname of the file.
     * @param   configName      (Optional) name of the configuration.
     *
     * @return  true if it succeeds, false if it fails.
     */

    bool LoadByObj( ConfigurationFile *const conf, const std::string &filePath,
                    const std::string &configName = "" );

    /// @}


    std::string GetConfigurationDirectory() const;

    void SetGlobalConfigFileName( const std::string &name );

private:

    // Allows us to obtain a configuration file by its name
    std::unordered_map< std::string, ConfigurationFile * > mConfigurationFileNameMap;
    // Holds the configuration files that are loaded and passed by object
    std::unordered_set< ConfigurationFile * > mConfigurationFileObjectSet;

    std::string mConfigFile;

    mutable std::recursive_mutex mMutex;


    static void LogErrors( ConfigurationFile::ParseError error, const std::string &configName, const std::string &key = "",
                           std::size_t lineNumber = 0, const std::string &value = "" );
};



/// @}

#endif

