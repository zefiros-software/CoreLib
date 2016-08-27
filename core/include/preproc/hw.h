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
#ifndef __ENGINE_HW_H__
#define __ENGINE_HW_H__

#include "external/predef.h"

#define HW_HAS_SIMD BOOST_HW_SIMD_AVAILABLE
#define HW_HAS_SIMD_X86 BOOST_HW_SIMD_X86_AVAILABLE
#define HW_HAS_SIMD_X86_AMD BOOST_HW_SIMD_X86_AMD_AVAILABLE
#define HW_HAS_SIMD_ARM BOOST_HW_SIMD_ARM_AVAILABLE
#define HW_HAS_SIMD_PPC BOOST_HW_SIMD_PPC_AVAILABLE

#define HW_HAS_SIMD_X86_SSE (BOOST_HW_SIMD_X86 >= BOOST_HW_SIMD_X86_SSE_VERSION)
#define HW_HAS_SIMD_X86_SSE2 (BOOST_HW_SIMD_X86 >= BOOST_HW_SIMD_X86_SSE2_VERSION)
#define HW_HAS_SIMD_X86_SSE3 (BOOST_HW_SIMD_X86 >= BOOST_HW_SIMD_X86_SSE3_VERSION)
#define HW_HAS_SIMD_X86_SSE4_1 (BOOST_HW_SIMD_X86 >= BOOST_HW_SIMD_X86_SSE4_1_VERSION)
#define HW_HAS_SIMD_X86_SSE4_2 (BOOST_HW_SIMD_X86 >= BOOST_HW_SIMD_X86_SSE4_2_VERSION)
#define HW_HAS_SIMD_X86_AVX (BOOST_HW_SIMD_X86 >= BOOST_HW_SIMD_X86_AVX_VERSION)
#define HW_HAS_SIMD_X86_FMA3 (BOOST_HW_SIMD_X86 >= BOOST_HW_SIMD_X86_FMA3_VERSION)
#define HW_HAS_SIMD_X86_AVX2 (BOOST_HW_SIMD_X86 >= BOOST_HW_SIMD_X86_AVX2_VERSION )

#define HW_HAS_SIMD_X86_AMD_SSE4A (BOOST_HW_SIMD_X86_AMD >= BOOST_HW_SIMD_X86_AMD_SSE4A_VERSION)
#define HW_HAS_SIMD_X86_AMD_FMA4 (BOOST_HW_SIMD_X86_AMD >= BOOST_HW_SIMD_X86_AMD_FMA4_VERSION)
#define HW_HAS_SIMD_X86_AMD_XOP (BOOST_HW_SIMD_X86_AMD >= BOOST_HW_SIMD_X86_AMD_XOP_VERSION)

#define HW_HAS_SIMD_PPC_QPX (BOOST_HW_SIMD_PPC >= BOOST_HW_SIMD_PPC_QPX_VERSION  )
#define HW_HAS_SIMD_PPC_VMX (BOOST_HW_SIMD_PPC >= BOOST_HW_SIMD_PPC_VMX_VERSION )
#define HW_HAS_SIMD_PPC_VSX (BOOST_HW_SIMD_PPC >= BOOST_HW_SIMD_PPC_VSX_VERSION)

#endif