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
#ifndef __CONTROLLERMANAGER_H__
#define __CONTROLLERMANAGER_H__

#include "manager/abstract/abstractManager.h"

#include "container/namespaceNamedStorage.h"

#include "api/console.h"

#include <typeindex>

class ControllerManager
    : public AbstractManager
{
public:

    virtual void OnInit() override;
    virtual void OnPostInit() override;
    virtual void OnPreUpdate() override;
    virtual void OnUpdate() override;
    virtual void OnPostUpdate() override;
    virtual void OnRelease() override;
    virtual void OnPostRelease() override;

    virtual void OnRelease( Namespace ns ) override;

    template< typename tT >
    tT *Add( Namespace ns = 0U )
    {
        const std::type_index typeID = typeid( tT );
        tT *controller = new tT();

        AddExt( typeID, controller, ns );

        return controller;
    }

    void AddExt( std::type_index typeID, AbstractManager *mngr, Namespace ns = 0U );

    template< typename tT>
    tT *Get()
    {
        return static_cast< tT * >( mControllers.Get( typeid( tT ) ) );
    }

private:

    NamespaceNamedStorage< std::type_index, AbstractManager > mControllers;
    std::vector< AbstractManager * > mControllerCache;

};


#endif