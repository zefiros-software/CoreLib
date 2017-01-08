// Copyright (C) <year> <copyright holders>
//
// Permission is hereby granted, free of charge, to Any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
//
// This heavily based off Query https://github.com/k06a/boolinq

#pragma once
#ifndef __QUERY_H__
#define __QUERY_H__

#include "common/types.h"

#include <set>
#include <list>
#include <deque>
#include <string>
#include <vector>
#include <functional>
#include <climits>

enum class BytesDirection
{
    FirstToLast = 0x00,
    LastToFirst = 0x01
};

enum class BitsDirection
{
    HighToLow = 0x00,
    LowToHigh = 0x01
};

class EnumeratorEndException
{
};

template<typename tT, typename tV>
class Enumerator
{
public:
    typedef tT value_type;

    Enumerator( std::function<tT( tV & )> next, tV value )
        : mNext( next ),
          mValue( value )
    {
    }

    tT Next()
    {
        return mNext( mValue );
    }

private:
    std::function<tT( tV & )> mNext;
    tV mValue;
};

template<typename tV, typename tC>
class IteratorContainerPair
{
public:

    std::function<tV( const tC & )> func;
    tC second;
    tV first;

    IteratorContainerPair( const tC &tc, std::function<tV( const tC & )> func )
        : func( func ),
          second( tc ),
          first( func( second ) )
    {
    }

    IteratorContainerPair( const IteratorContainerPair<tV, tC> &pair )
        : func( pair.func ),
          second( pair.second ),
          first( func( second ) )
    {
        for ( auto it = pair.func( pair.second ); it != pair.first; ++it )
        {
            ++first;
        }
    }
};

template<typename tV, typename tR>
class TransformComparer
{
public:

    TransformComparer( std::function<tR( tV )> func )
        : mFunc( func )
    {
    }

    bool operator()( const tV &a, const tV &b ) const
    {
        return mFunc( a ) < mFunc( b );
    }

private:

    std::function<tR( tV )> mFunc;
};

template<typename tFunc, typename TArg>
inline static auto GetReturnType( tFunc *func = nullptr, TArg *arg1 = nullptr )
-> decltype( ( *func )( *arg1 ) );

template<typename tFunc, typename T1Arg, typename T2Arg>
inline static auto GetReturnType( tFunc *func = nullptr, T1Arg *arg1 = nullptr, T2Arg *arg2 = nullptr )
-> decltype( ( *func )( *arg1, *arg2 ) );

template<typename tT>
class QueryObj
{
public:

    typedef typename tT::value_type tV;
    typedef typename tT::value_type value_type;


    typedef Enumerator<size_t, std::pair<size_t, std::pair<tT, tV> > > ByteEnum;
    typedef Enumerator< size_t, std::pair< size_t, std::pair<QueryObj< ByteEnum >, U8 > > > BitEnum;

    tT enumerator;

    QueryObj( tT enumerator )
        : enumerator( enumerator )
    {
    }

    tV Next()
    {
        return enumerator.Next();
    }

    void Foreach( std::function<void( tV, size_t )> action ) const
    {
        auto en = enumerator;
        size_t index = 0;

        try
        {
            for ( ;; )
            {
                action( en.Next(), ++index );
            }
        }
        catch ( EnumeratorEndException & )
        {
        }
    }

    void Foreach( std::function<void( tV )> action ) const
    {
        Foreach( [ = ]( tV a, size_t )
        {
            return action( a );
        } );
    }

    QueryObj<Enumerator<tV, std::pair<tT, size_t> > > Where( std::function<bool( tV, size_t )> predicate ) const
    {
        return Enumerator<tV, std::pair<tT, size_t> >( [ = ]( std::pair<tT, size_t> &pair )->tV
        {
            tV object;

            do
            {
                object = pair.first.Next();
            }
            while ( !predicate( object, pair.second++ ) );

            return object;
        }, { enumerator, 0 } );
    }

    QueryObj<Enumerator<tV, std::pair<tT, size_t> > > Where( std::function<bool( tV )> predicate ) const
    {
        return Where( [ = ]( tV a, size_t )
        {
            return predicate( a );
        } );
    }

    QueryObj<Enumerator<tV, std::pair<tT, size_t> > > Take( size_t count ) const
    {
        return Where( [ = ]( tV, size_t i )
        {
            if ( i == count )
            {
                throw EnumeratorEndException();
            }

            return true;
        } );
    }

