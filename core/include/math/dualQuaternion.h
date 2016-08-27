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
#ifndef __ENGINE_DUALQUATERNION_H__
#define __ENGINE_DUALQUATERNION_H__

#include "math/quaternion.h"

class Vec3;

class DualQuaternion
{
public:

    DualQuaternion();
    DualQuaternion( const Quaternion &real, const Quaternion &dual );
    DualQuaternion( const Vec3 &translation, const Quaternion &rotation, const Vec3 &scale );

    DualQuaternion operator*( const DualQuaternion &other ) const;
    DualQuaternion operator*( const F32 value ) const;

    const Quaternion &GetReal() const
    {
        return mReal;
    }
    const Quaternion &GetDual() const
    {
        return mDual;
    }

    void Decompose( Quaternion &q, Vec3 &translation ) const;

    void Normalize();

private:
    Quaternion mReal;
    Quaternion mDual;
};


#endif