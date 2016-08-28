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

#include "manager/factoryManager.h"

#include "api/expose.h"

#pragma once
#ifndef __FACTORY_H__
#define __FACTORY_H__

namespace Factory
{
    EXPOSE_API( factory, AddFactory );
    EXPOSE_API_NOARG( factory, AddFactory );

    EXPOSE_API( factory, ReleaseFactory );

    EXPOSE_API( factory, ReleaseFactories );

    EXPOSE_API( factory, HasFactory );
    EXPOSE_API_NOARG( factory, HasFactory );

    EXPOSE_API( factory, GetFactory );
    EXPOSE_API_NOARG( factory, GetFactory );

    EXPOSE_API( factory, CreateInstance );
    EXPOSE_API_NOARG( factory, CreateInstance );
}

#endif