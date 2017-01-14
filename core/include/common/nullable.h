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

#pragma once
#ifndef __ENGINE_NULLABLE_H__
#define __ENGINE_NULLABLE_H__

#include <cassert>

template< typename tT >
class Nullable
{
public:

    Nullable() noexcept
        : mValue(),
          mIsSet( false )
    {
    }

    Nullable( const tT &value ) noexcept
        : mValue( value ),
          mIsSet( true )
    {
    }

    Nullable( const Nullable &other ) noexcept
        : mValue( other.mValue ),
          mIsSet( other.mIsSet )
    {
    }

    ~Nullable()
    {
    }

    Nullable &operator=( const tT &value ) noexcept
    {
        return Assign( value );
    }

    Nullable &operator=( const Nullable &other ) noexcept
    {
        return Assign( other );
    }

    bool operator==( const Nullable<tT> &other ) const noexcept
    {
        return ( !mIsSet && !other.mIsSet ) || ( !mIsSet == !other.mIsSet && mValue == other.mValue );
    }

    bool operator!=( const Nullable<tT> &other ) const noexcept
    {
        return !( *this == other );
    }

    bool operator>( const Nullable<tT> &other ) const noexcept
    {
        return *this != other  && *this >= other;
    }

    bool operator<( const Nullable<tT> &other ) const noexcept
    {
        if ( mIsSet && other.mIsSet )
        {
            return mValue < other.mValue;
        }

        if ( !mIsSet && !other.mIsSet )
        {
            return false;
        }

        if ( !mIsSet )
        {
            return true;
        }

        return false;
    }

    operator tT &() noexcept
    {
        return Get();
    }

    operator const tT &() const noexcept
    {
        return Get();
    }

    Nullable &Assign( const tT &value ) noexcept
    {
        mValue = value;
        mIsSet = true;
        return *this;
    }

    Nullable &Assign( const Nullable &other ) noexcept
    {
        Nullable tmp( other );
        mValue = other.mValue;
        mIsSet = other.mIsSet;
        return *this;
    }

    Nullable &SetNull() noexcept
    {
        mIsSet = false;
        return *this;
    }

    bool IsNull() const noexcept
    {
        return !mIsSet;
    }

    tT &Get() noexcept
    {
        assert( mIsSet );
        return mValue;
    }

    const tT &Get() const noexcept
    {
        assert( mIsSet );
        return mValue;
    }

    const tT &Get( const tT &deflt ) const noexcept
    {
        return !mIsSet ? deflt : mValue;
    }


    void Clear() noexcept
    {
        mIsSet = false;
    }

private:

    tT mValue;
    bool mIsSet;
};

#endif