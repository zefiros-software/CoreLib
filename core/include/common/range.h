/**
* @cond ___LICENSE___
*
* Copyright (c) 2017 Koen Visscher, Paul Visscher and individual contributors.
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
#ifndef __RANGE_H__
#define __RANGE_H__

#include <functional>
#include <numeric>
#include <vector>

template<typename tT, typename tFunc>
struct IotaWrapper
{
    tT value;
    tFunc incrFunction;

    IotaWrapper() = delete;
    IotaWrapper( const tT &n, const tFunc &incrFunction )
        : value( n ),
          incrFunction( incrFunction )
    {
    }

    operator tT()
    {
        return value;
    }

    IotaWrapper &operator++()
    {
        value = incrFunction( value );
        return *this;
    }
};

template<class tT>
std::vector<tT> Range( const tT &first, const tT &last )
{
    IotaWrapper<tT, std::function< tT( tT & ) > > iota( first, [ = ]( tT & n )
    {
        return n + 1;
    } );
    std::vector<tT> result( last - first );
    std::iota( result.begin(), result.end(), iota );
    return result;
}

template<class tT, class Incr>
std::vector<tT> Range( const tT &first, const tT &last, const tT &increment )
{
    IotaWrapper<tT, std::function< tT( tT & ) > > iota( first, [ = ]( tT & n )
    {
        return n + increment;
    } );
    std::vector<tT> result( ( last - first ) / increment );
    std::iota( result.begin(), result.end(), iota );
    return result;
}

#endif