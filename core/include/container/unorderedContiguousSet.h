/**
 * @cond ___LICENSE___
 *
 * Copyright (c) 2016-2018 Zefiros Software.
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

#ifndef __UNORDERED_CONTIGUOUS_SET_H__
#define __UNORDERED_CONTIGUOUS_SET_H__

#include <unordered_map>
#include <vector>

template< class tItem >
class UnorderedContiguousSet
{
public:

    UnorderedContiguousSet() noexcept
        : mSize(0)
    {

    }

    bool Insert(const tItem item) noexcept
    {
        auto it = mSearchBlock.find(item);

        if (it == mSearchBlock.end())
        {
            mContiguousBlock.push_back(item);
            mSearchBlock.insert(std::make_pair(item, mSize++));

            return true;
        }

        return false;
    }

    bool Delete(const tItem item) noexcept
    {
        auto it = mSearchBlock.find(item);

        if (it != mSearchBlock.end())
        {
            size_t i = it->second;

            mContiguousBlock[i] = mContiguousBlock[--mSize];

            mContiguousBlock.pop_back();

            mSearchBlock.erase(item);

            return true;
        }

        return false;
    }

    bool Has(const tItem item) const noexcept
    {
        return mSearchBlock.find(item) != mSearchBlock.end();
    }

    void Clear() noexcept
    {
        mContiguousBlock.clear();
        mSearchBlock.clear();
        mSize = 0;
    }

    size_t Size() const noexcept
    {
        return mSize;
    }

    const std::vector< tItem > &GetValues() const noexcept
    {
        return mContiguousBlock;
    }

    std::vector< tItem > &GetValues() noexcept
    {
        return mContiguousBlock;
    }

private:

    std::unordered_map< tItem, size_t > mSearchBlock;
    std::vector< tItem > mContiguousBlock;

    size_t mSize;
};


#endif