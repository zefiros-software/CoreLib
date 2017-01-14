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

#include "engineTest.h"

namespace
{

    TEST( Macro, CONCAT )
    {
        //! [CONCAT]
#define PART1_PART2 10
        EXPECT_EQ( PART1_PART2, CONCAT( PART1_, PART2 ) );
        //! [CONCAT]
#undef PART1_PART2
    }

    TEST( Macro, VA_SIZE )
    {
        //! [VA_SIZE]
        EXPECT_EQ( 1, VA_SIZE() );
        EXPECT_EQ( 1, VA_SIZE( 1 ) );
        EXPECT_EQ( 2, VA_SIZE( 1, 2 ) );
        EXPECT_EQ( 3, VA_SIZE( 1, 2, 3 ) );
        EXPECT_EQ( 4, VA_SIZE( 1, 2, 3, 4 ) );
        EXPECT_EQ( 5, VA_SIZE( 1, 2, 3, 4, 5 ) );
        EXPECT_EQ( 6, VA_SIZE( 1, 2, 3, 4, 5, 6 ) );
        //! [VA_SIZE]
    }

    TEST( Macro, VA_SELECT )
    {
        //! [VA_SELECT]
#define SELECT( ... )   VA_SELECT( SELECT_, __VA_ARGS__ )
#define SELECT_1( a )   1
#define SELECT_2( a, b )   2
#define SELECT_3( a, b, c )   3
#define SELECT_4( a, b, c, d )   4
#define SELECT_5( a, b, c, d, e )   5
#define SELECT_6( a, b, c, d, e, f )   6

        //EXPECT_EQ( 1, SELECT() );
        EXPECT_EQ( 1, SELECT( 1 ) );
        EXPECT_EQ( 2, SELECT( 1, 2 ) );
        EXPECT_EQ( 3, SELECT( 1, 2, 3 ) );
        EXPECT_EQ( 4, SELECT( 1, 2, 3, 4 ) );
        EXPECT_EQ( 5, SELECT( 1, 2, 3, 4, 5 ) );
        EXPECT_EQ( 6, SELECT( 1, 2, 3, 4, 5, 6 ) );
        //! [VA_SELECT]
    }

    TEST( Macro, IS_SET, Simple )
    {
        //! [IS_SET Simple]
        EXPECT_TRUE( IS_SET( 0x01, 0x01 ) );
        //! [IS_SET Simple]
    }

    TEST( Macro, IS_SET, Multiple )
    {
        //! [IS_SET Multiple]
        EXPECT_TRUE( IS_SET( 0x01 | 0x02, 0x01 ) );
        //! [IS_SET Multiple]
    }

    TEST( Macro, IS_SET, HasMultipleSet )
    {
        //! [IS_SET HasMultipleSet]
        EXPECT_TRUE( IS_SET( 0x01 | 0x02, 0x01 | 0x02 ) );
        //! [IS_SET HasMultipleSet]
    }

    TEST( Macro, IS_SET, HasMultipleSetNot )
    {
        //! [IS_SET HasMultipleSetNot]
        EXPECT_FALSE( IS_SET( 0x01 | 0x02, 0x01 | 0x02 | 0x04 ) );
        //! [IS_SET HasMultipleSetNot]
    }

    TEST( Macro, IS_SET, Null )
    {
        //! [IS_SET Null]
        EXPECT_FALSE( IS_SET( 0x00, 0x01 ) );
        //! [IS_SET Null]
    }

    TEST( Macro, IS_SET, NullSet )
    {
        //! [IS_SET NullSet]
        EXPECT_TRUE( IS_SET( 0x01, 0x00 ) );
        //! [IS_SET NullSet]
    }

    TEST( Macro, IS_SET, NotSet )
    {
        //! [IS_SET NotSet]
        EXPECT_FALSE( IS_SET( 0x01, 0x02 ) );
        //! [IS_SET NotSet]
    }

    TEST( Macro, HAS_ANY_SET, Simple )
    {
        //! [HAS_ANY_SET Simple]
        EXPECT_TRUE( HAS_ANY_SET( 0x01, 0x01 ) );
        //! [HAS_ANY_SET Simple]
    }

