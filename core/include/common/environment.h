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
#ifndef __ENGINE_ENVIRONMENT_H__
#define __ENGINE_ENVIRONMENT_H__

#include <string>

/// @addtogroup docCommon
/// @{

/// @addtogroup docCommon_Environment
/// @{

/**
 * A collection to get information on the environment
 * the game is running on.
 */

namespace Environment
{
    /// @name Environment Variables
    /// @{

    /**
     * Gets an environment variable. Returns "" if the variable does not exists.
     *
     * @param   var The environment variable name.
     *
     * @return The value.
     *
     * @details
     * @examples
     *
     * @snippet testEnvironment.cpp Combo
     */

    std::string Get( const std::string &var ) noexcept;

    /**
     * Sets an environment variable with the given value.
     *
     * @param   var   The environment variable name.
     * @param   value The value.
     *
     * @return True if it succeeds, false if it fails.
     *
     * @post
     *      * GetVariable( var ) == value
     *      * HasVariable( var ) == true
     *
     * @details
     * @examples
     *
     * @snippet testEnvironment.cpp Combo
     */

    bool Set( const std::string &var, const std::string &value ) noexcept;

    /**
     * Query whether the given variable name exists.
     *
     * @param   var The environment variable name.
     *
     * @return True if the variable exists, false if not.
     *
     * @details
     * @examples
     *
     * @snippet testEnvironment.cpp Combo
     */

    bool Has( const std::string &var ) noexcept;

    /**
     * Removes the environment variable.
     *
     * @param   var The environment variable name.
     *
     * @return True if it succeeds, false if it fails.
     *
     * @post
     *      * GetVariable( var ) == ""
     *      * HasVariable( var ) == false
     *
     * @details
     * @examples
     *
     * @snippet testEnvironment.cpp Combo
     */

    bool Remove( const std::string &var ) noexcept;

    /// @}
};

/// @}

/// @}

#endif