    QueryObj<Enumerator<tV, std::pair<tT, size_t> > > TakeWhile( std::function<bool( tV, size_t )> predicate ) const
    {
        return Enumerator<tV, std::pair<tT, size_t> >( [ = ]( std::pair<tT, size_t> &pair )->tV
        {
            tV object = pair.first.Next();

            if ( !predicate( object, pair.second++ ) )
            {
                throw EnumeratorEndException();
            }

            return object;
        }, { enumerator, 0 } );
    }

    QueryObj<Enumerator<tV, std::pair<tT, size_t> > > TakeWhile( std::function<bool( tV )> predicate ) const
    {
        return TakeWhile( [ = ]( tV t, size_t )
        {
            return predicate( t );
        } );
    }

    QueryObj<Enumerator<tV, std::pair<tT, size_t> > > Skip( size_t count ) const
    {
        return Where( [ = ]( tV, size_t i )
        {
            return i >= count;
        } );
    }

    QueryObj<Enumerator<tV, std::pair<tT, size_t> > > SkipWhile( std::function<bool( tV, size_t )> predicate ) const
    {
        return Enumerator<tV, std::pair<tT, size_t> >( [ = ]( std::pair<tT, size_t> &pair )->tV
        {
            if ( pair.second != 0 )
            {
                return pair.first.Next();
            }

            tV object;

            do
            {
                object = pair.first.Next();
            }
            while ( predicate( object, pair.second++ ) );

            return object;
        }, { enumerator, 0 } );
    }

    QueryObj<Enumerator<tV, std::pair<tT, size_t> > > SkipWhile( std::function<bool( tV )> predicate ) const
    {
        return SkipWhile( [ = ]( tV t, size_t )
        {
            return predicate( t );
        } );
    }

    template<typename tR>
    QueryObj<Enumerator<tR, std::pair<tT, size_t> > > Select( std::function<tR( tV, size_t )> transform ) const
    {
        return Enumerator<tR, std::pair<tT, size_t> >( [ = ]( std::pair<tT, size_t> &pair )->tR
        {
            return transform( pair.first.Next(), pair.second++ );
        }, std::make_pair( enumerator, 0 ) );
    }

    template<typename tR>
    QueryObj<Enumerator<tR, std::pair<tT, size_t> > > Select( std::function<tR( tV )> transform ) const
    {
        return Select<tR>( [ = ]( tV a, size_t )
        {
            return transform( a );
        } );
    }

    template<typename tFunc>
    QueryObj<Enumerator<decltype( GetReturnType<tFunc, tV>() ), std::pair<tT, size_t> > > Select( tFunc transform ) const
    {
        return Select<decltype( GetReturnType<tFunc, tV>() )>( transform );
    }

    template<typename tR>
    QueryObj<Enumerator<tR, std::pair<tT, size_t> > > Cast() const
    {
        return Select<tR>( [ = ]( tV a, size_t )
        {
            return a;
        } );
    }

    template<typename tR>
    QueryObj<Enumerator<tV, std::pair<tT, std::set<tR> > > > Distinct( std::function<tR( tV )> transform ) const
    {
        typedef std::pair<tT, std::set<tR> > DataType;

        return Enumerator<tV, DataType>( [ = ]( DataType & pair )->tV
        {
            for ( ;; )
            {
                tV object = pair.first.Next();
                tR key = transform( object );

                if ( pair.second.find( key ) == pair.second.end() )
                {
                    pair.second.insert( key );
                    return object;
                }
            }
        }, std::make_pair( enumerator, std::set<tR>() ) );
    }

    template<typename tF>
    QueryObj<Enumerator<tV, std::pair<tT, std::set<decltype( GetReturnType<tF, tV>() )> > > > Distinct( tF transform ) const
    {
        return Distinct<decltype( GetReturnType<tF, tV>() )>( transform );
    }


    QueryObj<Enumerator<tV, std::pair<tT, std::set<tV> > > > Distinct() const
    {
        return Distinct<tV>( []( tV a )
        {
            return a;
        } );
    }

