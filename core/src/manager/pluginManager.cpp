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

#include "manager/controllerManager.h"
#include "manager/pluginManager.h"

#include "plugin/plugin.h"

#include "common/file.h"

#include "api/console.h"

#include "preproc/os.h"

#include "config.h"

#define BOOST_DLL_FORCE_ALIAS_INSTANTIATION
#include <boost/dll/runtime_symbol_info.hpp>
#include <boost/dll/shared_library.hpp>

void PluginManager::OnPreInit()
{
    LoadPlugins( FindPlugins() );
}

void PluginManager::OnPostRelease()
{
    for ( auto plugin : mPlugins )
    {
        delete plugin.second;
    }

    mPlugins.clear();
}

void PluginManager::SetBlackList( const std::set< std::string > &blacklist )
{
    mBlacklist = blacklist;
}

void PluginManager::LoadPlugins( const std::vector< std::string > &plugins )
{
    for ( auto plugin : plugins )
    {
        try
        {
            boost::dll::shared_library *lib = new boost::dll::shared_library( plugin, boost::dll::load_mode::append_decorations );

            std::string pluginName = GetName( plugin );

            if ( !LoadPlugin( lib, pluginName, plugin ) )
            {
                lib->unload();
                delete lib;
            }
        }
        catch ( std::exception &e )
        {
            Console::Warningp( LOG( "Failed to load plugin on path '{}'!" ), plugin );
        }
    }
}

bool PluginManager::LoadPlugin( boost::dll::shared_library *lib, std::string pluginName,
                                std::string plugin )
{
    const std::string func = GetLoadFunction( pluginName );

    if ( lib->has( func ) && !IsBlacklisted( pluginName ) && !IsLoaded( pluginName ) )
    {
        PluginInfo info = lib->get< PluginInfo( SystemManager * )>( func )( mManagerHolder->system );
        std::string name = info.manager->GetName();

        StorePlugin( info, plugin, pluginName );

        return true;
    }

    return false;
}

void PluginManager::StorePlugin( const PluginInfo &info, std::string plugin, std::string pluginName )
{
    Console::Initf( "Initialising library '%s' on path '%s'.", info.manager->GetName(), plugin );

    mManagerHolder->controller->AddExt( info.type, info.manager );

    mAreLoaded.insert( pluginName );
}

bool PluginManager::IsBlacklisted( const std::string &name ) const
{
    return mBlacklist.find( name ) != mBlacklist.end();
}

bool PluginManager::IsLoaded( const std::string &name ) const
{
    return mAreLoaded.find( name ) != mAreLoaded.end();
}

std::string PluginManager::GetName( const std::string &plugin )
{
    std::string fileName = Path::GetFileName( plugin, true );
#if !(OS_IS_WINDOWS)
    fileName = String::Replace( fileName, "lib", "" );
#endif
    return fileName;
}

std::string PluginManager::GetLoadFunction( const std::string &fileName )
{
    return String::Place( "Load{}PluginX{}", fileName, std::string( ARCH_IS_X86_32 ? "32" : "64" ) ) ;
}

std::vector< std::string > PluginManager::FindPlugins()
{
    std::vector< std::string > plugins;

    for ( auto file : File::List( Path::GetExeDirectory() + PROGRAM_PLUGIN_DIRECTORY ) )
    {
        if ( File::IsSharedLibrary( file ) )
        {
            plugins.push_back( file );
        }
    }

    return plugins;
}
