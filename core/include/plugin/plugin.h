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
#ifndef __PLUGIN_H__
#define __PLUGIN_H__

#include "plugin/abstract/abstractPlugin.h"

#include "manager/systemManager.h"

#include <boost/dll/shared_library.hpp>
#include <boost/dll/alias.hpp>

#include <typeindex>

#define API extern "C" BOOST_SYMBOL_EXPORT

#define SET_PLUGIN( type ) \
API PluginInfo *Load ## type ## Plugin( SystemManager *sys ) { \
type *p = new type(); \
p->SetName( #type); \
SystemManager::Get(sys); \
return new PluginInfo{p, typeid(type)}; \
};

#define DEFINE_PLUGIN( type ) \
API PluginInfo *Load ## type ## Plugin( SystemManager *sys );

class PluginBase
    : public AbstractPlugin
{
public:
};

struct PluginInfo
{
    PluginBase *manager;
    std::type_index type;
};

namespace Plugin
{
    bool IsSharedLibrary( const std::string &p );
}

#endif