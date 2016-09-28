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

#include "container/denseBitset.h"


const DenseBitset::BitReference &DenseBitset::BitReference::operator=( bool val ) const noexcept
{
    if ( val )
    {
        mInt |= mMask;
    }
    else
    {
        mInt &= ~mMask;
    }

    return *this;
}

const DenseBitset::BitReference &DenseBitset::BitReference::operator=( const BitReference &br ) const noexcept
{
    return this->operator =( bool( br ) );
}

DenseBitset::BitReference::operator bool() const
{
    return ( ( mInt & mMask ) != 0 ) ? true : false;
}

DenseBitset::BitReference::BitReference( U64 &integer, U64 mask ) noexcept
    : mInt( integer ),
      mMask( mask )
{

}

DenseBitset::DenseBitset( size_t size /*= 0 */ ) noexcept
{
    Resize( size );
}

void DenseBitset::Reset() noexcept
{
    std::fill( mBits.begin(), mBits.end(), 0x00 );
}

void DenseBitset::Resize( size_t size ) noexcept
{
    mBits.resize( ( size >> 6 ) + 1 );
}

bool DenseBitset::operator[]( size_t bit ) const noexcept
{
    return ( ( mBits[( bit >> 6 )] ) & ( static_cast< U64 >( 1 ) << ( bit & 63 ) ) ) != 0;
}

DenseBitset::BitReference DenseBitset::operator[]( size_t bit ) noexcept
{
    return BitReference( mBits[bit >> 6], static_cast< U64 >( 1 ) << ( bit & 63 ) );
}