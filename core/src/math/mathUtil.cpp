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

#include "math/mathUtil.h"
#include "math/mathf.h"

Matrix4 MathUtil::ProjectionMatPerspectiveLH( F32 fov, F32 hwRatio, F32 nearPlane, F32 farPlane )
{
    F32    sinFov = Mathf::Sin( 0.5f * fov );
    F32    cosFov = Mathf::Cos( 0.5f * fov );

    F32 height = cosFov / sinFov;
    F32 width  = height / hwRatio;
    F32 fRange = farPlane / ( farPlane - nearPlane );

    Vec4 r1( width, 0.0f, 0.0f, 0.0f );
    Vec4 r2( 0.0f, height, 0.0f, 0.0f );
    Vec4 r3( 0.0f, 0.0f, fRange, -fRange * nearPlane );
    Vec4 r4( 0.0f, 0.0f, 1.0f , 0.0f );

    Matrix4 newMat( r1, r2, r3, r4 );

    return newMat;
}

Matrix4 MathUtil::ProjectionMatOrthographicLH( F32 width, F32 height, F32 nearPlane, F32 farPlane )
{
    Vec4 rows[4];

    rows[0][0] = 2.0f / width;
    rows[1][0] = 0.0f;
    rows[2][0] = 0.0f;
    rows[3][0] = 0.0f;

    rows[0][1] = 0.0f;
    rows[1][1] = 2.0f / height;
    rows[2][1] = 0.0f;
    rows[3][1] = 0.0f;

    rows[0][2] = 0.0f;
    rows[1][2] = 0.0f;
    rows[2][2] = 1.0f / ( farPlane - nearPlane );
    rows[3][2] = 0.0f;

    rows[0][3] = 0.0f;
    rows[1][3] = 0.0f;
    rows[2][3] = -nearPlane / ( farPlane - nearPlane );
    rows[3][3] = 1.0f;

    return Matrix4( rows[0], rows[1], rows[2], rows[3] );
}


Matrix4 MathUtil::ViewMatrixLH( const Quaternion &rotation, const Vec3 &translation )
{
    Vec3 forward = Vec3( 0.0f, 0.0f, 1.0f ).Rotate( rotation.GetAxis(), rotation.GetAngle() );
    Vec3 up = Vec3( 0.0f, 1.0f, 0.0f ).Rotate( rotation.GetAxis(), rotation.GetAngle() );

    Vec3 right = up.Cross( forward );

    Vec4 rows[4];
    rows[0][0] = right[0];
    rows[1][0] = up[0];
    rows[2][0] = forward[0];
    rows[3][0] = 0.0f;

    rows[0][1] = right[1];
    rows[1][1] = up[1];
    rows[2][1] = forward[1];
    rows[3][1] = 0.0f;

    rows[0][2] = right[2];
    rows[1][2] = up[2];
    rows[2][2] = forward[2];
    rows[3][2] = 0.0f;

    rows[0][3] = -right.Dot( translation );
    rows[1][3] = -up.Dot( translation );
    rows[2][3] = -forward.Dot( translation );
    rows[3][3] = 1.0f;

    return Matrix4( rows[0], rows[1], rows[2], rows[3] );
}

Vec3 MathUtil::DualQuaternionTransform( const Vec3 &pt, const DualQuaternion &DQ )
{
    const Quaternion &qreal     = DQ.GetReal();
    const Vec3       qrealAxis( qreal[0], qreal[1], qreal[2] );
    const F32        qrealAngle = qreal[3];
    const Quaternion &qdual     = DQ.GetDual();
    const Vec3       qdualAxis( qdual[0], qdual[1], qdual[2] );
    const F32        qdualAngle = qdual[3];

    Vec3 finalPos = pt +
                    2 * qrealAxis.Cross( qrealAxis.Cross( pt )  + qrealAngle * pt ) +
                    2 * ( qrealAngle *  qdualAxis - qdualAngle * qrealAxis + qrealAxis.Cross( qdualAxis ) );

    return finalPos;
}
