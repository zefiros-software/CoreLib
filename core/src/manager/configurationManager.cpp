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

#include "manager/config/programConfiguration.h"
#include "manager/configurationManager.h"

#include "common/directory.h"
#include "common/file.h"

#include "api/console.h"

#include "config.h"


ConfigurationManager::ConfigurationManager()
    : mConfigFile( Path::Get( "config/config.cnf", Path::Type::Data ) )
{
}

void ConfigurationManager::OnInit()
{
    const std::string path = GetConfigurationDirectory();

    if ( !Directory::Exists( path ) )
    {
        Directory::CreateAll( path );
    }

    Load<ProgramConfiguration>( mConfigFile );
}

void ConfigurationManager::OnRelease()
{
    for ( auto it = mConfigurationFileNameMap.begin(); it != mConfigurationFileNameMap.end(); ++it )
    {
        delete *mConfigurationFileObjectSet.find( it->second );
    }

    mConfigurationFileObjectSet.clear();
    mConfigurationFileNameMap.clear();
}


std::string ConfigurationManager::GetString( const std::string &key, const std::string &configName )
{
    std::lock_guard< std::recursive_mutex > lock( mMutex );

    auto it = mConfigurationFileNameMap.find( configName );

    if ( it != mConfigurationFileNameMap.end() )
    {
        ConfigurationFile *const file = it->second;

        if ( file->IsStringKey( key ) )
        {
            return file->GetString( key );
        }
        else
        {
            Console::Errorf( LOG( "Failed to find string key '%s' in Configuratation file '%s'." ),
                             key, configName );
        }
    }
    else
    {
        Console::Errorf( LOG( "Configuration file '%s' doesn't exist." ), configName );
    }

    // never happens
    return "";
}

bool ConfigurationManager::GetBool( const std::string &key, const std::string &configName )
{
    std::lock_guard< std::recursive_mutex > lock( mMutex );

    auto it = mConfigurationFileNameMap.find( configName );

    if ( it != mConfigurationFileNameMap.end() )
    {
        ConfigurationFile *const file = it->second;

        if ( file->IsBoolKey( key ) )
        {
            return file->GetBool( key );
        }
        else
        {
            Console::Errorf( LOG( "Failed to find bool key '%s' in Configuratation file '%s'." ),
                             key, configName );
        }
    }
    else
    {
        Console::Errorf( LOG( "Configuration file '%w' doesn't exist." ), configName );
    }

    return false;
}

F32 ConfigurationManager::GetFloat( const std::string &key, const std::string &configName )
{
    std::lock_guard< std::recursive_mutex > lock( mMutex );

    auto it = mConfigurationFileNameMap.find( configName );

    if ( it != mConfigurationFileNameMap.end() )
    {
        ConfigurationFile *const file = it->second;

        if ( file->IsFloatKey( key ) )
        {
            return file->GetFloat( key );
        }
        else
        {
            Console::Errorf( LOG( "Failed to find float key '%s' in Configuratation file '%s'." ),
                             key, configName );
        }
    }
    else
    {
        Console::Errorf( LOG( "Configuration file '%w' doesn't exist." ), configName );
    }

    return 0.0f;
}

S32 ConfigurationManager::GetInt( const std::string &key, const std::string &configName )
{
    std::lock_guard< std::recursive_mutex > lock( mMutex );

    auto it = mConfigurationFileNameMap.find( configName );

    if ( it != mConfigurationFileNameMap.end() )
    {
        ConfigurationFile *const file = it->second;

        if ( file->IsIntKey( key ) )
        {
            return file->GetInt( key );
        }
        else
        {
            Console::Errorf( LOG( "Failed to find int key '%s' in Configuratation file '%s'." ),
                             key, configName );
        }
    }
    else
    {
        Console::Errorf( LOG( "Configuration file '%w' doesn't exist." ), configName );

    }

    return 0;
}

bool ConfigurationManager::SetString( const std::string &key, const std::string &value, bool changeInFile,
                                      const std::string &configName )
{
    std::lock_guard< std::recursive_mutex > lock( mMutex );


    auto it = mConfigurationFileNameMap.find( configName );

    if ( it != mConfigurationFileNameMap.end() )
    {
        ConfigurationFile *const file = it->second;

        ConfigurationFile::ParseError code = file->SetString( key, value, changeInFile );

        if ( code == ConfigurationFile::ParseError::None )
        {
            return true;
        }
        else
        {
            LogErrors( code, key, configName );
        }
    }
    else
    {
        Console::Warningf( LOG( "Configuration file '%w' doesn't exist." ), configName );
    }

    return false;
}

bool ConfigurationManager::SetBool( const std::string &key, bool value, bool changeInFile,
                                    const std::string &configName )
{
    std::lock_guard< std::recursive_mutex > lock( mMutex );


    auto it = mConfigurationFileNameMap.find( configName );

    if ( it != mConfigurationFileNameMap.end() )
    {
        ConfigurationFile *const file = it->second;

        ConfigurationFile::ParseError code = file->SetBool( key, value, changeInFile );

        if ( code == ConfigurationFile::ParseError::None )
        {
            return true;
        }
        else
        {
            LogErrors( code, key, configName );
        }
    }
    else
    {
        Console::Warningf( LOG( "Configuration file '%w' doesn't exist." ), configName );
    }

    return false;
}

