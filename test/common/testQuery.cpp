//
// Copyright (C) <year> <copyright holders>
//
// Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shAll be included in All copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//

#include "common/query.h"

#include "engineTest.h"
#include <array>

namespace
{

    template<typename tR, typename tT, S32 N, typename tF>
    void CheckRangeEqArray( tR dst, tT( &ans )[N], tF f )
    {
        for ( U32 i = 0; i < N; i++ )
        {
            EXPECT_EQ( f( ans[i] ), f( dst.Next() ) );
        }

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    template<typename tR, typename tT, S32 N>
    void CheckRangeEqArray( tR dst, tT( &ans )[N] )
    {
        for ( S32 i = 0; i < N; i++ )
        {
            EXPECT_EQ( ans[i], dst.Next() );
        }

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    template<typename tT, S32 N>
    std::set<tT> ArrayToSet( tT( &ans )[N] )
    {
        std::set<tT> res;

        for ( S32 i = 0; i < N; i++ )
        {
            res.insert( ans[i] );
        }

        EXPECT_EQ( N, static_cast<S32>( res.size() ) );

        return res;
    }

    template<typename tR, typename tT, S32 N>
    void CheckRangeEqSet( tR dst, tT( &ans )[N] )
    {
        std::set<tT> expected = ArrayToSet( ans );
        std::set<typename tR::value_type> actual = dst.ToSet();

        EXPECT_EQ( expected.size(), actual.size() );

        auto it1 = expected.begin();
        auto it2 = actual.begin();

        for ( ; it1 != expected.end() && it2 != actual.end(); ++it1, ++it2 )
        {
            EXPECT_EQ( *it2, *it1 );
        }
    }

    TEST( Query, All, ThreeInts )
    {
        std::vector<S32> src = {1, 2, 3};

        auto rng = Query( src );

        EXPECT_TRUE( rng.All() );

        EXPECT_TRUE( rng.All( []( S32 a )
        {
            return a > 0;
        } ) );
        EXPECT_TRUE( rng.All( []( S32 a )
        {
            return a < 4;
        } ) );
        EXPECT_TRUE( rng.All( []( S32 a )
        {
            return a > 0 && a < 4;
        } ) );

        EXPECT_FALSE( rng.All( []( S32 a )
        {
            return a > 2;
        } ) );
        EXPECT_FALSE( rng.All( []( S32 a )
        {
            return a == 1;
        } ) );
        EXPECT_FALSE( rng.All( []( S32 a )
        {
            return a < 3;
        } ) );
    }

    TEST( Query, All, ThreeIntsSecond )
    {
        std::vector<S32> src = { 0, 1, 2 };

        auto rng = Query( src );

        EXPECT_FALSE( rng.All() );

        EXPECT_TRUE( rng.All( []( S32 a )
        {
            return a >= 0;
        } ) );
        EXPECT_TRUE( rng.All( []( S32 a )
        {
            return a <= 2;
        } ) );
        EXPECT_TRUE( rng.All( []( S32 a )
        {
            return a >= 0 && a <= 2;
        } ) );

        EXPECT_FALSE( rng.All( []( S32 a )
        {
            return a > 1;
        } ) );
        EXPECT_FALSE( rng.All( []( S32 a )
        {
            return a == 1;
        } ) );
        EXPECT_FALSE( rng.All( []( S32 a )
        {
            return a < 2;
        } ) );
    }

    TEST( Query, Any, ThreeInts )
    {
        std::vector<S32> src = { 1, 2, 3 };

        auto rng = Query( src );

        EXPECT_TRUE( rng.Any() );

        EXPECT_TRUE( rng.Any( []( S32 a )
        {
            return a == 1;
        } ) );
        EXPECT_TRUE( rng.Any( []( S32 a )
        {
            return a == 2;
        } ) );
        EXPECT_TRUE( rng.Any( []( S32 a )
        {
            return a == 3;
        } ) );
        EXPECT_TRUE( rng.Any( []( S32 a )
        {
            return a > 1;
        } ) );
        EXPECT_TRUE( rng.Any( []( S32 a )
        {
            return a < 3;
        } ) );
        EXPECT_TRUE( rng.Any( []( S32 a )
        {
            return a != 2;
        } ) );

        EXPECT_FALSE( rng.Any( []( S32 a )
        {
            return a == 0;
        } ) );
        EXPECT_FALSE( rng.Any( []( S32 a )
        {
            return a == 4;
        } ) );
        EXPECT_FALSE( rng.Any( []( S32 a )
        {
            return a < 1;
        } ) );
        EXPECT_FALSE( rng.Any( []( S32 a )
        {
            return a > 3;
        } ) );
    }

    TEST( Query, Avg, ThreeInts )
    {
        std::vector<S32> src = { 1, 2, 3 };

        auto rng = Query( src );

        EXPECT_NEAR( 2.0, rng.Avg<double>(), DBL_EPSILON );
    }

    TEST( Query, Avg, ThreeInts2 )
    {
        std::vector<S32> src = { 1, 2, 3 };

        auto rng = Query( src );

        EXPECT_EQ( 1, rng.Avg() );
    }

    TEST( Query, Avg, FiveInts )
    {
        std::vector<S32> src = { 1, 2, 3, 4, 5 };

        auto rng = Query( src );
        auto dst0 = rng.Where( []( S32 a )
        {
            return a % 2 == 0;
        } );
        auto dst1 = rng.Where( []( S32 a )
        {
            return a % 2 == 1;
        } );

        EXPECT_NEAR( 3.0, dst0.Avg<F64>(), DBL_EPSILON );
        EXPECT_NEAR( 3.0, dst1.Avg<F64>(), DBL_EPSILON );
    }

    TEST( Query, Avg, FiveStringsLen )
    {
        std::vector<std::string> src = {"hello", "apple", "nokia", "oracle", "ponny"};

        auto rng = Query( src );

        EXPECT_EQ( 5u, rng.Avg<size_t>( []( const std::string & str )
        {
            return str.size();
        } ) );
    }

    TEST( Query, ConcatRange, ArrayArray )
    {
        S32 src1[] = { 1, 2, 3, 4, 5 };
        S32 src2[] = { 6, 7, 8, 9 };

        S32 ans[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

        auto rng1 = Query( src1 );
        auto rng2 = Query( src2 );
        auto dst = rng1.Concat( rng2 );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, ConcatRange, ArrayVector )
    {
        S32 src1[] = { 1, 2, 3, 4, 5 };
        std::vector<S32> src2 = { 6, 7, 8, 9 };

        S32 ans[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9 };

        auto rng1 = Query( src1 );
        auto rng2 = Query( src2 );
        auto dst = rng1.Concat( rng2 );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, ConcatRange, ArrayVectorArray )
    {
        S32 src1[] = { 1, 2, 3, 4, 5 };
        std::vector<S32> src2 = {6, 7, 8, 9};
        S32 src3[] = { 10, 11, 12, 13, 14, 15 };

        S32 ans[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15 };

        auto rng1 = Query( src1 );
        auto rng2 = Query( src2 );
        auto rng3 = Query( src3 );
        auto dst = rng1.Concat( rng2 ).Concat( rng3 );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, Contains, ThreeInts )
    {
        std::vector<S32> src = { 1, 2, 3 };

        auto rng = Query( src );

        EXPECT_TRUE( rng.Contains( 1 ) );
        EXPECT_TRUE( rng.Contains( 2 ) );
        EXPECT_TRUE( rng.Contains( 3 ) );

        EXPECT_FALSE( rng.Contains( 0 ) );
        EXPECT_FALSE( rng.Contains( 4 ) );
    }

    TEST( Query, Contains, FiveStrings )
    {
        std::vector<std::string> src = { "hello", "apple", "nokia", "oracle", "ponny" };
        auto rng = Query( src );

        EXPECT_TRUE( rng.Contains( "hello" ) );
        EXPECT_TRUE( rng.Contains( "apple" ) );
        EXPECT_TRUE( rng.Contains( "nokia" ) );
        EXPECT_TRUE( rng.Contains( "oracle" ) );
        EXPECT_TRUE( rng.Contains( "ponny" ) );

        EXPECT_FALSE( rng.Contains( "dino" ) );
        EXPECT_FALSE( rng.Contains( "lord" ) );
    }

    TEST( Query, Count, ThreeIntsVector )
    {
        std::vector<S32> src = { 1, 2, 3 };

        auto rng = Query( src );

        EXPECT_EQ( 3u, rng.Count() );
    }

    TEST( Query, Count, ThreeIntsList )
    {
        std::vector<S32> src = { 1, 2, 3 };

        auto rng = Query( src );

        EXPECT_EQ( 3u, rng.Count() );
    }

    TEST( Query, Count, FiveInts )
    {
        std::vector<S32> src = { 1, 2, 3, 4, 5 };

        auto rng = Query( src );
        auto dst0 = rng.Where( []( S32 a )
        {
            return a % 2 == 0;
        } );
        auto dst1 = rng.Where( []( S32 a )
        {
            return a % 2 == 1;
        } );

        EXPECT_EQ( 2u, dst0.Count() );
        EXPECT_EQ( 3u, dst1.Count() );
    }

    TEST( Query, Count, OddCount )
    {
        std::vector<S32> src = {1, 0, 1, 0, 1};

        auto rng = Query( src );

        EXPECT_EQ( 2u, rng.Count( 0 ) );
        EXPECT_EQ( 3u, rng.Count( 1 ) );
    }

    TEST( Query, DistinctRange, Ints1to6 )
    {
        S32 src[] = { 4, 5, 3, 1, 4, 2, 1, 4, 6 };
        S32 ans[] = { 1, 2, 3, 4, 5, 6 };

        auto rng = Query( src );
        auto dst = rng.Distinct();

        CheckRangeEqSet( dst, ans );
    }

    TEST( Query, DistinctRange, IntMirrorFront )
    {
        S32 src[] = { 3, 2, 1, 0, 1, 2, 3 };
        S32 ans[] = { 0, 1, 2, 3 };

        auto rng = Query( src );
        auto dst = rng.Distinct();

        CheckRangeEqSet( dst, ans );
    }

    TEST( Query, DistinctRange, ManyEqualsFront )
    {
        S32 src[] = { 1, 1, 1, 1 };
        S32 ans[] = { 1 };

        auto rng = Query( src );
        auto dst = rng.Distinct();

        CheckRangeEqSet( dst, ans );
    }

    TEST( Query, DistinctRange, ManyEqualsWithOneFront )
    {
        S32 src[] = { 1, 1, 2, 1 };
        S32 ans[] = { 1, 2 };

        auto rng = Query( src );
        auto dst = rng.Distinct();

        CheckRangeEqSet( dst, ans );
    }

    TEST( Query, DistinctRange, OneFieldFront )
    {
        struct Man
        {
            std::string name;
            S32 age;

            bool operator < ( const Man &man ) const
            {
                return name < man.name || ( name == man.name && age < man.age );
            }

            bool operator == ( const Man &man ) const
            {
                return name == man.name;
            }

            bool operator == ( const std::string &manName ) const
            {
                return name == manName;
            }
        };

        Man src[] =
        {
            { "Anton", 1 },
            { "Taran", 2 },
            { "Poker", 3 },
            { "Agata", 4 },
            { "Mango", 2 },
            { "Banan", 1 },
        };

        std::string ans[] =
        {
            "Anton",
            "Taran",
            "Poker",
            "Agata",
        };

        auto rng = Query( src );
        auto dst = rng.Distinct( []( const Man & man )
        {
            return man.age;
        } );

        CheckRangeEqSet( dst, ans );
    }

    TEST( Query, ElementAt, ThreeInts )
    {
        std::vector<S32> src = { 1, 2, 3 };

        auto rng = Query( src );

        EXPECT_EQ( 1, rng.ElementAt( 0 ) );
        EXPECT_EQ( 2, rng.ElementAt( 1 ) );
        EXPECT_EQ( 3, rng.ElementAt( 2 ) );
    }

    TEST( Query, ElementAt, FiveStrings )
    {
        std::vector<std::string> src = { "hello", "apple", "nokia", "oracle", "ponny" };

        auto rng = Query( src );

        EXPECT_EQ( "hello", rng.ElementAt( 0 ) );
        EXPECT_EQ( "apple", rng.ElementAt( 1 ) );
        EXPECT_EQ( "nokia", rng.ElementAt( 2 ) );
        EXPECT_EQ( "oracle", rng.ElementAt( 3 ) );
        EXPECT_EQ( "ponny", rng.ElementAt( 4 ) );
    }

    TEST( Query, ForEach, ThreeCharsSum )
    {
        std::vector<S8> src = { 'a', 'b', 'c' };

        std::string str = "";
        Query( src ).Foreach( [&]( char a )
        {
            str += a;
        } );

        EXPECT_EQ( "abc", str );
    }

    TEST( Query, ForEach, ThreeCharsUpperSum )
    {
        std::vector<S8> src = { 'a', 'b', 'c' };

        std::string str = "";
        Query( src ).Foreach( [&]( char a )
        {
            str += a + ( 'A' - 'a' );
        } );

        EXPECT_EQ( "ABC", str );
    }

    TEST( Query, ForEach, ThreeIntsSum )
    {
        std::vector<S32> src = {10, 20, 30};

        S32 sum = 0;
        Query( src ).Foreach( [&]( S32 a )
        {
            sum += a;
        } );

        EXPECT_EQ( 60, sum );
    }

    TEST( Query, BytesRange, OneByteDefault )
    {
        U8 src[] = { 0xAA };
        S32 ans[] = { 0xAA };

        auto rng = Query( src );
        auto dst = rng.Bytes();

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, BytesRange, OneByteFL )
    {
        U8 src[] = { 0xAA };
        S32 ans[] = { 0xAA };

        auto rng = Query( src );
        auto dst = rng.Bytes( BytesDirection::FirstToLast );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, BytesRange, OneByteLF )
    {
        U8 src[] = { 0xAA };
        S32 ans[] = { 0xAA };

        auto rng = Query( src );
        auto dst = rng.Bytes( BytesDirection::LastToFirst );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, BytesRange, OneIntDefault )
    {
        S32 src[] = { 0x12345678 };
        S32 ans[] = { 0x78, 0x56, 0x34, 0x12 };

        auto rng = Query( src );
        auto dst = rng.Bytes();

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, BytesRange, OneIntFL )
    {
        S32 src[] = { 0x12345678 };
        S32 ans[] = { 0x78, 0x56, 0x34, 0x12 };

        auto rng = Query( src );
        auto dst = rng.Bytes( BytesDirection::FirstToLast );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, BytesRange, OneIntLF )
    {
        S32 src[] = { 0x12345678 };
        S32 ans[] = { 0x12, 0x34, 0x56, 0x78 };

        auto rng = Query( src );
        auto dst = rng.Bytes( BytesDirection::LastToFirst );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, BytesRange, IntsDefault )
    {
        U32 src[] = { 0x12345678, 0xAABBCCDD };
        S32 ans[] =
        {
            0x78, 0x56, 0x34, 0x12,
            0xDD, 0xCC, 0xBB, 0xAA,
        };

        auto rng = Query( src );
        auto dst = rng.Bytes( BytesDirection::FirstToLast );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, BytesRange, IntsFL )
    {
        U32 src[] = { 0x12345678, 0xAABBCCDD };
        S32 ans[] =
        {
            0x78, 0x56, 0x34, 0x12,
            0xDD, 0xCC, 0xBB, 0xAA,
        };

        auto rng = Query( src );
        auto dst = rng.Bytes( BytesDirection::FirstToLast );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, BytesRange, IntsLF )
    {
        U32 src[] = { 0x12345678, 0xAABBCCDD };
        S32 ans[] =
        {
            0x12, 0x34, 0x56, 0x78,
            0xAA, 0xBB, 0xCC, 0xDD,
        };

        auto rng = Query( src );
        auto dst = rng.Bytes( BytesDirection::LastToFirst );

        CheckRangeEqArray( dst, ans );
    }






    TEST( Query, BitsRange, OneByteDefault )
    {
        U8 src[] = { 0xAA };
        S32 ans[] = { 1, 0, 1, 0, 1, 0, 1, 0 };

        auto rng = Query( src );
        auto dst = rng.Bits();

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, BitsRange, OneByteHL )
    {
        U8 src[] = { 0xAA };
        S32 ans[] = { 1, 0, 1, 0, 1, 0, 1, 0 };

        auto rng = Query( src );
        auto dst = rng.Bits( BitsDirection::HighToLow );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, BitsRange, OneByteLH )
    {
        U8 src[] = { 0xAA };
        S32 ans[] = { 0, 1, 0, 1, 0, 1, 0, 1 };

        auto rng = Query( src );
        auto dst = rng.Bits( BitsDirection::LowToHigh );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, BitsRange, OneIntDefault )
    {
        U32 src[] = { 0x12345678 };
        S32 ans[] =
        {
            0, 1, 1, 1, 1, 0, 0, 0,
            0, 1, 0, 1, 0, 1, 1, 0,
            0, 0, 1, 1, 0, 1, 0, 0,
            0, 0, 0, 1, 0, 0, 1, 0,
        };

        auto rng = Query( src );
        auto dst = rng.Bits();

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, BitsRange, OneIntHL )
    {
        U32 src[] = { 0x12345678 };
        S32 ans[] =
        {
            0, 1, 1, 1, 1, 0, 0, 0,
            0, 1, 0, 1, 0, 1, 1, 0,
            0, 0, 1, 1, 0, 1, 0, 0,
            0, 0, 0, 1, 0, 0, 1, 0,
        };

        auto rng = Query( src );
        auto dst = rng.Bits( BitsDirection::HighToLow );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, BitsRange, OneIntLH )
    {
        U32 src[] = { 0x12345678 };
        S32 ans[] =
        {
            0, 0, 0, 1, 1, 1, 1, 0,
            0, 1, 1, 0, 1, 0, 1, 0,
            0, 0, 1, 0, 1, 1, 0, 0,
            0, 1, 0, 0, 1, 0, 0, 0,
        };

        auto rng = Query( src );
        auto dst = rng.Bits( BitsDirection::LowToHigh, BytesDirection::FirstToLast );
        auto vvv = dst.ToVector();

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, BitsRange, IntsDefault )
    {
        U32 src[] = { 0x12345678, 0xAABBCCDD };
        S32 ans[] =
        {
            0, 1, 1, 1, 1, 0, 0, 0, // 78
            0, 1, 0, 1, 0, 1, 1, 0, // 56
            0, 0, 1, 1, 0, 1, 0, 0, // 34
            0, 0, 0, 1, 0, 0, 1, 0, // 12

            1, 1, 0, 1, 1, 1, 0, 1, // DD
            1, 1, 0, 0, 1, 1, 0, 0, // CC
            1, 0, 1, 1, 1, 0, 1, 1, // BB
            1, 0, 1, 0, 1, 0, 1, 0, // AA
        };

        auto rng = Query( src );
        auto dst = rng.Bits();

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, BitsRange, IntsHL )
    {
        U32 src[] = { 0x12345678, 0xAABBCCDD };
        S32 ans[] =
        {
            0, 1, 1, 1, 1, 0, 0, 0, // 78
            0, 1, 0, 1, 0, 1, 1, 0, // 56
            0, 0, 1, 1, 0, 1, 0, 0, // 34
            0, 0, 0, 1, 0, 0, 1, 0, // 12

            1, 1, 0, 1, 1, 1, 0, 1, // DD
            1, 1, 0, 0, 1, 1, 0, 0, // CC
            1, 0, 1, 1, 1, 0, 1, 1, // BB
            1, 0, 1, 0, 1, 0, 1, 0, // AA
        };

        auto rng = Query( src );
        auto dst = rng.Bits( BitsDirection::HighToLow );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, BitsRange, IntsLH )
    {
        U32 src[] = { 0x12345678, 0xAABBCCDD };
        S32 ans[] =
        {
            0, 0, 0, 1, 1, 1, 1, 0, // -87
            0, 1, 1, 0, 1, 0, 1, 0, // -65
            0, 0, 1, 0, 1, 1, 0, 0, // -43
            0, 1, 0, 0, 1, 0, 0, 0, // -21

            1, 0, 1, 1, 1, 0, 1, 1, // -DD
            0, 0, 1, 1, 0, 0, 1, 1, // -CC
            1, 1, 0, 1, 1, 1, 0, 1, // -BB
            0, 1, 0, 1, 0, 1, 0, 1, // -AA
        };

        auto rng = Query( src );
        auto dst = rng.Bits( BitsDirection::LowToHigh );

        CheckRangeEqArray( dst, ans );
    }



    TEST( Query,  DotCall, BytesRange )
    {
        U32 src[] = { 0x12345678, 0xAABBCCDD };
        S32 ansFL[] =
        {
            0x78, 0x56, 0x34, 0x12,
            0xDD, 0xCC, 0xBB, 0xAA,
        };
        S32 ansLF[] =
        {
            0x12, 0x34, 0x56, 0x78,
            0xAA, 0xBB, 0xCC, 0xDD,
        };

        auto dstFL1 = Query( src ).Bytes();
        auto dstFL2 = Query( src ).Bytes( BytesDirection::FirstToLast );
        auto dstLF1 = Query( src ).Bytes( BytesDirection::LastToFirst );

        CheckRangeEqArray( dstFL1, ansFL );
        CheckRangeEqArray( dstFL2, ansFL );
        CheckRangeEqArray( dstLF1, ansLF );
    }

    TEST( Query,  DotCall, UnbytesRange )
    {
        U8 src[] =
        {
            0x78, 0x56, 0x34, 0x12,
            0xDD, 0xCC, 0xBB, 0xAA,
        };
        U32 ansFL[] = { 0x12345678, 0xAABBCCDD };
        U32 ansLF[] = { 0x78563412, 0xDDCCBBAA };

        auto dstFL1 = Query( src ).Unbytes<U32>();
        auto dstFL2 = Query( src ).Unbytes<U32>( BytesDirection::FirstToLast );
        auto dstLF1 = Query( src ).Unbytes<U32>( BytesDirection::LastToFirst );

        CheckRangeEqArray( dstFL1, ansFL );
        CheckRangeEqArray( dstFL2, ansFL );
        CheckRangeEqArray( dstLF1, ansLF );
    }

    TEST( Query,  DotCall, BitsRangeHL )
    {
        U32 src[] = { 0xAABBCCDD };
        S32 ansFL[] =
        {
            1, 1, 0, 1, 1, 1, 0, 1,
            1, 1, 0, 0, 1, 1, 0, 0,
            1, 0, 1, 1, 1, 0, 1, 1,
            1, 0, 1, 0, 1, 0, 1, 0,
        };
        S32 ansLF[] =
        {
            1, 0, 1, 0, 1, 0, 1, 0,
            1, 0, 1, 1, 1, 0, 1, 1,
            1, 1, 0, 0, 1, 1, 0, 0,
            1, 1, 0, 1, 1, 1, 0, 1,
        };

        auto dstFL1 = Query( src ).Bits();
        auto dstFL2 = Query( src ).Bits( BitsDirection::HighToLow );
        auto dstFL3 = Query( src ).Bits( BitsDirection::HighToLow, BytesDirection::FirstToLast );
        auto dstLF1 = Query( src ).Bits( BitsDirection::HighToLow, BytesDirection::LastToFirst );

        CheckRangeEqArray( dstFL1, ansFL );
        CheckRangeEqArray( dstFL2, ansFL );
        CheckRangeEqArray( dstFL3, ansFL );
        CheckRangeEqArray( dstLF1, ansLF );
    }

    TEST( Query,  DotCall, BitsRangeLH )
    {
        U32 src[] = { 0xAABBCCDD };
        S32 ansFL[] =
        {
            1, 0, 1, 1, 1, 0, 1, 1,
            0, 0, 1, 1, 0, 0, 1, 1,
            1, 1, 0, 1, 1, 1, 0, 1,
            0, 1, 0, 1, 0, 1, 0, 1,
        };
        S32 ansLF[] =
        {
            0, 1, 0, 1, 0, 1, 0, 1,
            1, 1, 0, 1, 1, 1, 0, 1,
            0, 0, 1, 1, 0, 0, 1, 1,
            1, 0, 1, 1, 1, 0, 1, 1,
        };

        auto dstFL1 = Query( src ).Bits( BitsDirection::LowToHigh );
        auto dstFL2 = Query( src ).Bits( BitsDirection::LowToHigh, BytesDirection::FirstToLast );
        auto dstLF1 = Query( src ).Bits( BitsDirection::LowToHigh, BytesDirection::LastToFirst );

        CheckRangeEqArray( dstFL1, ansFL );
        CheckRangeEqArray( dstFL2, ansFL );
        CheckRangeEqArray( dstLF1, ansLF );
    }

    TEST( Query, DotCall, UnbitsRangeHLFL )
    {
        S32 src[] =
        {
            1, 1, 0, 1, 1, 1, 0, 1,
            1, 1, 0, 0, 1, 1, 0, 0,
            1, 0, 1, 1, 1, 0, 1, 1,
            1, 0, 1, 0, 1, 0, 1, 0
        };
        S32 ans_4b[] = { 0xDD, 0xCC, 0xBB, 0xAA };
        U32 ans_1i[] = { 0xAABBCCDD };
        U32 ansLF_1i[] = { 0xDDCCBBAA };

        auto dst1_4b = Query( src ).Unbits();
        auto dst2_4b = Query( src ).Unbits( BitsDirection::HighToLow );
        auto dst1_1i = Query( src ).Unbits<U32>( BitsDirection::HighToLow );
        auto dst2_1i = Query( src ).Unbits<U32>( BitsDirection::HighToLow, BytesDirection::FirstToLast );
        auto dst3_1i = Query( src ).Unbits<U32>( BitsDirection::HighToLow, BytesDirection::LastToFirst );

        CheckRangeEqArray( dst1_4b, ans_4b );
        CheckRangeEqArray( dst2_4b, ans_4b );
        CheckRangeEqArray( dst1_1i, ans_1i );
        CheckRangeEqArray( dst2_1i, ans_1i );
        CheckRangeEqArray( dst3_1i, ansLF_1i );
    }





    TEST( Query, IterRange, ListInt )
    {
        std::list<S32> lst = {1, 2, 3, 4, 5};
        S32 ans[] = { 1, 2, 3, 4, 5 };

        CheckRangeEqArray( Query( lst ), ans );
        CheckRangeEqArray( Query<S32>( lst.begin(), lst.end() ), ans );
        CheckRangeEqArray( Query<S32>( lst.cbegin(), lst.cend() ), ans );
    }

    TEST( Query, IterRange, DequeInt )
    {
        std::deque<S32> dck = { 1, 2, 3, 4, 5 };
        S32 ans[] = { 1, 2, 3, 4, 5 };

        CheckRangeEqArray( Query( dck ), ans );
        CheckRangeEqArray( Query<S32>( dck.begin(), dck.end() ), ans );
        CheckRangeEqArray( Query<S32>( dck.cbegin(), dck.cend() ), ans );
    }

    TEST( Query, IterRange, VectorInt )
    {
        std::vector<S32> vec = { 1, 2, 3, 4, 5 };
        S32 ans[] = { 1, 2, 3, 4, 5 };

        CheckRangeEqArray( Query( vec ), ans );
        CheckRangeEqArray( Query<S32>( vec.begin(), vec.end() ), ans );
        CheckRangeEqArray( Query<S32>( vec.cbegin(), vec.cend() ), ans );
    }

    TEST( Query, IterRange, SetInt )
    {
        std::set<S32> set = { 1, 2, 3, 4, 5 };
        S32 ans[] = { 1, 2, 3, 4, 5 };

        CheckRangeEqSet( Query( set ), ans );
        CheckRangeEqSet( Query<S32>( set.begin(), set.end() ), ans );
        CheckRangeEqSet( Query<S32>( set.cbegin(), set.cend() ), ans );
    }

    TEST( Query, IterRange, MapInt )
    {
        std::map<S32, S32> map =
        {
            { 5, 1 },
            { 4, 2 },
            { 3, 3 },
            { 2, 4 },
            { 1, 5 }
        };
        std::pair<S32, S32> ans[] =
        {
            { 5, 1 },
            { 4, 2 },
            { 3, 3 },
            { 2, 4 },
            { 1, 5 }
        };

        CheckRangeEqArray( Query( map ).OrderBy( []( std::pair<S32, S32> p ) { return p.second;} ), ans );

        CheckRangeEqArray( Query<std::pair<S32, S32> >( map.begin(), map.end() )
        .OrderBy( []( std::pair<S32, S32> p ) { return p.second; } ), ans );

        CheckRangeEqArray( Query<std::pair<S32, S32> >( map.cbegin(), map.cend() )
        .OrderBy( []( std::pair<S32, S32> p ) { return p.second; } ), ans );
    }

    TEST( Query, IterRange, StdArrayInt )
    {
        std::array<S32, 5> arr = { { 1, 2, 3, 4, 5 } };
        S32 ans[] = { 1, 2, 3, 4, 5 };

        CheckRangeEqArray( Query( arr ), ans );
        CheckRangeEqArray( Query<S32>( arr.begin(), arr.end() ), ans );
        CheckRangeEqArray( Query<S32>( arr.cbegin(), arr.cend() ), ans );
    }

    TEST( Query, IterRange, ArrayInt )
    {
        S32 arr[] = { 1, 2, 3, 4, 5 };
        S32 *ptr = static_cast<S32 *>( arr );

        S32 ans[] = { 1, 2, 3, 4, 5 };

        CheckRangeEqArray( Query( arr ), ans );
        CheckRangeEqArray( Query<S32>( ptr, 5 ), ans );
        CheckRangeEqArray( Query<S32>( ptr, ptr + 5 ), ans );
    }

    TEST( Query, IterRange, OneElement )
    {
        S32 src[] = { 5 };
        S32 ans[] = { 5 };

        auto rng = Query( src );

        CheckRangeEqArray( rng, ans );
    }

    TEST( Query, IterRange, EmptyVector )
    {
        std::vector<S32> src;

        auto rng = Query( src );

        EXPECT_THROW( rng.Next(), EnumeratorEndException );
    }

    TEST( Query, Linq, WhereOdd )
    {
        std::vector<S32> src = {1, 2, 3, 4, 5, 6};

        auto rng = Query( src ).Where( []( S32 a ) { return a % 2 == 1; } );

        for ( size_t i = 1; i <= 5; i += 2 )
        {
            EXPECT_EQ( i, static_cast< size_t>( rng.Next() ) );
        }

        EXPECT_THROW( rng.Next(), EnumeratorEndException );
    }

    TEST( Query, Linq, WhereOdd_WhereLess )
    {
        std::vector<U32> src = { 1, 2, 3, 4, 5, 6, 7, 8 };

        auto rng = Query( src )
        .Where( []( U32 a ) { return a % 2 == 1;} )
        .Where( []( U32 a ) { return a < 4;} );

        for ( size_t i = 1; i <= 3; i += 2 )
        {
            EXPECT_EQ( i, rng.Next() );
        }

        EXPECT_THROW( rng.Next(), EnumeratorEndException );
    }

    TEST( Query, Linq, WhereLess_WhereOdd )
    {
        std::vector<S32> src = { 1, 2, 3, 4, 5, 6, 7, 8 };

        auto rng = Query( src )
        .Where( []( S32 a ) {return a < 4;} )
        .Where( []( S32 a ) {return a % 2 == 1;} )
        .ToVector();

        std::vector<S32> ans = { 1, 3 };

        EXPECT_EQ( ans, rng );
    }

    TEST( Query, Linq, WhereLess_WhereOdd_OrderByDesc )
    {
        std::vector<S32> src = { 1, 2, 3, 4, 5, 6, 7, 8 };

        auto rng = Query( src )
        .Where( []( S32 a ) { return a < 6; } )
        .Where( []( S32 a ) { return a % 2 == 1; } )
        .OrderBy( []( S32 a ) { return -a; } )
        .ToVector();

        std::vector<S32> ans = {5, 3, 1 };

        EXPECT_EQ( ans, rng );
    }

    TEST( Query, Linq, WhereOdd_ToVector )
    {
        std::vector<S32> src = { 1, 2, 3, 4, 5, 6, 7, 8 };

        auto dst = Query( src )
        .Where( []( S32 a ) {return a % 2 == 1;} )
        .ToVector();

        std::vector<S32> ans = {1, 3, 5, 7};

        EXPECT_EQ( ans, dst );
    }

    TEST( Query, Linq, WhereOdd_WhereLess_SelectMul2_ToVector )
    {
        std::vector<S32> src = { 1, 2, 3, 4, 5, 6, 7, 8 };

        auto dst = Query( src )
        .Where( []( S32 a ) { return a % 2 == 1; } )
        .Where( []( S32 a ) { return a < 6; } )
        .Select( []( S32 a ) { return a * 2; } )
        .ToVector();

        std::vector<S32> ans = {2, 6, 10};

        EXPECT_EQ( ans, dst );
    }

    TEST( Query, Linq, WhereOdd_WhereLess_SelectMul2_Reverse_ToVector )
    {
        std::vector<S32> src = { 1, 2, 3, 4, 5, 6, 7, 8 };

        auto dst = Query( src )
        .Where( []( S32 a ) { return a % 2 == 1; } )
        .Where( []( S32 a ) { return a < 6; } )
        .Select( []( S32 a ) { return a * 2; } )
        .Reverse()
        .ToVector();

        std::vector<S32> ans = {10, 6, 2};

        EXPECT_EQ( ans, dst );
    }

    TEST( Query, Linq, WhereOdd_Reverse_Reverse )
    {
        std::vector<S32> src = { 1, 2, 3, 4, 5, 6, 7, 8 };

        auto dst = Query( src )
        .Where( []( S32 a ) { return a % 2 == 1; } )
        .Reverse()
        .Where( []( S32 a ) { return a < 4;} )
        .Reverse()
        .ToVector();

        std::vector<S32> ans = {1, 3};

        EXPECT_EQ( ans, dst );
    }

    TEST( Query, Linq, Pointer_Front )
    {
        U32 src[] = { 1, 2, 3, 4, 5 };

        auto dst = Query<U32>( static_cast<U32 *>( src ), static_cast<U32 *>( src ) + 5 );

        for ( size_t i = 1; i <= 5; i++ )
        {
            EXPECT_EQ( i, dst.Next() );
        }

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    TEST( Query, Linq, Array_Front )
    {
        U32 src[] = { 1, 2, 3, 4, 5 };

        auto dst = Query( src );

        for ( size_t i = 1; i <= 5; i++ )
        {
            EXPECT_EQ( i, dst.Next() );
        }

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    TEST( Query, Linq, Creations )
    {
        std::vector<S32> vec = { 1, 2, 3, 4, 5 };
        S32 arr[] = { 1, 2, 3, 4, 5 };
        const S32 carr[] = { 1, 2, 3, 4, 5 };
        S32 *ptr = static_cast<S32 *>( arr );
        //const S32 *cptr = const_cast<S32 *>( arr );

        auto dst_vec = Query( vec );
        auto dst_arr = Query( arr );
        auto dst_carr = Query( carr );
        auto dst_ptr = Query<S32>( ptr, ptr + 5 );
        //auto dst_cptr = Query<const S32>( cptr, cptr + 5 );
        auto dst_ptr_length = Query<S32>( ptr, 5 );
        //auto dst_cptr_length = Query<const S32>( cptr, 5 );
        auto dst_vec_iter = Query<S32>( vec.begin(), vec.end() );
        //auto dst_vec_citer = Query< const S32 > ( vec.cbegin(), vec.cend() );
    }

    TEST( Query, Linq, MessagesCountUniqueContacts )
    {
        struct Message
        {
            std::string PhoneA;
            std::string PhoneB;
            std::string Text;

            bool operator < ( const Message &rhs ) const
            {
                return PhoneA < rhs.PhoneA ||
                       ( PhoneA == rhs.PhoneA && PhoneB < rhs.PhoneB ) ||
                       ( PhoneA == rhs.PhoneA && PhoneB == rhs.PhoneB && Text < rhs.Text );
            }
        };

        Message messages[] =
        {
            { "Anton", "Denis", "Hello, friend!" },
            { "Denis", "Write", "OLOLO" },
            { "Anton", "Papay", "WTF?" },
            { "Denis", "Maloy", "How r u?" },
            { "Denis", "Write", "Param-pareram!" },
        };

        size_t DenisUniqueContactCount = Query( messages )
        .Where( []( const Message & msg ) {return msg.PhoneA == "Denis"; } )
        .Distinct( []( const Message & msg ) {return msg.PhoneB; } )
        .Count();

        EXPECT_EQ( 2u, DenisUniqueContactCount );
    }

    TEST( Query, Linq, ForwardIterating )
    {
        std::stringstream stream( "0123456789" );
        auto dst = Query<char>( std::istream_iterator<char>( stream ),
                                std::istream_iterator<char>() )
        .Where( []( char a ) {return a % 2 == 0; } )
        .Select( []( char a ) {return std::string( 1, a ); } )
        .Sum();

        EXPECT_EQ( "02468", dst );
    }

    TEST( Query, Max, ThreeInts )
    {
        std::vector<S32> src = { 1, 2, 3 };
        auto rng = Query( src );

        EXPECT_EQ( 3, rng.Max() );
        EXPECT_EQ( 1, rng.Max( []( S32 a ) {return -a; } ) );
    }

    TEST( Query, Max, FiveStrings )
    {
        std::vector<std::string> src = { "hell", "apple", "zip" };

        auto rng = Query( src );

        EXPECT_EQ( "zip", rng.Max() );
        EXPECT_EQ( "apple", rng.Max( []( std::string s ) {return s.size(); } ) );
    }

    TEST( Query, Min, ThreeInts )
    {
        std::vector<S32> src = { 1, 2, 3 };
        auto rng = Query( src );

        EXPECT_EQ( 1, rng.Min() );
        EXPECT_EQ( 3, rng.Min( []( S32 a ) {return -a; } ) );
    }

    TEST( Query, Min, FiveStrings )
    {
        std::vector<std::string> src = { "hell", "apple", "zip" };

        auto rng = Query( src );

        EXPECT_EQ( "apple", rng.Min() );
        EXPECT_EQ( "zip", rng.Min( []( std::string s ) {return s.size(); } ) );
    }

    TEST( Query, OrderByRange, RandomIntsWithDuplicates )
    {
        S32 src[] = { 4, 5, 3, 1, 4, 2, 1, 4, 6 };
        S32 ans[] = { 1, 1, 2, 3, 4, 4, 4, 5, 6 };

        auto rng = Query( src );
        auto dst = rng.OrderBy();

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, OrderByRange, ReverseInts )
    {
        S32 src[] = { 4, 3, 2, 1 };
        S32 ans[] = { 1, 2, 3, 4 };

        auto rng = Query( src );
        auto dst = rng.OrderBy();

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, OrderByRange, ThreeElements )
    {
        S32 src[] = { 1, 3, 2 };
        S32 ans[] = { 1, 2, 3 };

        auto rng = Query( src );
        auto dst = rng.OrderBy();

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, OrderByRange, OneElement )
    {
        S32 src[] = { 5 };
        S32 ans[] = { 5 };

        auto rng = Query( src );
        auto dst = rng.OrderBy();

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, OrderByRange, NoElements )
    {
        std::vector<S32> src;

        auto rng = Query( src );
        auto dst = rng.OrderBy();

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    TEST( Query, OrderByRange, RandomStringByContent )
    {
        std::string src[] =
        {
            "microsoft",
            "intel",
            "nokia",
            "apple",
            "oracle",
            "sun",
        };

        std::string ans[] =
        {
            "apple",
            "intel",
            "microsoft",
            "nokia",
            "oracle",
            "sun",
        };

        auto rng = Query( src );
        auto dst = rng.OrderBy();

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, OrderByRange, RandomStringByLength )
    {
        std::string src[] =
        {
            "microsoft",
            "intel",
            "nokia",
            "apple",
            "oracle",
            "sun",
        };

        std::string ans[] =
        {
            "sun",
            "intel",
            "nokia",
            "apple",
            "oracle",
            "microsoft",
        };

        auto rng = Query( src );
        auto dst = rng.OrderBy( []( std::string a ) {return a.size(); } );

        CheckRangeEqArray( dst, ans, []( const std::string & s ) {return s.size(); } );
    }

    TEST( Query, OrderByRange, Composite )
    {
        std::vector< std::string > src =
        {
            "ab",
            "de",
            "fe",
            "ab",
            "de",
            "fe",
            "xe"
        };

        std::vector< std::string > ans =
        {
            "ab",
            "ab",
            "de",
            "de",
            "fe",
            "fe",
            "xe"
        };

        auto rng = Query( src );
        auto dst = rng.OrderBy( []( std::string a ) { return std::make_tuple( a[0], a[1] ); } ).ToVector();

        EXPECT_THAT( dst, ::testing::ContainerEq( ans ) );
    }

    TEST( Query, OrderByRange, Composite2 )
    {
        std::vector< std::string > src =
        {
            "xb",
            "de",
            "fe",
            "gb",
            "de",
            "fe",
            "xe"
        };

        std::vector< std::string > ans =
        {
            "gb",
            "xb",
            "de",
            "de",
            "fe",
            "fe",
            "xe"
        };

        auto rng = Query( src );
        auto dst = rng.OrderBy( []( std::string a ) { return std::make_tuple( a[1], a[0] ); } ).ToVector();

        EXPECT_THAT( dst, ::testing::ContainerEq( ans ) );
    }

    TEST( Query, ReverseRange, IntVector )
    {
        S32 src[] = { 1, 2, 3, 4 };
        S32 ans[] = { 4, 3, 2, 1 };

        auto rng = Query( src );
        auto dst = rng.Reverse();

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, ReverseRange, DoubleReverse )
    {
        S32 src[] = { 1, 2, 3, 4 };
        S32 ans[] = { 1, 2, 3, 4 };

        auto rng = Query( src );
        auto dst = rng.Reverse().Reverse();

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, SelectRange, Mul2 )
    {
        S32 src[] = { 1, 2, 3, 4 };
        S32 ans[] = { 2, 4, 6, 8 };

        auto rng = Query( src );
        auto dst = rng.Select( []( S32 a ) {return a * 2; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, SelectRange, MakeChar )
    {
        S32 src[] = { 1, 2, 3, 4 };
        char ans[] = { '1', '2', '3', '4' };

        auto rng = Query( src );
        auto dst = rng.Select( []( S32 a ) {return static_cast<S8>( '0' + a ); } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, SelectRange, MakeString )
    {
        S32 src[] = { 1, 2, 3, 4 };

        static std::string ans[] =
        {
            "hello",
            "world",
            "apple",
            "intel",
        };

        auto rng = Query( src );
        auto dst = rng.Select( []( S32 a ) {return ans[a - 1]; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, SkipRange, ManyToMany )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };
        S32 ans[] = { 1, 2, 3, 4, 5, 6 };

        auto rng = Query( src );
        auto dst = rng.Skip( 0 );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, SkipRange, ManyToLess )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };
        S32 ans[] = { 4, 5, 6 };

        auto rng = Query( src );
        auto dst = rng.Skip( 3 );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, SkipRange, ManyToOne )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };
        S32 ans[] = { 6 };

        auto rng = Query( src );
        auto dst = rng.Skip( 5 );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, SkipRange, ManyToZero )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };

        auto rng = Query( src );
        auto dst = rng.Skip( 6 );

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    TEST( Query, SkipRange, ManyToZeroLess )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };

        auto rng = Query( src );
        auto dst = rng.Skip( 10 );

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    TEST( Query, SkipRange, OneToOne )
    {
        S32 src[] = { 5 };
        S32 ans[] = { 5 };

        auto rng = Query( src );
        auto dst = rng.Skip( 0 );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, SkipRange, OneToZero )
    {
        S32 src[] = { 5 };

        auto rng = Query( src );
        auto dst = rng.Skip( 1 );

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    TEST( Query, SkipRange, OneToZeroLess )
    {
        S32 src[] = { 5 };

        auto rng = Query( src );
        auto dst = rng.Skip( 2 );

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    TEST( Query, SkipRange, ZeroToZero )
    {
        std::vector<S32> src;

        auto rng = Query( src );
        auto dst = rng.Skip( 0 );

        EXPECT_THROW( rng.Next(), EnumeratorEndException );
    }

    TEST( Query, SkipRange, ZeroToZeroLess )
    {
        std::vector<S32> src;

        auto rng = Query( src );
        auto dst = rng.Skip( 2 );

        EXPECT_THROW( rng.Next(), EnumeratorEndException );
    }


    TEST( Query, SkipWhileRange, ManyToMore )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };
        S32 ans[] = { 1, 2, 3, 4, 5, 6 };

        auto rng = Query( src );
        auto dst = rng.SkipWhile( []( S32 it ) {return it < 0 || it > 10; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, SkipWhileRange_i, ManyToMoreByIndex )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };
        S32 ans[] = { 1, 2, 3, 4, 5, 6 };

        auto rng = Query( src );
        auto dst = rng.SkipWhile( []( S32, size_t idx ) {return idx > 10; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, SkipWhileRange_i, ManyToMoreByItemValue )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };
        S32 ans[] = { 1, 2, 3, 4, 5, 6 };

        auto rng = Query( src );
        auto dst = rng.SkipWhile( []( S32 it, size_t ) {return it < 0 || it > 10; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, SkipWhileRange_i, ManyToMoreByIndexAndItemValue )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };
        S32 ans[] = { 1, 2, 3, 4, 5, 6 };

        auto rng = Query( src );
        auto dst = rng.SkipWhile( []( S32 it, size_t idx ) {return idx * it > 0; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, SkipWhileRange, ManyToMany )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };
        S32 ans[] = { 1, 2, 3, 4, 5, 6 };

        auto rng = Query( src );
        auto dst = rng.SkipWhile( []( S32 it ) {return it < 0 && it > 6; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, SkipWhileRange_i, ManyToManyByIndex )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };
        S32 ans[] = { 1, 2, 3, 4, 5, 6 };

        auto rng = Query( src );
        auto dst = rng.SkipWhile( []( S32, size_t idx ) {return idx > 5; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, SkipWhileRange_i, ManyToManyByItemValue )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };
        S32 ans[] = { 1, 2, 3, 4, 5, 6 };

        auto rng = Query( src );
        auto dst = rng.SkipWhile( []( S32 it, size_t ) {return it < 1 || it > 6; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, SkipWhileRange_i, ManyToManyByIndexAndItemValue )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };
        S32 ans[] = { 1, 2, 3, 4, 5, 6 };

        auto rng = Query( src );
        auto dst = rng.SkipWhile( []( S32 it, size_t idx ) {return idx > 5 || it < 0; } );

        CheckRangeEqArray( dst, ans );
    }


    TEST( Query, SkipWhileRange, ManyToLess )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };
        S32 ans[] = { 3, 4, 5, 6 };

        auto rng = Query( src );
        auto dst = rng.SkipWhile( []( S32 it ) {return it < 3 || it > 4; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, SkipWhileRange_i, ManyToLessByIndex )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };
        S32 ans[] = { 4, 5, 6 };

        auto rng = Query( src );
        auto dst = rng.SkipWhile( []( S32, size_t idx ) {return idx < 3 || idx > 3; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, SkipWhileRange_i, ManyToLessByItemValue )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };
        S32 ans[] = { 3, 4, 5, 6 };

        auto rng = Query( src );
        auto dst = rng.SkipWhile( []( S32 it, size_t ) {return it < 3 || it > 4; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, SkipWhileRange_i, ManyToLessByIndexAndItemValue )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };
        S32 ans[] = { 4, 5, 6 };

        auto rng = Query( src );
        auto dst = rng.SkipWhile( []( S32 it, size_t idx ) {return idx * it < 7; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, SkipWhileRange, ManyToOne )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };
        S32 ans[] = { 6 };

        auto rng = Query( src );
        auto dst = rng.SkipWhile( []( S32 it ) {return it != 6; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, SkipWhileRange_i, ManyToOneByIndex )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };
        S32 ans[] = { 6 };

        auto rng = Query( src );
        auto dst = rng.SkipWhile( []( S32, size_t idx ) {return idx < 5; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, SkipWhileRange_i, ManyToOneByItemValue )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };
        S32 ans[] = { 6 };

        auto rng = Query( src );
        auto dst = rng.SkipWhile( []( S32 it, size_t ) {return it < 6; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, SkipWhileRange_i, ManyToOneByIndexAndItemValue )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };
        S32 ans[] = { 6 };

        auto rng = Query( src );
        auto dst = rng.SkipWhile( []( S32 it, size_t idx ) {return idx * it < 30; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, SkipWhileRange, ManyToZero )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };

        auto rng = Query( src );
        auto dst = rng.SkipWhile( []( S32 it ) {return it > 0; } );

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    TEST( Query, SkipWhileRange_i, ManyToZeroeByIndex )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };

        auto rng = Query( src );
        auto dst = rng.SkipWhile( []( S32, size_t idx ) {return idx < 6; } );

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    TEST( Query, SkipWhileRange_i, ManyToZeroByItemValue )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };

        auto rng = Query( src );
        auto dst = rng.SkipWhile( []( S32 it, size_t ) {return it > 0; } );

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    TEST( Query, SkipWhileRange_i, ManyToZeroIndexAndItemValue )
    {
        size_t src[] = { 1, 2, 3, 4, 5, 6 };

        auto rng = Query( src );
        auto dst = rng.SkipWhile( []( size_t it, size_t idx ) {return idx != it; } );

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    TEST( Query, SkipWhileRange, OneToOne )
    {
        S32 src[] = { 5 };
        S32 ans[] = { 5 };

        auto rng = Query( src );
        auto dst = rng.SkipWhile( []( S32 it ) {return it != 5; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, SkipWhileRange_i, OneToOneByIndex )
    {
        S32 src[] = { 6 };
        S32 ans[] = { 6 };

        auto rng = Query( src );
        auto dst = rng.SkipWhile( []( S32, size_t idx ) {return idx > 0; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, SkipWhileRange_i, OneToOneByItemValue )
    {
        S32 src[] = { 6 };
        S32 ans[] = { 6 };

        auto rng = Query( src );
        auto dst = rng.SkipWhile( []( S32 it, size_t ) {return it != 6; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, SkipWhileRange_i, OneToOneByIndexAndItemValue )
    {
        S32 src[] = { 6 };
        S32 ans[] = { 6 };

        auto rng = Query( src );
        auto dst = rng.SkipWhile( []( S32 it, size_t idx ) {return idx != 0 || it != 6; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, SkipWhileRange, OneToZero )
    {
        S32 src[] = { 5 };

        auto rng = Query( src );
        auto dst = rng.SkipWhile( []( S32 it ) {return it == 5; } );

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    TEST( Query, SkipWhileRange_i, OneToZeroByIndex )
    {
        S32 src[] = { 6 };

        auto rng = Query( src );
        auto dst = rng.SkipWhile( []( S32, size_t idx ) {return idx < 6; } );

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    TEST( Query, SkipWhileRange_i, OneToZeroByItemValue )
    {
        S32 src[] = { 6 };

        auto rng = Query( src );
        auto dst = rng.SkipWhile( []( S32 it, size_t ) {return it > 0; } );

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    TEST( Query, SkipWhileRange_i, OneToZeroIndexAndItemValue )
    {
        size_t src[] = { 6 };

        auto rng = Query( src );
        auto dst = rng.SkipWhile( []( size_t it, size_t idx ) {return idx != it; } );

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    TEST( Query, SkipWhileRange, ZeroToZero )
    {
        std::vector<S32> src;

        auto rng = Query( src );
        auto dst = rng.SkipWhile( []( S32 ) {return false; } );

        EXPECT_THROW( rng.Next(), EnumeratorEndException );
    }

    TEST( Query, SkipWhileRange_i, ZeroToZero )
    {
        std::vector<S32> src;

        auto rng = Query( src );
        auto dst = rng.SkipWhile( []( S32, size_t ) {return true; } );

        EXPECT_THROW( rng.Next(), EnumeratorEndException );
    }

    TEST( Query, Sum, ThreeInts )
    {
        std::vector<S32> src = {1, 2, 3};

        auto rng = Query( src ).Sum();

        EXPECT_EQ( 6, rng );
    }

    TEST( Query, Sum, FiveInts )
    {
        std::vector<S32> src = {1, 2, 3, 4, 5};

        auto rng = Query( src );
        auto dst0 = rng.Where( []( S32 a ) {return a % 2 == 0; } ).Sum();
        auto dst1 = rng.Where( []( S32 a ) {return a % 2 == 1; } ).Sum();

        EXPECT_EQ( 6, dst0 );
        EXPECT_EQ( 9, dst1 );
    }

    TEST( Query, Sum, BoolSum )
    {
        std::vector<S32> src = { 1, 2, 3, 4, 5 };

        auto rng1 = Query( src ).Sum<S32>( []( S32 a ) {return a % 2 == 0; } );
        auto rng2 = Query( src ).Sum<S32>( []( S32 a ) {return a % 2 == 1; } );

        EXPECT_EQ( 2, rng1 );
        EXPECT_EQ( 3, rng2 );
    }

    TEST( Query, Sum, FiveStringsLen )
    {
        std::vector<std::string> src = { "hello", "apple", "nokia", "oracle", "ponny" };

        auto rng = Query( src ).Sum( []( const std::string & str ) {return str.size(); } );

        EXPECT_EQ( 26U, rng );
    }

    TEST( Query, Sum, FiveStringsData )
    {
        std::vector<std::string> src = { "hello", "apple", "nokia", "oracle", "ponny" };

        auto rng = Query( src ).Sum();

        std::string ans = "helloapplenokiaoracleponny";

        EXPECT_EQ( ans, rng );
    }

    TEST( Query, TakeRange, ManyToMore )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };
        S32 ans[] = { 1, 2, 3, 4, 5, 6 };

        auto rng = Query( src );
        auto dst = rng.Take( 10 );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, TakeRange, ManyToMany )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };
        S32 ans[] = { 1, 2, 3, 4, 5, 6 };

        auto rng = Query( src );
        auto dst = rng.Take( 6 );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, TakeRange, ManyToLess )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };
        S32 ans[] = { 1, 2, 3 };

        auto rng = Query( src );
        auto dst = rng.Take( 3 );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, TakeRange, ManyToOne )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };
        S32 ans[] = { 1 };

        auto rng = Query( src );
        auto dst = rng.Take( 1 );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, TakeRange, ManyToZero )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };

        auto rng = Query( src );
        auto dst = rng.Take( 0 );

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    TEST( Query, TakeRange, OneToMore )
    {
        S32 src[] = { 5 };
        S32 ans[] = { 5 };

        auto rng = Query( src );
        auto dst = rng.Take( 5 );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, TakeRange, OneToOne )
    {
        S32 src[] = { 5 };
        S32 ans[] = { 5 };

        auto rng = Query( src );
        auto dst = rng.Take( 1 );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, TakeRange, OneToZero )
    {
        S32 src[] = { 5 };

        auto rng = Query( src );
        auto dst = rng.Take( 0 );

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    TEST( Query, TakeRange, ZeroToZero )
    {
        std::vector<S32> src;

        auto rng = Query( src );
        auto dst = rng.Take( 0 );

        EXPECT_THROW( rng.Next(), EnumeratorEndException );
    }


    TEST( Query, TakeWhileRange, ManyToMore )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };
        S32 ans[] = { 1, 2, 3, 4, 5, 6 };

        auto rng = Query( src );
        auto dst = rng.TakeWhile( []( S32 it ) {return it >= 0 && it <= 10; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, TakeWhileRange_i, ManyToMoreByIndex )
    {
        S32 src[] = { 1, 3, 5, 7, 9, 11 };
        S32 ans[] = { 1, 3, 5, 7, 9 };

        auto rng = Query( src );
        auto dst = rng.TakeWhile( []( S32, size_t idx ) {return idx < 5; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, TakeWhileRange_i, ManyToMoreByItemValue )
    {
        S32 src[] = { 1, 3, 5, 7, 9, 11 };
        S32 ans[] = { 1, 3, 5, 7, 9, 11 };

        auto rng = Query( src );
        auto dst = rng.TakeWhile( []( S32 it, size_t ) {return it % 2 != 0; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, TakeWhileRange_i, ManyToMoreByIndexAndItemValue )
    {
        S32 src[] = { 1, 3, 5, 7, 9, 11 };
        S32 ans[] = { 1, 3, 5, 7, 9, 11 };

        auto rng = Query( src );
        auto dst = rng.TakeWhile( []( S32 it, size_t idx ) {return idx < 3 || it > 5; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, TakeWhileRange, ManyToMany )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };
        S32 ans[] = { 1, 2, 3, 4, 5, 6 };

        auto rng = Query( src );
        auto dst = rng.TakeWhile( []( S32 it ) {return it >= 1 && it <= 6; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, TakeWhileRange_i, ManyToManyByIndex )
    {
        S32 src[] = { 1, 3, 5, 7, 9, 11 };
        S32 ans[] = { 1, 3, 5, 7, 9, 11 };

        auto rng = Query( src );
        auto dst = rng.TakeWhile( []( S32, size_t idx ) {return idx < 6; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, TakeWhileRange_i, ManyToManyByItemValue )
    {
        S32 src[] = { 1, 3, 5, 7, 9, 11 };
        S32 ans[] = { 1, 3, 5, 7, 9, 11 };

        auto rng = Query( src );
        auto dst = rng.TakeWhile( []( S32 it, size_t ) {return it > 0 && it < 12; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, TakeWhileRange_i, ManyToManyByIndexAndItemValue )
    {
        S32 src[] = { 1, 3, 5, 7, 9, 11 };
        S32 ans[] = { 1, 3, 5, 7, 9, 11 };

        auto rng = Query( src );
        auto dst = rng.TakeWhile( []( S32 it, size_t idx ) {return idx < 3 || ( it > 5 && it < 12 ); } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, TakeWhileRange, ManyToLess )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };
        S32 ans[] = { 1, 2, 3 };

        auto rng = Query( src );
        auto dst = rng.TakeWhile( []( S32 it ) {return it != 4; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, TakeWhileRange_i, ManyToLessByIndex )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
        S32 ans[] = { 1, 2, 3, 4 };

        auto rng = Query( src );
        auto dst = rng.TakeWhile( []( S32, size_t idx ) {return idx < 4; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, TakeWhileRange_i, ManyToLessByItemValue )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
        S32 ans[] = { 1, 2, 3, 4 };

        auto rng = Query( src );
        auto dst = rng.TakeWhile( []( S32 it, size_t ) {return it < 5; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, TakeWhileRange_i, ManyToLessByIndexAndItemValue )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
        S32 ans[] = { 1, 2, 3, 4 };

        auto rng = Query( src );
        auto dst = rng.TakeWhile( []( S32 it, size_t idx ) {return idx < 4 && it > 0; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, TakeWhileRange, ManyToOne )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };
        S32 ans[] = { 1 };

        auto rng = Query( src );
        auto dst = rng.TakeWhile( []( S32 it ) {return it % 2 != 0; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, TakeWhileRange_i, ManyToOneByIndex )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
        S32 ans[] = { 1 };

        auto rng = Query( src );
        auto dst = rng.TakeWhile( []( S32, size_t idx ) {return idx == 0; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, TakeWhileRange_i, ManyToOneByItemValue )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
        S32 ans[] = { 1 };

        auto rng = Query( src );
        auto dst = rng.TakeWhile( []( S32 it, size_t ) {return it == 1; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, TakeWhileRange_i, ManyToOneByIndexAndItemValue )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };
        S32 ans[] = { 1 };

        auto rng = Query( src );
        auto dst = rng.TakeWhile( []( S32 it, size_t idx ) {return idx < 5 && it < 2; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, TakeWhileRange, ManyToZero )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };

        auto rng = Query( src );
        auto dst = rng.TakeWhile( []( S32 it ) {return it < 0; } );

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    TEST( Query, TakeWhileRange_i, ManyToZeroByIndex )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

        auto rng = Query( src );
        auto dst = rng.TakeWhile( []( S32, size_t idx ) {return idx > 0; } );

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    TEST( Query, TakeWhileRange_i, ManyToZeroByItemValue )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

        auto rng = Query( src );
        auto dst = rng.TakeWhile( []( S32 it, size_t ) {return it > 2; } );

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    TEST( Query, TakeWhileRange_i, ManyToZeroByIdexAndItemValue )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11 };

        auto rng = Query( src );
        auto dst = rng.TakeWhile( []( S32 it, size_t idx ) {return it < 0 || idx > 0; } );

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    TEST( Query, TakeWhileRange, OneToOne )
    {
        S32 src[] = { 5 };
        S32 ans[] = { 5 };

        auto rng = Query( src );
        auto dst = rng.TakeWhile( []( S32 it ) {return it > 0; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, TakeWhileRange_i, OneToOneByIndex )
    {
        S32 src[] = { 5 };
        S32 ans[] = { 5 };

        auto rng = Query( src );
        auto dst = rng.TakeWhile( []( S32, size_t idx ) {return idx < 1; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, TakeWhileRange_i, OneToOneByItemValue )
    {
        S32 src[] = { 5 };
        S32 ans[] = { 5 };

        auto rng = Query( src );
        auto dst = rng.TakeWhile( []( S32 it, size_t ) {return it > 2; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, TakeWhileRange_i, OneToOneByIndexAndItemValue )
    {
        S32 src[] = { 5 };
        S32 ans[] = { 5 };

        auto rng = Query( src );
        auto dst = rng.TakeWhile( []( S32 it, size_t idx ) {return idx == 0 && it > 2; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, TakeWhileRange, OneToZero )
    {
        S32 src[] = { 5 };

        auto rng = Query( src );
        auto dst = rng.TakeWhile( []( S32 ) {return false; } );

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    TEST( Query, TakeWhileRange_i, OneToZeroByIndex )
    {
        S32 src[] = { 5 };

        auto rng = Query( src );
        auto dst = rng.TakeWhile( []( S32, size_t idx ) {return idx > 0; } );

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    TEST( Query, TakeWhileRange_i, OneToZeroByItemValue )
    {
        S32 src[] = { 5 };

        auto rng = Query( src );
        auto dst = rng.TakeWhile( []( S32 it, size_t ) {return it < 5; } );

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    TEST( Query, TakeWhileRange_i, OneToZeroByIndexAndItemValue )
    {
        S32 src[] = { 5 };

        auto rng = Query( src );
        auto dst = rng.TakeWhile( []( S32 it, size_t idx ) {return idx == 0 && it > 5; } );

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    TEST( Query, TakeWhileRange, ZeroToZero )
    {
        std::vector<S32> src;

        auto rng = Query( src );
        auto dst = rng.TakeWhile( []( S32 ) {return false; } );

        EXPECT_THROW( rng.Next(), EnumeratorEndException );
    }

    TEST( Query, TakeWhileRange_i, ZeroToZero )
    {
        S32 src[] = { 5 };

        auto rng = Query( src );
        auto dst = rng.TakeWhile( []( S32, size_t ) {return false; } );

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    TEST( Query, ToDeque, Deque2Deque )
    {
        std::deque<S32> src = { 100, 200, 300 };

        auto rng = Query( src );
        auto dst = rng.ToDeque();

        EXPECT_EQ( dst, src );
    }

    TEST( Query, ToList, List2List )
    {
        std::list<S32> src = { 100, 200, 300 };

        auto rng = Query( src );
        auto dst = rng.ToList();

        EXPECT_EQ( dst, src );
    }

    TEST( Query, ToVector, Vector2Vector )
    {
        std::vector<S32> src = { 100, 200, 300 };

        auto rng = Query( src );
        auto dst = rng.ToVector();

        EXPECT_EQ( dst, src );
    }

    TEST( Query, ToSet, Set2Set )
    {
        std::set<S32> src = { 1, 1, 2, 3, 2 };

        auto rng = Query( src );
        auto dst = rng.ToSet();

        EXPECT_EQ( 3u, dst.size() );
        EXPECT_NE( dst.end(), dst.find( 1 ) );
        EXPECT_NE( dst.end(), dst.find( 2 ) );
        EXPECT_NE( dst.end(), dst.find( 3 ) );
    }

    TEST( UnbitsRange, OneByteDefault )
    {
        S32 src[] = { 1, 0, 1, 0, 1, 0, 1, 0 };
        S32 ans[] = { 0xAA };

        auto rng = Query( src );
        auto dst = rng.Unbits();

        CheckRangeEqArray( dst, ans );
    }

    TEST( UnbitsRange, OneByteHL )
    {
        S32 src[] = { 1, 0, 1, 0, 1, 0, 1, 0 };
        S32 ans[] = { 0xAA };

        auto rng = Query( src );
        auto dst = rng.Unbits( BitsDirection::HighToLow );

        CheckRangeEqArray( dst, ans );
    }

    TEST( UnbitsRange, OneByteLH )
    {
        S32 src[] = { 0, 1, 0, 1, 0, 1, 0, 1 };
        S32 ans[] = { 0xAA };

        auto rng = Query( src );
        auto dst = rng.Unbits( BitsDirection::LowToHigh );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, UnbytesRange, OneIntDefault )
    {
        U8 src[] = { 0xAA, 0xBB, 0xCC, 0xDD };
        U32 ans[] = { 0xDDCCBBAA };

        auto rng = Query( src );
        auto dst = rng.Unbytes<U32>();

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, UnbytesRange, OneIntFL )
    {
        U8 src[] = { 0xAA, 0xBB, 0xCC, 0xDD };
        U32 ans[] = { 0xDDCCBBAA };

        auto rng = Query( src );
        auto dst = rng.Unbytes<U32>( BytesDirection::FirstToLast );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, UnbytesRange, OneIntLF )
    {
        U8 src[] = { 0xAA, 0xBB, 0xCC, 0xDD };
        U32 ans[] = { 0xAABBCCDD };

        auto rng = Query( src );
        auto dst = rng.Unbytes<U32>( BytesDirection::LastToFirst );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, UnbytesRange, TwoIntsDefault )
    {
        U8 src[] =
        {
            0x78, 0x56, 0x34, 0x12,
            0xAA, 0xBB, 0xCC, 0xDD,
        };
        U32 ans[] = { 0x12345678, 0xDDCCBBAA };

        auto rng = Query( src );
        auto dst = rng.Unbytes<U32>();

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, UnbytesRange, TwoIntsFL )
    {
        U8 src[] =
        {
            0x78, 0x56, 0x34, 0x12,
            0xAA, 0xBB, 0xCC, 0xDD,
        };
        U32 ans[] = { 0x12345678, 0xDDCCBBAA };

        auto rng = Query( src );
        auto dst = rng.Unbytes<U32>( BytesDirection::FirstToLast );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, UnbytesRange, TwoIntsLF )
    {
        U8 src[] =
        {
            0x78, 0x56, 0x34, 0x12,
            0xAA, 0xBB, 0xCC, 0xDD,
        };
        U32 ans[] = { 0x78563412, 0xAABBCCDD };

        auto rng = Query( src );
        auto dst = rng.Unbytes<U32>( BytesDirection::LastToFirst );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, UnbytesRange, EmptyDefault )
    {
        std::vector<U8> src;

        auto rng = Query( src );
        auto dst = rng.Unbytes<S32>();

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    TEST( Query, WhereRange, IntOdd )
    {
        S32 src[] = { 1, 2, 3, 4, 5, 6 };
        S32 ans[] = { 1,  3,  5 };

        auto rng = Query( src );
        auto dst = rng.Where( []( S32 a ) {return a % 2 == 1; } );

        CheckRangeEqArray( dst, ans );
    }

    //////////////////////////////////////////////////////////////////////////

    TEST( Query, WhereRange, FirstLetterFront )
    {
        std::string src[] =
        {
            "apple",
            "blackberry",
            "adobe",
            "microsoft",
            "nokia",
        };

        std::string ans[] =
        {
            "apple",
            "adobe",
        };

        auto rng = Query( src );
        auto dst = rng.Where( []( std::string a ) {return a[0] == 'a'; } );

        CheckRangeEqArray( dst, ans );
    }

    //////////////////////////////////////////////////////////////////////////

    TEST( Query, WhereRange, NameAgeLess )
    {
        struct NameAge
        {
            std::string name;
            S32 age;
        };

        NameAge src[] =
        {
            { "man1", 20 },
            { "man2", 15 },
            { "man3", 30 },
            { "man4", 14 },
            { "man5", 18 },
        };

        NameAge ans[] =
        {
            { "man2", 20 },
            { "man4", 15 },
        };

        auto rng = Query( src );
        auto dst = rng.Where( []( const NameAge & a ) {return a.age < 18; } );

        CheckRangeEqArray( dst, ans, []( const NameAge & a ) {return a.name; } );
    }

    //////////////////////////////////////////////////////////////////////////

    TEST( Query, WhereRange, MayToOne )
    {
        S32 src[] = { 0, 1, 2 };
        S32 ans[] = { 1 };

        auto rng = Query( src );
        auto dst = rng.Where( []( S32 a ) {return a == 1; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, WhereRange, OneToOne )
    {
        S32 src[] = { 5 };
        S32 ans[] = { 5 };

        auto rng = Query( src );
        auto dst = rng.Where( []( S32 a ) {return a > 0; } );

        CheckRangeEqArray( dst, ans );
    }

    TEST( Query, WhereRange, ManyToZero )
    {
        S32 src[] = { 0, 1, 2 };

        auto rng = Query( src );
        auto dst = rng.Where( []( S32 a ) {return a == 5; } );

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    TEST( Query, WhereRange, OneToZero )
    {
        S32 src[] = { 5 };

        auto rng = Query( src );
        auto dst = rng.Where( []( S32 a ) {return a > 10; } );

        EXPECT_THROW( dst.Next(), EnumeratorEndException );
    }

    TEST( Query, WhereRange, ZeroToZero )
    {
        std::vector<S32> src;

        auto rng = Query( src );
        auto dst = rng.Where( []( S32 a ) {return a > 0; } );

        EXPECT_THROW( rng.Next(), EnumeratorEndException );
    }
}
