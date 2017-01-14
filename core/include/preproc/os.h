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
#ifndef __ENGINE_OS_H__
#define __ENGINE_OS_H__

#include "external/predef.h"

#define OS_IS_WINDOWS BOOST_OS_WINDOWS

#define OS_IS_ANDROID BOOST_OS_ANDROID

#define OS_IS_BSD BOOST_OS_BSD
#define OS_IS_BSD_DRAGONFLY BOOST_OS_BSD_DRAGONFLY
#define OS_IS_BSD_FREE BOOST_OS_BSD_FREE
#define OS_IS_BSD_BSDI BOOST_OS_BSD_BSDI
#define OS_IS_BSD_NET BOOST_OS_BSD_NET
#define OS_IS_BSD_OPEN BOOST_OS_BSD_OPEN

#define OS_IS_CYGWIN BOOST_OS_CYGWIN
#define OS_IS_IOS BOOST_OS_IOS
#define OS_IS_LINUX BOOST_OS_LINUX
#define OS_IS_UNIX BOOST_OS_UNIX
#define OS_IS_MACOS BOOST_OS_MACOS

#endif