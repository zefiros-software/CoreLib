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
#ifndef __ENGINE_APPLICATIONMANAGER_H__
#define __ENGINE_APPLICATIONMANAGER_H__

#include "manager/abstract/abstractManager.h"

class WindowActivateEvent;

/// @addtogroup Application
/// @{

/**
 * Controls the application on the most basic level, for example
 * it allows the application to shutdown.
 *
 * @sa  Manager<ApplicationManager>
 */

class ApplicationManager
    : public AbstractManager
{
public:

    ApplicationManager();

    /// @name Events
    /// @{

    virtual void OnInit() override;

    /// @}


    /**
     * Query if the application is running or shutting down.
     *
     * @return  true if running, false if it is shutting down.
     */

    bool IsRunning() const;

    /**
     * Query if the application is active.
     *
     * @return  true if active, false if not.
     */

    bool IsActive() const;

    /**
     * Quits the application, by setting the Application::IsRunning() to false, which will stop the
     * mainloop from running.
     */

    void Quit();

    bool IsDebug();

    std::string GetVersion();

private:

    mutable std::mutex mMutex;

    // Holds whether the application is currently running, or shutting down
    bool mApplicationIsRunning;
    bool mApplicationIsActive;
};

/// @}

#endif

