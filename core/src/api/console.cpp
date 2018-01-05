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

#include "api/console.h"

namespace Console
{
    void PrintTitle(const std::string &title, const U8 headerLength /* = 70 */)
    {
        const size_t titleLength = title.length();
        // Find the start of the header output
        const size_t midoffset = (headerLength - titleLength) / 2 ;
        std::stringstream ss;

        // Fill up the header
        for (U8 i = 0; i < headerLength; ++i)
        {
            if (i < midoffset || i > midoffset + titleLength)
            {
                ss << "-";
            }
            else
            {
                // We have come to the part where the title can be
                // inserted into the header, after that we skip to the
                // last part of the header.
                ss << title;
                i += static_cast< U8 >(titleLength);
            }
        }

        /// Output the header as an init statement
        Initf(ss.str());
    }

}