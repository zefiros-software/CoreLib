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
#ifndef __ENGINE_PTRHASHMAP_H__
#define __ENGINE_PTRHASHMAP_H__

#include "container/denseBitset.h"

#include "math/scalar/mathf.h"

template< typename tPtr, typename tValue >
class FastPtrHashMap
{
public:

    struct Node
    {
        tValue element;
        tPtr *ptr;
    };

    FastPtrHashMap() noexcept
        : mItemCount( 0 ),
          mSize( 0 )
    {
        Resize( 1024 );
    }

    explicit FastPtrHashMap( size_t size ) noexcept
        : mItemCount( 0 ),
          mSize( 0 )
    {
        Resize( size );
    }

    ~FastPtrHashMap() noexcept
    {
        Clear();
    }

    const std::vector< Node > &Data() const noexcept
    {
        return mNodes;
    }

    std::vector< Node > &Data() noexcept
    {
        return mNodes;
    }

    void Clear() noexcept
    {
        mFilled.Reset();
        mNodes.clear();
        mItemCount = 0;
    }

    size_t Resize( size_t newSize ) noexcept
    {
        const size_t nextPow2 = Mathf::IsPow2( newSize ) ? newSize : static_cast< size_t >( Mathf::NextPow2( newSize ) );

        if ( nextPow2 > mSize )
        {
            std::vector< Node > temp = std::move( mNodes );

            mNodes = std::vector< Node >();
            mNodes.reserve( nextPow2 );

            std::fill( mTable.begin(), mTable.end(), 0x00 );
            mTable.resize( nextPow2 );

            mFilled.Reset();
            mFilled.Resize( nextPow2 );
            mSize = nextPow2;

            // temporarily so we can insert the items
            mItemCount = 0;

            for ( Node &node : temp )
            {
                Insert( node.ptr, node.element );
            }
        }

        return mSize;
    }

    void Insert( tPtr *ptr, tValue element ) noexcept
    {
        size_t hash = FirstHash( ptr );
        const size_t step = StepHash( ptr );

        while ( mFilled[hash] )
        {
            const Node &node = mNodes[mTable[ hash ]];

            if ( node.ptr == ptr )
            {
                return;
            }

            hash += step;
            hash &= ( mSize - 1 );
        }

        mFilled[hash] = true;
        mTable[hash] = mItemCount;

        Node node = {element, ptr };
        mNodes.emplace_back( node );

        if ( ++mItemCount > mSize * 0.7 )
        {
            Resize( mSize << 1 );
        }
    }

    bool Find( tPtr *ptr, tValue *&value ) noexcept
    {
        size_t hash = FirstHash( ptr );
        const size_t step = StepHash( ptr );

        while ( mFilled[hash] )
        {
            Node &node = mNodes[mTable[ hash ]];

            if ( node.ptr == ptr )
            {
                value = &node.element;
                return true;
            }

            hash += step;
            hash &= ( mSize - 1 );
        }

        value = nullptr;
        return false;
    }

    bool Has( tPtr *ptr ) const noexcept
    {
        size_t hash = FirstHash( ptr );
        const size_t step = StepHash( ptr );

        while ( mFilled[hash] )
        {
            const Node &node = mNodes[mTable[ hash ]];

            if ( node.ptr == ptr )
            {
                return true;
            }

            hash += step;
            hash &= ( mSize - 1 );
        }

        return false;
    }


private:

    std::vector< size_t > mTable;
    std::vector< Node > mNodes;
    DenseBitset mFilled;
    size_t mItemCount;
    size_t mSize;

    size_t FirstHash( tPtr *ptr ) const noexcept
    {
        return ( reinterpret_cast< size_t >( ptr ) * 2654435761 ) & ( mSize - 1 );
    }

    size_t SecondHash( tPtr *ptr ) const noexcept
    {
        return ( reinterpret_cast< size_t >( ptr ) * 33 ) & ( mSize - 1 );
    }

    size_t ThirdHash( tPtr *ptr ) const noexcept
    {
        ptrdiff_t x = reinterpret_cast<ptrdiff_t>( ptr );
        x = ( ( x >> 16 ) ^ x ) * 0x45d9f3b;
        x = ( ( x >> 16 ) ^ x ) * 0x45d9f3b;
        x = ( ( x >> 16 ) ^ x );
        return x & ( mSize - 1 );
    }

    size_t StepHash( tPtr *ptr )const noexcept
    {
        return ( SecondHash( ptr ) * ThirdHash( ptr ) ) | 0x01;
    }
};

#endif