    template<typename tR>
    QueryObj<Enumerator<tV, IteratorContainerPair<typename std::multiset<tV, TransformComparer<tV, tR> >::iterator, std::multiset<tV, TransformComparer<tV, tR> > > > >
    OrderBy( std::function<tR( tV )> transform ) const
    {
        typedef IteratorContainerPair<typename std::multiset<tV, TransformComparer<tV, tR> >::iterator,
                std::multiset<tV, TransformComparer<tV, tR> > > DataType;

        std::multiset<tV, TransformComparer<tV, tR> > objects( transform );

        try
        {
            auto en = enumerator;

            for ( ;; )
            {
                objects.insert( en.Next() );
            }
        }
        catch ( EnumeratorEndException & )
        {
        }

        return Enumerator<tV, DataType>( []( DataType & pair )
        {
            return ( pair.first == pair.second.end() )
                   ? throw EnumeratorEndException() : *( pair.first++ );
        }, DataType( objects, []( const std::multiset<tV, TransformComparer<tV, tR> > &mset )
        {
            return mset.begin();
        } ) );
    }

    template<typename TFunc>
    QueryObj<Enumerator<tV, IteratorContainerPair<typename std::multiset<tV, TransformComparer<tV, decltype( GetReturnType<TFunc, tV>() )> >::iterator, std::multiset<tV, TransformComparer<tV, decltype( GetReturnType<TFunc, tV>() )> > > > >
    OrderBy( TFunc transform ) const
    {
        return OrderBy<decltype( GetReturnType<TFunc, tV>() )>( transform );
    }

    QueryObj<Enumerator<tV, IteratorContainerPair<typename std::multiset<tV, TransformComparer<tV, tV> >::iterator, std::multiset<tV, TransformComparer<tV, tV> > > > >
    OrderBy() const
    {
        return OrderBy<tV>( []( tV a )
        {
            return a;
        } );
    }

    QueryObj<Enumerator<tV, IteratorContainerPair<typename std::vector<tV>::const_reverse_iterator, std::vector<tV> > > >
    Reverse() const
    {
        typedef IteratorContainerPair<typename std::vector<tV>::const_reverse_iterator, std::vector<tV> > DataType;

        return Enumerator<tV, DataType>( []( DataType & pair )
        {
            return ( pair.first == pair.second.crend() )
                   ? throw EnumeratorEndException() : *( pair.first++ );
        }, DataType( ToVector(), []( const std::vector<tV> &vec )
        {
            return vec.crbegin();
        } ) );
    }

    template<typename tR>
    tR Aggregate( tR start, std::function<tR( tR, tV )> accumulate ) const
    {
        try
        {
            auto en = enumerator;

            for ( ;; )
            {
                start = accumulate( start, en.Next() );
            }
        }
        catch ( EnumeratorEndException & )
        {
        }

        return start;
    }

    template<typename tR>
    tR Sum( std::function<tR( tV )> transform ) const
    {
        return Aggregate<tR>( tR(), [ = ]( tR accumulator, tV object )
        {
            return accumulator + transform( object );
        } );
    }

    template<typename tFunc>
    decltype( GetReturnType<tFunc, tV>() ) Sum( tFunc transform ) const
    {
        return Sum<decltype( GetReturnType<tFunc, tV>() )>( transform );
    }

    template<typename tR>
    tR Sum() const
    {
        return Sum<tR>( []( tV a )
        {
            return a;
        } );
    }

    tV Sum() const
    {
        return Sum<tV>();
    }

    template<typename tR>
    tR Avg( std::function<tR( tV )> transform ) const
    {
#include "warnings/push.h"
#include "warnings/possibleLossOfData.h"
        size_t count = 0;
        return Aggregate<tR>( tR(), [&]( tR accumulator, tV object )->tR
        {
            ++count;
            return ( accumulator * ( count - 1 ) + transform( object ) ) / count;
        } );
#include "warnings/pop.h"
    }

    template<typename tR>
    tR Avg() const
    {
        return Avg<tR>( []( tV a )
        {
            return a;
        } );
    }

    tV Avg() const
    {
        return Avg<tV>();
    }

    size_t Count( std::function<bool( tV )> predicate ) const
    {
        return Aggregate<size_t>( 0, [ = ]( size_t count, tV a )
        {
            return count + ( predicate( a ) ? 1 : 0 );
        } );
    }

    size_t Count( const tV &value ) const
    {
        return Count( [ = ]( tV a )
        {
            return a == value;
        } );
    }

    size_t Count() const
    {
        return Count( []( tV )
        {
            return true;
        } );
    }

    bool Any( std::function<bool( tV )> predicate ) const
    {
        try
        {
            auto en = enumerator;

            for ( ;; )
                if ( predicate( en.Next() ) )
                {
                    return true;
                }
        }
        catch ( EnumeratorEndException & )
        {
        }

        return false;
    }

