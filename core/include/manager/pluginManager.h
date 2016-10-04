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
#ifndef __PLUGINMANAGER_H__
#define __PLUGINMANAGER_H__

#include "abstract/abstractManager.h"
#include "plugin/api.h"

#include <unordered_map>
#include <set>

struct PluginInfo;
class PluginBase;

namespace boost
{
    namespace dll
    {
        class shared_library;
    }
}

class PluginManager
    : public AbstractManager
{
public:

    void OnPreInit() override;

    void OnPostRelease() override;

    template< typename tT >
    tT *Add()
    {
        tT *p = new tT();
        const std::string name = GetName< tT >();
        p->SetName( name );
        StorePlugin( PluginInfo{ p, typeid( tT )}, "static", name );
        return p;
    }

    void SetBlackList( const std::set< std::string > &blacklist );

    template< typename tT >
    static std::string GetName( tT * = nullptr )
    {
        Console::Error( LOG( "This should never be called, make sure you have the correct includes!" ) );
        return "";
    }

private:

    std::unordered_map< std::string, PluginBase * > mPlugins;
    std::set< std::string > mBlacklist;
    std::set< std::string > mAreLoaded;

    void LoadPlugins( const std::vector< std::string > &plugins );

    bool LoadPlugin( boost::dll::shared_library *lib, std::string pluginName, std::string plugin );

    void StorePlugin( const PluginInfo &info, std::string plugin, std::string pluginName );

    bool IsBlacklisted( const std::string &name ) const;

    bool IsLoaded( const std::string &name ) const;

    std::string GetName( const std::string &plugin ) const;

    std::string GetLoadFunction( const std::string &fileName )const;

    static std::vector< std::string > FindPlugins();

};

#endif