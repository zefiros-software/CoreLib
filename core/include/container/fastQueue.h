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
#ifndef __ENGINE_FASTQUEUE_H__
#define __ENGINE_FASTQUEUE_H__

#include <vector>

template< typename tT >
class FastQueue
{
public:

    void Queue( tT item ) noexcept
    {
        mInbox.push_back( item );
    }

    void Dequeue() noexcept
    {
        Move();
        mOutbox.pop_back();
    }

    tT Front() noexcept
    {
        Move();
        return mOutbox.back();
    }

    bool IsEmpty() const noexcept
    {
        return mOutbox.empty() && mInbox.empty();
    }

private:

    std::vector<tT> mInbox;
    std::vector<tT> mOutbox;

    void Move() noexcept
    {
        if ( mOutbox.empty() )
        {
            while ( !mInbox.empty() )
            {
                mOutbox.push_back( mInbox.back() );
                mInbox.pop_back();
            }
        }
    }

};

#endif