    bool Any() const
    {
        return Any( []( tV a )
        {
#include "warnings/push.h"
#include "warnings/forcingValueToBool.h"
            return static_cast<bool>( a );
#include "warnings/pop.h"
        } );
    }

    bool All( std::function<bool( tV )> predicate ) const
    {
        return !Any( [ = ]( tV a )
        {
            return !predicate( a );
        } );
    }

    bool All() const
    {
        return All( []( tV a )
        {
#include "warnings/push.h"
#include "warnings/forcingValueToBool.h"
            return static_cast<bool>( a );
#include "warnings/pop.h"
        } );
    }

    bool Contains( const tV &value ) const
    {
        return Any( [&]( tV a )
        {
            return value == a;
        } );
    }

    tV Elect( std::function<tV( tV, tV )> accumulate ) const
    {
        auto en = enumerator;
        tV result = en.Next();

        try
        {
            for ( ;; )
            {
                result = accumulate( result, en.Next() );
            }
        }
        catch ( EnumeratorEndException & ) {}

        return result;
    }

    template<typename tR>
    tV Max( std::function<tR( tV )> transform ) const
    {
        return Elect( [ = ]( tV a, tV b )
        {
            return transform( a ) < transform( b ) ? b : a;
        } );
    }

    template<typename tFunc>
    tV Max( tFunc transform ) const
    {
        return Max<decltype( GetReturnType<tFunc, tV>() )>( transform );
    }

    tV Max() const
    {
        return Max<tV>( []( tV a )
        {
            return a;
        } );
    }

    template<typename tR>
    tV Min( std::function<tR( tV )> transform ) const
    {
        return Elect( [ = ]( tV a, tV b )
        {
            return transform( a ) < transform( b ) ? a : b;
        } );
    }

    template<typename tFunc>
    tV Min( tFunc transform ) const
    {
        return Min<decltype( GetReturnType<tFunc, tV>() )>( transform );
    }

    tV Min() const
    {
        return Min<tV>( []( tV a )
        {
            return a;
        } );
    }

    tV ElementAt( size_t index ) const
    {
        auto en = enumerator;

        for ( size_t i = 0; i < index; ++i )
        {
            en.Next();
        }

        return en.Next();
    }

    tV First( std::function<bool( tV )> predicate ) const
    {
        return Where( predicate ).enumerator.Next();
    }

    tV First() const
    {
        return First( []( tV )
        {
            return true;
        } );
    }

    tV FirstOrDefault( std::function<bool( tV )> predicate )
    {
        try
        {
            return First( predicate );
        }
        catch ( EnumeratorEndException & )
        {
            return tV();
        }
    }

    tV FirstOrDefault() const
    {
        try
        {
            return First();
        }
        catch ( EnumeratorEndException & )
        {
            return tV();
        }
    }

    tV Last( std::function<bool( tV )> predicate ) const
    {
        auto linq = Where( predicate );
        tV object = linq.enumerator.Next();

        try
        {
            for ( ;; )
            {
                object = linq.enumerator.Next();
            }
        }
        catch ( EnumeratorEndException & )
        {
            return object;
        }
    }

    tV Last() const
    {
        return Last( []( tV )
        {
            return true;
        } );
    }

    tV LastOrDefault( std::function<bool( tV )> predicate ) const
    {
        try
        {
            return Last( predicate );
        }
        catch ( EnumeratorEndException & )
        {
            return tV();
        }
    }

    tV LastOrDefault() const
    {
        return LastOrDefault( []( tV )
        {
            return true;
        } );
    }

    template<typename tT2>
    QueryObj<Enumerator<tV, std::pair<bool, std::pair<tT, tT2> > > > Concat( QueryObj<tT2> rhs ) const
    {
        typedef std::pair<bool, std::pair<tT, tT2> > DataType;

        return Enumerator<tV, DataType>( [ = ]( DataType & pair ) -> tV
        {
            if ( pair.first )
            {
                return pair.second.second.Next();
            }

            try
            {
                return pair.second.first.Next();
            }
            catch ( EnumeratorEndException & )
            {
                pair.first = true;
                return pair.second.second.Next();
            }

        }, { false, { enumerator, rhs.enumerator } } );
    }

    std::vector<tV> ToVector() const
    {
        return ExportToContainer<std::vector<tV> >( []( std::vector<tV> &container, const tV & value )
        {
            container.push_back( value );
        } );
    }

