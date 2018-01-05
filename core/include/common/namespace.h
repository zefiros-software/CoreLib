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
#ifndef __ENGINE_NAMESPACE_H__
#define __ENGINE_NAMESPACE_H__

#include "common/types.h"

#include <functional>

/// @addtogroup docCommon
/// @{

/// @addtogroup docCommon_Namespace
/// @{

/**
 * This class should be used to separate the plugin and addin namespaces. With namespaces we actually mean an 32 bit
 * integer split in two 16 bit integers. Where the least significant 16 bits stand for the plugin namespace, and the
 * most significant 16 bits stand for the addin namespace.
 *
 * When any of addin namespace bits is set, the namespace will point to a addin namespace. (This means 0 is not a
 * valid addin namespace, and thus we should start all namespaces on the integer 1). Please note that 0 is in fact a
 * valid plugin namespace. When none of those bits is set, the namespace will point to a plugin namespace.
 *
 * Using this mechanism, we can actually group addins, with the same addin namespace under different plugins.
 *
 * When core architecture groups entities (objects), based on their plugin/addin, we should favour using this class,
 * since it is just an integer set automatically, making it more performent and safer to use than the names of the
 * plugins/addins, which can in fact have collisions.
 */

class Namespace
{
public:

    /**
     * Default constructor, sets both the addin and plugin namespace to 0.
     */

    Namespace() noexcept;

    /**
     * Set both the plugin and addin namespace separately, using Namespace classes. The plugin namespace will be set
     * using only the plugin namespace part of the given plugin namespace, the same will hold for the addin namespace.
     *
     * @param   pluginNamespace The plugin namespace.
     * @param   addinNamespace  The addin namespace.
     */

    explicit Namespace(const Namespace pluginNamespace, const Namespace addinNamespace) noexcept;

    /**
     * Set both the plugin and addin namespace using a 32 bit integer, which then will be split automatically in two 16
     * bit integers, setting the plugin and addin namespace.
     *
     * @param   fullNamespace The full namespace.
     */

    Namespace(const U32 fullNamespace) noexcept;

    /**
     * Set both the plugin and addin namespace, using their own integers.
     *
     * @param   pluginNamespace The plugin namespace.
     * @param   addinNamespace  The addin namespace.
     */

    Namespace(const U16 pluginNamespace, const U16 addinNamespace) noexcept;

    /// @addtogroup Element access
    /// @{

    /**
     * Gets the addin namespace.
     *
     * @return The addin namespace.
     *
     * @details
     * @examples
     *
     * @snippet testNamespace.cpp SeperateNS
     */

    U16 GetAddinNamespace() const noexcept;

    /**
     * Gets the plugin namespace.
     *
     * @return The plugin namespace.
     *
     * @details
     * @examples
     *
     * @snippet testNamespace.cpp SeperateNS
     */

    U16 GetPluginNamespace() const noexcept;

    /**
     * Gets the addin namespace.
     *
     * @return The addin namespace.
     *
     * @details
     * @examples
     *
     * @snippet testNamespace.cpp SeperateNS
     */

    Namespace GetAddin() const noexcept;

    /**
     * Gets the plugin namespace.
     *
     * @return The plugin namespace.
     *
     * @details
     * @examples
     *
     * @snippet testNamespace.cpp SeperateNS
     */

    Namespace GetPlugin() const noexcept;

    /**
     * Gets the full namespace.
     *
     * @return The namespace.
     *
     * @details
     * @examples
     *
     * @snippet testNamespace.cpp GetNamespace
     */

    U32 GetNamespace() const noexcept;

    /// @}

    /// @addtogroup State verification
    /// @{

    /**
     * Query if this object holds an addin namespace. The addin namespace part should differ from 0, for this to be true.
     *
     * @return True if addin, false if not.
     *
     * @details
     * @examples
     *
     * @snippet testNamespace.cpp IsAddin
     */

    bool IsAddin() const noexcept;

    /// @}

    /**
     * Equality operator.
     *
     * @param   other The other.
     *
     * @return True if the both the addin and plugin namespace are equal.
     */

    bool operator==(const Namespace &other) const noexcept;

    /**
     * Inequality operator.
     *
     * @param   other The other.
     *
     * @return True if the either the addin or plugin namespace are not equal.
     */

    bool operator!=(const Namespace &other) const noexcept;

    /**
     * Function call operator.
     *
     * @return The hash code.
     */

    U32 operator()() const noexcept;

private:

    // Holds our addin namespace
    U16 mAddinNamespace;

    // Holds our plugin namespace
    U16 mPluginNamespace;
};

namespace std
{
    template <>
    struct hash<Namespace>
    {
        std::size_t operator()(const Namespace &k) const noexcept
        {
            return static_cast< size_t >(k.GetNamespace());
        }
    };
}

/// @}

/// @}

#endif