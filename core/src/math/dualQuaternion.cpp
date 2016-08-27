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

#include "math/dualQuaternion.h"

DualQuaternion::DualQuaternion()
    : mReal( 0.0, 0.0, 0.0, 1.0 ),
      mDual( 0.0, 0.0, 0.0, 0.0 )
{
}

DualQuaternion::DualQuaternion( const Vec3 &translation, const Quaternion &rotation, const Vec3 &scale )
{
    const F32 qx = rotation[0];
    const F32 qy = rotation[1];
    const F32 qz = rotation[2];
    const F32 qw = rotation[3];

    const F32 tx = translation.GetX();
    const F32 ty = translation.GetY();
    const F32 tz = translation.GetZ();

    mReal = rotation;

    mDual = Quaternion( 0.5f * ( tx * qw + ty * qz - tz * qy ),
                        0.5f * ( -tx * qz + ty * qw + tz * qx ),
                        0.5f * ( tx * qy - ty * qx + tz * qw ),
                        -0.5f * ( tx * qx + ty * qy + tz * qz ) );

}

void DualQuaternion::Decompose( Quaternion &q, Vec3 &translation ) const
{
    // regular quaternion (just copy the non-dual part):

    q = mReal;

    F32 x = mReal[0];
    F32 y = mReal[1];
    F32 z = mReal[2];
    F32 w = mReal[3];

    F32 dx = mDual[0];
    F32 dy = mDual[1];
    F32 dz = mDual[2];
    F32 dw = mDual[3];

    // translation vector:
    F32 doub = 2.0;
    translation[0] = doub * ( -dw * x + dx * w - dy * z + dz * y );
    translation[1] = doub * ( -dw * y + dx * z + dy * w - dz * x );
    translation[2] = doub * ( -dw * z - dx * y + dy * x + dz * w );
}

DualQuaternion::DualQuaternion( const Quaternion &real, const Quaternion &dual )
{
    mReal = real;
    mDual = dual;
}

DualQuaternion DualQuaternion::operator*( const DualQuaternion &other ) const
{
    return DualQuaternion( mReal * other.GetReal(), mReal * other.GetDual() + mDual * other.GetReal() );
}

DualQuaternion DualQuaternion::operator*( const F32 factor ) const
{
    return DualQuaternion( mReal * factor, mDual * factor );
}

void DualQuaternion::Normalize()
{
    const F32 length = mReal.Length();

    if ( length > 0 )
    {
        const Quaternion realN = mReal * ( 1.f / length );
        const Quaternion dualN = mDual * ( 1.f / length );

        mReal = realN;
        mDual = dualN - ( realN * realN.Dot( dualN ) );
    }
}