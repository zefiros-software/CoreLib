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
#ifndef __ENGINE_ABSTRACTMANAGER_H__
#define __ENGINE_ABSTRACTMANAGER_H__

#include "manager/abstract/managerHolder.h"

#include "common/namespace.h"

#include "events/observer.h"

#include <string>

/// @addtogroup Managers
/// @{

/**
 * Implements the manager interface.
 */

class AbstractManager
{
public:

    enum class Type : U32
    {
        Critical = 0x01
    };

    AbstractManager();
    virtual ~AbstractManager();

    /// @name Initialisation
    /// @{

    /**
     * Initialises this manager.
     */

    void Initialise();

    void Release();

    void PreInitialise();

    void PostInitialise();

    void PreRelease();

    void PostRelease();

    void PreUpdate();

    void Update();

    void PostUpdate();

    void Synchronise();

    void ProcessEvents();

    // /@}

    /// @name Events
    /// @{

    /**
     * Executes the initialisation event.
     */

    virtual void OnInit();

    /**
     * Executes the release event.
     */

    virtual void OnRelease();

    virtual void OnRelease( Namespace ns );

    /**
     * Executes the initialisation event.
     */

    virtual void OnPreInit();

    virtual void OnPostInit();

    /**
     * Executes the release event.
     */

    virtual void OnPostRelease();

    virtual void OnPreRelease();

    virtual void OnPreUpdate();

    virtual void OnUpdate();

    virtual void OnPostUpdate();

    virtual void OnSynchronise();

    virtual void OnProcessEvents();

    /// @}

    /// @name Miscellaneous
    /// @{

    virtual void SetName( const std::string &name );

    /**
     * Gets the manager name. This may be equal to "".
     *
     * @return  The manager name.
     */

    const std::string &GetName();

    /// @}

    /// @name Managers
    /// @{

    /**
     * Sets the manager holder, so we can access other managers.
     *
     * @param [in,out]  managerHolder   If non-null, the manager holder.
     */

    void SetManagers( ManagerHolder *const managerHolder );

    /**
     * Gets the other managers.
     *
     * @return  The manager holder.
     */

    ManagerHolder *GetManagers() const;

    bool IsFlagSet( const U32 flag ) const;

    void UnsetFlag( const U32 flag );

    void SetFlag( const U32 flag );

    /// @}

    bool IsInitialised() const;

    template< class tC, class tN >
    void Observe( void ( tC::*method )( const tN & ) );

    template< class tC, class tN >
    void Unobserve( void ( tC::*method )( const tN & ) );

protected:

    // Holds whether this manager is already initialised so we can prevent it from being
    // initialised again.
    bool mInitialised;

    U32 mFlags;

    // Holds the managers name
    std::string mName;

    // Holds a pointer to the struct with
    // all the other managers
    ManagerHolder *mManagerHolder;
};

#include "manager/eventManager.h"

template <class tC, class tN>
void AbstractManager::Observe( void( tC::* method )( const tN & ) )
{
    GetManagers()->event->AddObserver< tN >( new Observer< tC, tN >( static_cast< tC *const >( this ), method ) );
}

template< class tC, class tN >
void AbstractManager::Unobserve( void ( tC::*method )( const tN & ) )
{
    Observer< tC, tN > observer( static_cast< tC *const >( this ), method );
    GetManagers()->event->RemoveObserver< tN >( &observer );
}

/// @}

#endif