/**
 * @cond ___LICENSE___
 *
 * Copyright (c) 2016-2018 Zefiros Software.
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

#include "manager/configurationManager.h"

#include "api/expose.h"


class ConfigurationFile;

/// @addtogroup Configuration
/// @{

namespace Configuration
{
    EXPOSE_API(configuration, SetGlobalConfig);

    /// @name Get Value
    /// @{

    /**
     * @see ConfigurationManager::GetString()
     */


    EXPOSE_API(configuration, GetString);

    /**
     * @see ConfigurationManager::GetBool()
     */

    EXPOSE_API(configuration, GetBool);

    /**
     * @see ConfigurationManager::GetFloat()
     */

    EXPOSE_API(configuration, GetFloat);

    /**
     * @see ConfigurationManager::GetInt()
     */

    EXPOSE_API(configuration, GetInt);

    /// @}

    /// @name SetValue
    /// @{

    /**
     * @see ConfigurationManager::SetString()
     */

    EXPOSE_API(configuration, SetString);

    /**
     * @see ConfigurationManager::SetBool()
     */

    EXPOSE_API(configuration, SetBool);

    /**
     * @see ConfigurationManager::SetFloat()
     */

    EXPOSE_API(configuration, SetFloat);

    /**
     * @see ConfigurationManager::SetInt()
     */

    EXPOSE_API(configuration, SetInt);

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

    EXPOSE_API(configuration, Load);
}

/// @}

#endif