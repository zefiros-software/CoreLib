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
#ifndef __POOL_H__
#define __POOL_H__

#include "manager/poolManager.h"
#include "api/expose.h"

namespace Pool
{
    EXPOSE_API( pool, Remove );
    EXPOSE_API_NOARG( pool, Remove );

    EXPOSE_API( pool, ClearAll );

    EXPOSE_API( pool, AddFromFactory );
    EXPOSE_API_NOARG( pool, AddFromFactory );

    EXPOSE_API( pool, Add );
    EXPOSE_API_NOARG( pool, Add );

    EXPOSE_API( pool, Get );
    EXPOSE_API_NOARG( pool, Get );

    EXPOSE_API( pool, HasPools );

    EXPOSE_API( pool, Has );
    EXPOSE_API_NOARG( pool, Has );
}

#endif