bool ConfigurationManager::SetFloat( const std::string &key, F32 value, bool changeInFile,
                                     const std::string &configName )
{
    std::lock_guard< std::recursive_mutex > lock( mMutex );

    auto it = mConfigurationFileNameMap.find( configName );

    if ( it != mConfigurationFileNameMap.end() )
    {
        ConfigurationFile *const file = it->second;

        ConfigurationFile::ParseError code = file->SetFloat( key, value, changeInFile );

        if ( code == ConfigurationFile::ParseError::None )
        {
            return true;
        }
        else
        {
            LogErrors( code, key, configName );
        }
    }
    else
    {
        Console::Warningf( LOG( "Configuration file '%w' doesn't exist." ), configName );
    }

    return false;
}

bool ConfigurationManager::SetInt( const std::string &key, S32 value, bool changeInFile,
                                   const std::string &configName )
{
    std::lock_guard< std::recursive_mutex > lock( mMutex );

    auto it = mConfigurationFileNameMap.find( configName );

    if ( it != mConfigurationFileNameMap.end() )
    {
        ConfigurationFile *const file = it->second;

        ConfigurationFile::ParseError code = file->SetInt( key, value, changeInFile );

        if ( code == ConfigurationFile::ParseError::None )
        {
            return true;
        }
        else
        {
            LogErrors( code, key, configName );
        }
    }
    else
    {
        Console::Warningf( LOG( "Configuration file '%w' doesn't exist." ), configName );
    }

    return false;
}

std::string ConfigurationManager::GetConfigurationDirectory() const
{
    return Path::GetDirectory( mConfigFile );
}

void ConfigurationManager::SetGlobalConfigFileName( const std::string &name )
{
    assert( !mInitialised );
    mConfigFile = name;
}

bool ConfigurationManager::LoadByObj( ConfigurationFile *const conf, const std::string &filePath,
                                      const std::string &configName /*= "" */ )
{
    std::lock_guard< std::recursive_mutex > lock( mMutex );

    if ( mConfigurationFileNameMap.find( configName ) == mConfigurationFileNameMap.end() )
    {
        if ( !File::Exists( filePath ) )
        {
            Console::Warningf( LOG( "Couldn't find the configuration file on path '%s', building a new one." ),
                               filePath );
        }

        mConfigurationFileNameMap[ configName ] = static_cast< ConfigurationFile * >( conf );

        if ( conf->Load( filePath, configName, &ConfigurationManager::LogErrors ) )
        {
            mConfigurationFileObjectSet.insert( conf );

            Console::Initf( "Loaded Configuration file '%s' on path '%s' successfully.", configName, filePath );
        }
        else
        {

            Console::Errorf( LOG( "Failed to load configuration file '%s' on path '%s'." ), configName,
                             filePath );
        }

        return false;
    }
    else
    {
        Console::Errorf( LOG( "Already loaded the configuration file with the name '%s'." ),
                         configName );
    }

    return true;
}

void ConfigurationManager::LogErrors( ConfigurationFile::ParseError error, const std::string &configName,
                                      const std::string &key /*= ""*/, std::size_t lineNumber /*= 0*/, const std::string &value /*= "" */ )
{
    switch ( error )
    {
    case ConfigurationFile::ParseError::NoSeperator:
        Console::Warningf( LOG( "No separator found, line '%z' in file '%s'." ), lineNumber,
                           configName );
        break;

    case ConfigurationFile::ParseError::NotRegistered:
        Console::Warningf( LOG( "The key '%s' isn't registered and won't be used, line '%z' in file '%s'." ),
                           key, lineNumber, configName );
        break;

    case ConfigurationFile::ParseError::IsDuplicateKey:
        Console::Warningf( LOG( "The key '%s' has a duplicate value, line '%z' in file '%s'." ),
                           key, lineNumber, configName );
        break;

    case ConfigurationFile::ParseError::NoKey:
        Console::Warningf( LOG( "The value '%s' doesn't have an associated key, line '%z' in file '%s'." ),
                           value, lineNumber, configName );
        break;

    case ConfigurationFile::ParseError::NoValue:
        Console::Warningf( LOG( "There is an assignment without value, line '%z' in file '%s'." ),
                           lineNumber, configName );
        break;

    case ConfigurationFile::ParseError::InvalidString:
        Console::Errorf( LOG( "The value '%s' on key '%s' is an invalid string, line '%z' in file '%s'." ),
                         value, key, lineNumber, configName );
        break;

    case ConfigurationFile::ParseError::InvalidBool:
        Console::Errorf( LOG( "The value '%s' on key '%s' is an invalid bool, line '%z' in file '%s'." ),
                         value, key, lineNumber, configName );
        break;

    case ConfigurationFile::ParseError::InvalidFloat:
        Console::Errorf( LOG( "The value '%s' on key '%s' is an invalid float, line '%z' in file '%s'." ),
                         value, key, lineNumber, configName );
        break;

    case ConfigurationFile::ParseError::InvalidInt:
        Console::Errorf( LOG( "The value '%s' on key '%s' is an invalid int, line '%z' in file '%s'." ),
                         value, key, lineNumber, configName );
        break;

    case ConfigurationFile::ParseError::InvalidKey:
        Console::Errorf( LOG( "Failed to find key '%s' in configuration file '%s'." ), key,
                         configName );
        break;

    case ConfigurationFile::ParseError::FixingFailed:
        Console::Errorf( LOG( "Failed to fix configuration file '%s'." ), configName );
        break;

    case ConfigurationFile::ParseError::FailedToOpenFile:
        Console::Errorf( LOG( "Failed to open configuration file '%s'." ), configName );
        break;

    case ConfigurationFile::ParseError::None:
    default:
        break;
    }
}