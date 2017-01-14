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

#pragma once
#ifndef __ENGINE_APPLICATION_H__
#define __ENGINE_APPLICATION_H__

#include "manager/applicationManager.h"

#include "api/expose.h"


/// @addtogroup Application
/// @{

namespace Application
{
    /// @name Query State
    /// @{

    /**
     * Query if the application is running, or if it is closing.
     *
     * @return  true if running, false if it is shutting down.
     *
     * @see ApplicationManager::IsRunning()
     */

    EXPOSE_API( application, IsRunning );

    /**
     * Query if the application is active.
     *
     * @return  true if active, false if not.
     *
     * @see ApplicationManager::IsActive()
     */

    EXPOSE_API( application, IsActive );

    /// @}

    /// @name Control State
    /// @{

    /**
     * Quits the application, by setting the Application::IsRunning() to false, which will stop the
     * mainloop from running.
     *
     * @see ApplicationManager::Quit()
     */

    EXPOSE_API( application, Quit );

    /// @}

    EXPOSE_API( application, IsDebug );

    EXPOSE_API( application, GetVersion );
}

/// @}

#endif