    std::list<tV> ToList() const
    {
        return ExportToContainer<std::list<tV> >( []( std::list<tV> &container, const tV & value )
        {
            container.push_back( value );
        } );
    }

    std::deque<tV> ToDeque() const
    {
        return ExportToContainer<std::deque<tV> >( []( std::deque<tV> &container, const tV & value )
        {
            container.push_back( value );
        } );
    }

    std::set<tV> ToSet() const
    {
        return ExportToContainer<std::set<tV> >( []( std::set<tV> &container, const tV & value )
        {
            container.insert( value );
        } );
    }

    QueryObj< ByteEnum > Bytes( BytesDirection direction = BytesDirection::FirstToLast ) const
    {
        typedef std::pair<size_t, std::pair<tT, tV> > DataType;

        auto pair = std::make_pair( enumerator, tV() );
        pair.second = pair.first.Next();

        if ( direction == BytesDirection::FirstToLast )
        {
            return Enumerator<size_t, DataType>( [ = ]( DataType & p )->S32
            {
                if ( p.first == sizeof( tV ) )
                {
                    p.first = 0;
                    p.second.second = p.second.first.Next();
                }

                U8 *ptr = reinterpret_cast<U8 *>( &p.second.second );
                S32 value = ptr[p.first];
                ++p.first;

                return value;
            }, { 0, pair } );
        }

        return Enumerator<size_t, DataType>( [ = ]( DataType & p )->S32
        {
            if ( p.first == -1 )
            {
                p.first = sizeof( tV ) - 1;
                p.second.second = p.second.first.Next();
            }

            U8 *ptr = reinterpret_cast<U8 *>( &p.second.second );
            S32 value = ptr[p.first];
            --p.first;

            return value;
        }, { sizeof( tV ) - 1, pair } );
    }

    template<typename tR>
    QueryObj<Enumerator<tR, tT> > Unbytes( BytesDirection direction = BytesDirection::FirstToLast ) const
    {
        std::function< tR( tT & )> pred = [ = ]( tT & en )->tR
        {
            tR object;
            U8 *ptr = reinterpret_cast<U8 *>( &object );

            for ( size_t i = 0; i != sizeof( tR ); ++i )
            {
                ptr[i] = en.Next();
            }

            return object;
        };

        if ( direction != BytesDirection::FirstToLast )
        {
            pred = [ = ]( tT & en )->tR
            {
                tR object;
                U8 *ptr = reinterpret_cast<U8 *>( &object );

                for ( size_t i = sizeof( tR ) - 1; i != -1; --i )
                {
                    ptr[i] = en.Next();
                }

                return object;
            };
        }

        return Enumerator<tR, tT>( pred, enumerator );
    }

    QueryObj< BitEnum > Bits( BitsDirection direction = BitsDirection::HighToLow,
                              BytesDirection bytesDirection = BytesDirection::FirstToLast ) const
    {
        typedef std::pair<size_t, std::pair<QueryObj<Enumerator<size_t, std::pair<size_t, std::pair<tT, tV> > > >, U8> > DataType;

        auto inner = Bytes( bytesDirection );

        if ( direction == BitsDirection::LowToHigh )
        {
            return Enumerator<size_t, DataType>( [ = ]( DataType & pair )->S32
            {
                if ( pair.first == CHAR_BIT )
                {
                    pair.first = 0;
                    pair.second.second = static_cast<U8>( pair.second.first.Next() );
                }
                S32 value = 1 & ( pair.second.second >> ( pair.first % CHAR_BIT ) );
                ++pair.first;

                return value;
            }, { 0, { inner, static_cast<U8>( inner.Next() ) } } );
        }

        return Enumerator<size_t, DataType>( [ = ]( DataType & pair )->S32
        {
            if ( pair.first == -1 )
            {
                pair.first = CHAR_BIT - 1;
                pair.second.second = static_cast<U8>( pair.second.first.Next() );
            }
            S32 value = 1 & ( pair.second.second >> ( pair.first % CHAR_BIT ) );
            --pair.first;

            return value;
        }, { CHAR_BIT - 1, { inner, static_cast<U8>( inner.Next() ) } } );
    }