    TEST( Macro, HAS_ANY_SET, Multiple )
    {
        //! [HAS_ANY_SET Multiple]
        EXPECT_TRUE( HAS_ANY_SET( 0x01 | 0x02, 0x01 ) );
        //! [HAS_ANY_SET Multiple]
    }

    TEST( Macro, HAS_ANY_SET, HasMultipleSet )
    {
        //! [HAS_ANY_SET HasMultipleSet]
        EXPECT_TRUE( HAS_ANY_SET( 0x01 | 0x02, 0x01 | 0x02 ) );
        //! [HAS_ANY_SET HasMultipleSet]
    }

    TEST( Macro, HAS_ANY_SET, HasOneOfSet )
    {
        //! [HAS_ANY_SET HasOneOfSet]
        EXPECT_TRUE( HAS_ANY_SET( 0x01 | 0x02, 0x01 | 0x02 | 0x04 ) );
        //! [HAS_ANY_SET HasOneOfSet]
    }

    TEST( Macro, HAS_ANY_SET, Null )
    {
        //! [HAS_ANY_SET Null]
        EXPECT_FALSE( HAS_ANY_SET( 0x00, 0x01 ) );
        //! [HAS_ANY_SET Null]
    }

    TEST( Macro, HAS_ANY_SET, NullSet )
    {
        //! [HAS_ANY_SET NullSet]
        EXPECT_FALSE( HAS_ANY_SET( 0x01, 0x00 ) );
        //! [HAS_ANY_SET NullSet]
    }

    TEST( Macro, HAS_ANY_SET, NotSet )
    {
        //! [HAS_ANY_SET NotSet]
        EXPECT_FALSE( HAS_ANY_SET( 0x01, 0x02 ) );
        //! [HAS_ANY_SET NotSet]
    }

    //! [SAFE_RELEASE Example class]

    class SafeRelease
    {
    public:

        SafeRelease( bool *var )
            : mBool( var )
        {
        }

        void Release() const
        {
            *mBool = false;
            delete this;
        }

    private:

        bool *mBool;
    };

    //! [SAFE_RELEASE Example class]

    TEST( Macro, SAFE_RELEASE, ReleaseTest )
    {
        //! [SAFE_RELEASE]
        bool var = true;
        EXPECT_TRUE( var );

        SafeRelease *obj = new SafeRelease( &var );

        SAFE_RELEASE( obj );

        EXPECT_FALSE( var );
        EXPECT_EQ( NULL, obj );
        //! [SAFE_RELEASE]
    }

    TEST( Macro, SAFE_RELEASE, Safety )
    {
        //! [SAFE_RELEASE nullptr]
        SafeRelease *ptr = nullptr;
        SAFE_RELEASE( ptr );
        EXPECT_EQ( nullptr, ptr );
        //! [SAFE_RELEASE nullptr]
    }

    TEST( Macro, SIZE_OF_KB )
    {
        //! [SIZE_OF_KB]
        EXPECT_EQ( 1024, SIZE_OF_KB( 1 ) );
        //! [SIZE_OF_KB]
    }

    TEST( Macro, SIZE_OF_KB, Multiple )
    {
        EXPECT_EQ( 2048, SIZE_OF_KB( 2 ) );
    }

    TEST( Macro, SIZE_OF_MB )
    {
        //! [SIZE_OF_MB]
        EXPECT_EQ( 1048576, SIZE_OF_MB( 1 ) );
        //! [SIZE_OF_MB]
    }

    TEST( Macro, SIZE_OF_MB, Multiple )
    {
        EXPECT_EQ( 2097152, SIZE_OF_MB( 2 ) );
    }

    //! [SAFE_RELEASEDELETE Example class]
    class SafeReleaseDelete
    {
    public:

        SafeReleaseDelete( bool *var1, bool *var2 )
            : mBool1( var1 ),
              mBool2( var2 )
        {
        }

        ~SafeReleaseDelete()
        {
            *mBool2 = false;
        }

        void Release() const
        {
            *mBool1 = false;
        }

    private:

        bool *mBool1;
        bool *mBool2;
    };
    //! [SAFE_RELEASEDELETE Example class]

