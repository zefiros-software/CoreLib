/**
 * @cond ___LICENSE___
 *
 * Copyright (c) 2015 Koen Visscher, Paul Visscher and individual contributors.
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
#ifndef __UNORDERED_CONTIGUOUS_MAP_H__
#define __UNORDERED_CONTIGUOUS_MAP_H__

#include <unordered_map>
#include <vector>

template< class Key, class Item >
class UnorderedContiguousMap
{
public:

    UnorderedContiguousMap()
        : mSize( 0 )
    {

    }

    bool Insert( const Key &key, const Item &item )
    {
        auto it = mSearchBlock.find( key );

        if ( it == mSearchBlock.end() )
        {
            mContiguousBlock.push_back( item );
            mSearchBlock.insert( std::make_pair( key, mSize++ ) );

            return true;
        }

        return false;
    }

    bool Delete( const Key &key )
    {
        auto it = mSearchBlock.find( key );

        if ( it != mSearchBlock.end() )
        {
            size_t i = it->second;

            mContiguousBlock[i] = mContiguousBlock[--mSize];

            mContiguousBlock.pop_back();

            mSearchBlock.erase( key );

            return true;
        }

        return false;
    }

    bool Has( const Key &key ) const
    {
        return mSearchBlock.find( key ) != mSearchBlock.end();
    }
    
    Item *Find( const Key &key )
    {
        auto it = mSearchBlock.find( key );
        
        if ( it != mSearchBlock.end() )
        {
            Item *rval = &mContiguousBlock[it->second];
            
            return rval;
        }
        
        return nullptr;
    }
    
    const Item *CFind( const Key &key ) const
    {
        auto it = mSearchBlock.find( key );
        
        if ( it != mSearchBlock.end() )
        {
            const Item *rval = &mContiguousBlock[it->second];
            
            return rval;
        }
        
        return nullptr;
    }
    
    void Clear()
    {
        mContiguousBlock.clear();
        mSearchBlock.clear();
    }

    size_t Size() const
    {
        return mSize;
    }

    const std::vector< Item > &GetValues() const
    {
        return mContiguousBlock;
    }

    std::vector< Item > &GetValues()
    {
        return mContiguousBlock;
    }

private:

    std::unordered_map< Key, size_t > mSearchBlock;
    std::vector< Item > mContiguousBlock;

    size_t mSize;
};


#endif