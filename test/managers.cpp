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

#include "manager/systemManager.h"
#include "manager/eventManager.h"
#include "manager/poolManager.h"

#include "managers.h"


void Managers::InitialisePoolManager()
{
    PoolManager *const pool = SystemManager::Get()->GetManagers()->pool;

    pool->SetManagers(SystemManager::Get()->GetManagers());

    pool->PreInitialise();
    pool->Initialise();
}

void Managers::ReleasePoolManager()
{
    PoolManager *const pool = SystemManager::Get()->GetManagers()->pool;

    pool->Release();
    pool->PostRelease();
}

void Managers::InitialiseFactoryManager()
{
    FactoryManager *const factory = SystemManager::Get()->GetManagers()->factory;

    factory->SetManagers(SystemManager::Get()->GetManagers());
    factory->PreInitialise();
    factory->Initialise();
}

void Managers::ReleaseFactoryManager()
{
    FactoryManager *const factory = SystemManager::Get()->GetManagers()->factory;

    factory->Release();
    factory->PostRelease();
}

void Managers::InitialiseEventManager()
{
    EventManager *const ev = SystemManager::Get()->GetManagers()->event;

    ev->SetManagers(SystemManager::Get()->GetManagers());
    ev->PreInitialise();
    ev->Initialise();
}

void Managers::ReleaseEventManager()
{
    EventManager *const ev = SystemManager::Get()->GetManagers()->event;

    ev->Release();
    ev->PostRelease();
}