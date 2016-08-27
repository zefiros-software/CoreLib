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
#ifndef __ENGINE_CONFIGURATION_H__
#define __ENGINE_CONFIGURATION_H__

#include "common/types.h"

#include <string>

class ConfigurationFile;

/// @addtogroup Configuration
/// @{

namespace Configuration
{
    std::string GetProgramName();

    std::string GetProgramCompany();

    std::string GetCopyright();

    void SetGlobalConfig( const std::string &path );

    /// @name Get Value
    /// @{

    /**
     * @see ConfigurationManager::GetString()
     */

    std::string GetString( const std::string &key, const std::string &configName = "" );

    /**
     * @see ConfigurationManager::GetBool()
     */

    bool GetBool( const std::string &key, const std::string &configName = "" );

    /**
     * @see ConfigurationManager::GetFloat()
     */

    F32 GetFloat( const std::string &key, const std::string &configName = "" );

    /**
     * @see ConfigurationManager::GetInt()
     */

    S32 GetInt( const std::string &keyin, const std::string &configName = "" );

    /// @}

    /// @name SetValue
    /// @{

    /**
     * @see ConfigurationManager::SetString()
     */

    void SetString( const std::string &key, const std::string &value, const bool changeInFile,
                    const std::string &configName = "" );

    /**
     * @see ConfigurationManager::SetBool()
     */

    void SetBool( const std::string &key, const bool value, const bool changeInFile,
                  const std::string &configName = "" );

    /**
     * @see ConfigurationManager::SetFloat()
     */

    void SetFloat( const std::string &key, const F32 value, const bool changeInFile,
                   const std::string &configName = "" );

    /**
     * @see ConfigurationManager::SetInt()
     */

    void SetInt( const std::string &key, const S32 value, const bool changeInFile,
                 const std::string &configName = "" );

    /// @}

    /**
     * Loads the given configuration file.
     *
     * @param   configurationFile   The configuration file.
     * @param   filePath            Full pathname of the file.
     * @param   configName          Name of the configuration.
     *
     * @note    Should only be called once in the lifetime of
     *          the configuration file.
     */

    void Load( ConfigurationFile *const configurationFile, const std::string &filePath, const std::string &configName );
}

/// @}

#endif