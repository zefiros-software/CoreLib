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
#ifndef __ENGINE_SYSTEMMANAGER_H__
#define __ENGINE_SYSTEMMANAGER_H__

#include "manager/abstract/abstractManager.h"

#include "common/util.h"

/// @addtogroup Managers
/// @{

/**
 * This manager manages all managers without managing itself.
 */

class SystemManager
{
public:

    SystemManager( S32 argc, char **argv );

    virtual ~SystemManager();

    /// @name Control Methods
    /// @{

    /**
     * Initialises all managers it holds.
     */

    virtual void Initialise();

    /**
     * Releases this managers and all managers it manages.
     */

    virtual void Release();

    virtual void Release( Namespace ns );

    /**
     * Updates all managers.
     */

    void Update();

    /// @}

    /// @name Managers
    /// @{

    /**
     * Gets the other managers.
     *
     * @return  The manager holder.
     */

    ManagerHolder *GetManagers();

    /**
     * Registers the managers.
     */

    virtual void RegisterManagers();

    /// @}

    /// @name Global Access
    /// @{

    /**
     * Gets the get system manager object. This is the only way to obtain the pointers to the other
     * managers. However, this should only be used when there is no other choice.
     *
     * @return  The system manager.
     */

    static SystemManager *Get( SystemManager *systemManager = nullptr );

    /// @}

    const S32 &GetArgc() const;

    S32 &GetArgc();

    char **GetArgv() const;

protected:


    // keeps a pointer to the other managers
    ManagerHolder mManagerHolder;

    // Holds a list with all managers
    std::vector< AbstractManager * > mCrititicalManagersList;

    std::vector< AbstractManager * > mManagersList;

    /**
     * Adds a manager to the system manager. This function uses the given type to
     * create the manager for itself.
     *
     * @tparam  typename T  Type should be a derivative of Manager.
     * @param [in,out]  managerStorage  The place to store the new manager in.
     * @param   name                    (Optional) the manager name.
     *
     * @return  The newly created manager.
     */

    template< typename tT >
    tT *AddManager( tT **managerStorage, const std::string &name, U32 flags = 0 )
    {
        tT *manager = new tT;
        *managerStorage = manager;

        InitialiseManager( manager, flags, name );

        if ( IS_SET( flags, ( U32 )AbstractManager::Type::Critical ) )
        {
            mCrititicalManagersList.push_back( manager );
        }
        else
        {
            mManagersList.push_back( manager );
        }


        return manager;
    }

    virtual void InitialiseManager( AbstractManager *manager, U32 flags, const std::string &name )
    {
        manager->SetFlag( flags );
        manager->SetName( name );
        manager->SetManagers( &mManagerHolder );
    }

    void InitialiseManagers();

    static void InitialiseManager( AbstractManager *const manager );

    void PreInitialiseManagers();

    void PostInitialiseManagers();

    void ReleaseManagers();

    void ReleaseManagers( Namespace ns );

    void PreReleaseManagers();

    void PostReleaseManagers();

    void DeleteManagers();

    void PreUpdateManagers();

    void UpdateManagers();

    void PostUpdateManagers();

    void SynchroniseManagers();

    void ProcessEventsManagers();

    S32 mArgc;
    char **mArgv;
};

/// @}

#endif