    TEST( Macro, SAFE_RELEASEDELETE, ReleaseDeleteTest )
    {
        //! [SAFE_RELEASEDELETE]
        bool var1 = true;
        bool var2 = true;
        EXPECT_TRUE( var1 );
        EXPECT_TRUE( var2 );

        SafeReleaseDelete *obj = new SafeReleaseDelete( &var1, &var2 );

        SAFE_RELEASE_DELETE( obj );

        EXPECT_FALSE( var1 );
        EXPECT_FALSE( var2 );
        EXPECT_EQ( NULL, obj );
        //! [SAFE_RELEASEDELETE]
    }

    TEST( Macro, SAFE_RELEASEDELETE, Safety )
    {
        //! [SAFE_RELEASEDELETE nullptr]
        SafeReleaseDelete *ptr = nullptr;
        SAFE_RELEASE_DELETE( ptr );
        EXPECT_EQ( nullptr, ptr );
        //! [SAFE_RELEASEDELETE nullptr]
    }

    TEST( Macro, SAFE_DELETE, NoLeak )
    {
        //! [SAFE_DELETE]
        U32 *obj = new U32;
        SAFE_DELETE( obj );
        EXPECT_EQ( nullptr, obj );
        //! [SAFE_DELETE]
    }

    TEST( Macro, SAFE_DELETE, Safety )
    {
        //! [SAFE_DELETE nullptr]
        U32 *ptr = nullptr;
        SAFE_DELETE( ptr );
        EXPECT_EQ( nullptr, ptr );
        //! [SAFE_DELETE nullptr]
    }

    TEST( Macro, SAFE_ARRAY_DELETE, NoLeak )
    {
        //! [SAFE_ARRAY_DELETE]
        U32 *obj = new U32[32];
        SAFE_ARRAY_DELETE( obj );
        EXPECT_EQ( nullptr, obj );
        //! [SAFE_ARRAY_DELETE]
    }

    TEST( Macro, SAFE_ARRAY_DELETE, Safety )
    {
        //! [SAFE_ARRAY_DELETE nullptr]
        U32 *ptr = nullptr;
        SAFE_ARRAY_DELETE( ptr );
        EXPECT_EQ( nullptr, ptr );
        //! [SAFE_ARRAY_DELETE nullptr]
    }

    TEST( Util, BitExponent )
    {
        //! [BitExponent]
        const U64 one = 1;

        for ( size_t i = 0; i < 64; ++i )
        {
            EXPECT_EQ( i, Util::BitExponent( one << i ) );

            if ( i > 3 )
            {
                EXPECT_EQ( i, Util::BitExponent( static_cast<U64>( ( one << i ) | ( ( one << i ) - 1 ) ) ) );
            }
        }

        //! [BitExponent]
    }

    TEST( Util, NearestPower2 )
    {
        //! [NearestPower2]
        const U64 one = 1;

        for ( size_t i = 1; i < 64; ++i )
        {
            EXPECT_EQ( one << i, Util::NearestPower2( one << i ) );

            if ( i > 3 )
            {
                EXPECT_EQ( one << i, Util::NearestPower2( static_cast<U64>( ( one << i ) | ( ( one << i ) - 1 ) ) ) );
            }
        }

        //! [NearestPower2]
    }

    //! [IsChildParent Example]
    class Base
    {
    };

    class Child
        : public Base
    {
    };
    //! [IsChildParent Example]

    TEST( Util, IsChildParent )
    {
        //! [IsChildParent]
        EXPECT_TRUE( ( Util::IsChildParent< U32, U32>::value ) );
        EXPECT_TRUE( ( Util::IsChildParent< Child, Base >::value ) );
        EXPECT_TRUE( ( Util::IsChildParent< Child, Child>::value ) );
        EXPECT_FALSE( ( Util::IsChildParent< Base, Child >::value ) );
        EXPECT_FALSE( ( Util::IsChildParent< F64, U32 >::value ) );
        EXPECT_FALSE( ( Util::IsChildParent< F64, Child >::value ) );
        EXPECT_FALSE( ( Util::IsChildParent< F32, F64 >::value ) );
        EXPECT_FALSE( ( Util::IsChildParent< U32, U64 >::value ) );
        //! [IsChildParent]
    }

}