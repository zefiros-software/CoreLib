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

#include "common/namespace.h"

Namespace::Namespace() noexcept
    : mAddinNamespace( 0 ),
      mPluginNamespace( 0 )
{
}


Namespace::Namespace( const U16 pluginNamespace, const U16 addinNamespace ) noexcept
    : mAddinNamespace( addinNamespace ),
      mPluginNamespace( pluginNamespace )
{
}

Namespace::Namespace( const U32 fullNamespace ) noexcept
    : mAddinNamespace( fullNamespace >> 16 ),
      mPluginNamespace( ( ( fullNamespace << 16 ) >> 16 ) )
{
}

Namespace::Namespace( const Namespace pluginNamespace, const Namespace addinNamespace ) noexcept
    : mAddinNamespace( addinNamespace.GetAddinNamespace() ),
      mPluginNamespace( pluginNamespace.GetPluginNamespace() )
{
}

U16 Namespace::GetAddinNamespace() const noexcept
{
    return mAddinNamespace;
}

U16 Namespace::GetPluginNamespace() const noexcept
{
    return mPluginNamespace;
}

Namespace Namespace::GetAddin() const noexcept
{
    return Namespace( 0, mAddinNamespace );
}

Namespace Namespace::GetPlugin() const noexcept
{
    return Namespace( mPluginNamespace, 0 );
}

U32 Namespace::GetNamespace() const noexcept
{
    return ( mAddinNamespace << 16 ) | mPluginNamespace;
}

bool Namespace::IsAddin() const noexcept
{
    return mAddinNamespace != 0;
}

bool Namespace::operator!=( const Namespace &other ) const noexcept
{
    return mAddinNamespace != other.mAddinNamespace || mPluginNamespace != other.mPluginNamespace;
}

bool Namespace::operator==( const Namespace &other ) const noexcept
{
    return mAddinNamespace == other.mAddinNamespace && mPluginNamespace == other.mPluginNamespace;
}

U32 Namespace::operator()() const noexcept
{
    return GetNamespace();
}