    QueryObj<Enumerator< U8, tT> > Unbits( BitsDirection direction = BitsDirection::HighToLow ) const
    {
        if ( direction == BitsDirection::LowToHigh )
        {
            return Enumerator< U8, tT>( [ = ]( tT & en ) -> U8
            {
                U8 object = 0;

                for ( size_t i = 0; i != CHAR_BIT; ++i )
                {
                    object |= ( en.Next() << i );
                }
                return object;
            }, enumerator );
        }

        return Enumerator< U8, tT>( [ = ]( tT & en ) -> U8
        {
            U8 object = 0;

            for ( size_t i = CHAR_BIT - 1; i != -1; --i )
            {
                object |= ( en.Next() << i );
            }
            return object;
        }, enumerator );
    }

    template<typename tR>
    QueryObj<Enumerator<tR, Enumerator<U8, tT> > > Unbits( BitsDirection direction = BitsDirection::HighToLow,
                                                           BytesDirection bytesDirection = BytesDirection::FirstToLast ) const
    {
        auto unbits = Unbits( direction );
        return unbits.template Unbytes<tR>( bytesDirection );
    }

private:

    template<typename tC, typename tFunc>
    tC ExportToContainer( tFunc func ) const
    {
        tC container;

        try
        {
            auto en = enumerator;

            for ( ;; )
            {
                func( container, en.Next() );
            }
        }
        catch ( EnumeratorEndException & ) {}

        return container;
    }

};

template<typename tT, typename tV>
QueryObj<Enumerator<tT, tV> > Query( tV begin, tV end )
{
    return Enumerator<tT, tV>( [ = ]( tV & iter )
    {
        return ( iter == end ) ? throw EnumeratorEndException() : *( iter++ );
    }, begin );
}

template<typename tT, typename tV>
QueryObj<Enumerator<tT, std::pair<tV, size_t> > > Query( tV begin, size_t length )
{
    return Enumerator<tT, std::pair<tV, size_t> >( [ = ]( std::pair<tV, size_t> &pair )
    {
        return ( pair.second++ == length ) ? throw EnumeratorEndException() : *( pair.first++ );
    }, std::make_pair( begin, 0 ) );
}

template<typename tT, size_t N>
auto Query( tT( &array )[N] )
-> decltype( Query<tT>( array, array + N ) )
{
    return Query<tT>( array, array + N );
}

template<typename tT, size_t N>
auto Query( const tT( &array )[N] )
-> decltype( Query<tT>( array, array + N ) )
{
    return Query<tT>( array, array + N );
}

template<template<class> class TV, typename TT>
auto Query( const TV<TT> &container )
-> decltype( Query<TT>( std::begin( container ), std::end( container ) ) )
{
    return Query<TT>( std::begin( container ), std::end( container ) );
}

template<template<class, class> class TV, typename TT, typename TU>
auto Query( const TV<TT, TU> &container )
-> decltype( Query<TT>( std::begin( container ), std::end( container ) ) )
{
    return Query<TT>( std::begin( container ), std::end( container ) );
}

template<template<class, class, class> class TV, typename TT, typename TS, typename TU>
auto Query( const TV<TT, TS, TU> &container )
-> decltype( Query<TT>( std::begin( container ), std::end( container ) ) )
{
    return Query<TT>( std::begin( container ), std::end( container ) );
}

template<template<class, class, class, class> class TV, typename TK, typename TT, typename TS, typename TU>
auto Query( const TV<TK, TT, TS, TU> &container )
-> decltype( Query<std::pair<TK, TT> >( std::begin( container ), std::end( container ) ) )
{
    return Query<std::pair<TK, TT> >( std::begin( container ), std::end( container ) );
}


template<template<class, size_t> class TV, typename TT, size_t TL>
auto Query( const TV<TT, TL> &container ) -> decltype( Query<TT>( std::begin( container ), std::end( container ) ) )
{
    return Query<TT>( std::begin( container ), std::end( container ) );
}

template<typename tT>
QueryObj<Enumerator<tT, size_t> > Repeat( tT value, size_t count )
{
    return Enumerator<tT, size_t>( [ = ]( size_t &index )
    {
        return index++ >= count ? throw EnumeratorEndException() : value;
    }, 0 );
}

template<typename tT>
QueryObj<Enumerator<tT, std::pair<bool, tT> > > Range( tT begin, tT end, tT step )
{
    return Enumerator<tT, std::pair<bool, tT> >( [ = ]( std::pair<bool, tT> &pair )
    {
        if ( !( pair.second < end ) )
        {
            throw EnumeratorEndException();
        }

        if ( !pair.first )
        {
            pair.first = true;
        }
        else
        {
            pair.second += step;
        }

        return pair.second;
    }, { false, begin } );
}

#endif