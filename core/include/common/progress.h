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

#pragma once
#ifndef __PROGRESS_H__
#define __PROGRESS_H__

#include "common/format.h"
#include "common/string.h"
#include "common/range.h"

#include "preproc/os.h"

#include "math/scalar/mathf.h"
#include "date/date.h"

#include <iostream>
#include <chrono>

#if OS_IS_WINDOWS
#   include <windows.h>
#   include <io.h>
#   include <fcntl.h>
#else
#   include <sys/ioctl.h>
#   include <stdio.h>
#   include <unistd.h>
#endif

extern size_t gActiveProgressBars;

// based on https://github.com/tqdm/tqdm/blob/master/tqdm/_tqdm.py
template< typename tT >
class ProgressBar
{
public:

    template< typename tI >
    class iterator
    {
    public:

        explicit iterator(ProgressBar< tT > val, tI it)
            : mVal(val),
              mIt(it)
        {
        }

        iterator &operator++()
        {
            ++mIt;

            mVal.Increment();

            return *this;
        }

        bool operator== (const iterator &rhs)
        {
            return mIt == rhs.mIt;
        }

        bool operator!= (const iterator &rhs)
        {
            return !this->operator==(rhs);
        }

        auto operator*()
        {
            return *mIt;
        }

    private:

        ProgressBar< tT > mVal;
        tI mIt;
    };

    explicit ProgressBar(tT val)
        : mNow(std::chrono::system_clock::now()),
          mSize(std::distance(std::begin(val), std::end(val))),
          mIts(0),
          mCopied(false),
          mVal(val)
    {
        mBarIndex = gActiveProgressBars++;

        if (mBarIndex > 0)
        {
            std::cout << "\n";
        }
    }

    ProgressBar(ProgressBar &other) noexcept
        : mNow(other.mNow),
          mBarIndex(other.mBarIndex),
          mSize(other.mSize),
          mIts(other.mIts),
          mCopied(true),
          mVal(other.mVal)
    {
    }

    ProgressBar(ProgressBar &&other) noexcept
        : mNow(other.mNow),
          mBarIndex(other.mBarIndex),
          mSize(other.mSize),
          mIts(other.mIts),
          mCopied(other.mCopied),
          mVal(other.mVal)
    {
    }

    ~ProgressBar()
    {
        if (!mCopied)
        {
            if (--gActiveProgressBars > 0)
            {
                std::cout << "\r" << String::Repeat(" ", GetColumns() - 1) << "\r";
#if OS_IS_WINDOWS

                HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE);
                CONSOLE_SCREEN_BUFFER_INFO csbi;
                GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
                COORD position = { 0, csbi.dwCursorPosition.Y - 1 };
                SetConsoleCursorPosition(hStdout, position);
#else

                std::cout << "\033[A\r\33[2K";
#endif
            }

        }

    }

    void Increment()
    {
        ++mIts;
        std::cout << "\r";
        std::chrono::time_point< std::chrono::system_clock > now = std::chrono::system_clock::now();
        std::chrono::seconds diff = std::chrono::duration_cast<std::chrono::seconds>(now - mNow);
        FormatMeter(mIts, diff, GetColumns() - 1);
    }

    auto begin()
    {
        return iterator<decltype(std::begin(mVal))>(*this, std::begin(mVal));
    }

    auto end()
    {
        return iterator<decltype(std::begin(mVal))>(*this, std::end(mVal));
    }

private:

    std::chrono::time_point< std::chrono::system_clock > mNow;
    std::string mPrefix;
    size_t mBarIndex;
    size_t mSize;
    size_t mIts;
    bool mCopied;
    tT mVal;

    void FormatMeter(size_t n, std::chrono::seconds duration, ptrdiff_t cols)
    {
        F32 f = (F32)n / mSize;
        F32 rate = (F32)n / duration.count();
        F32 irate = 1 / rate;
        std::string rateStr = String::Place("{0:5.2f}s/it", irate);

        std::string lbar = String::Place("{0}{1:3.0f}%|", mPrefix, f * 100);
        std::string rbar = String::Place("| {0}/{1} [{2}<{3}, {4}]", FormatUnit(n), FormatUnit(mSize),
                                         FormatTime(duration),
                                         FormatTime(std::chrono::seconds((size_t)((mSize - n) / rate))), rateStr);
        ptrdiff_t nBars = Mathf::GetMax< ptrdiff_t >(1, cols - lbar.size() - rbar.size());

        ptrdiff_t length = (ptrdiff_t)(f * nBars);

        std::wstring bar = String::Repeat(L"█", length) + String::Repeat(L" ", Mathf::GetMax< ptrdiff_t >(nBars - length, 0));

        std::cout << lbar;

#if OS_IS_WINDOWS
        fflush(stdout);
        S32 prev = _setmode(_fileno(stdout), _O_WTEXT);
#endif

        std::wcout << bar;

#if OS_IS_WINDOWS
        fflush(stdout);
        _setmode(_fileno(stdout), prev);
#endif
        std::cout << rbar;
    }

    template< typename tV >
    static std::string FormatUnit(tV n, const std::string &suffix = "")
    {
        auto units = { "", "K", "M", "G", "T", "P", "E", "Z" };
        F64 v = (F64)n;

        for (auto unit : units)
        {
            if (Mathf::Abs(v) < 999.95)
            {
                if (Mathf::Abs(v) < 99.95)
                {
                    if (Mathf::Abs(v) < 9.995)
                    {
                        return String::Place("{0:1.0f}{1}{2}", v, unit, suffix);
                    }

                    return String::Place("{0:2.0f}{1}{2}", v, unit, suffix);
                }

                return String::Place("{0:3.0f}{1}{2}", v, unit, suffix);
            }

            v /= 1000.0;
        }

        return String::Place("{0:3.0f}Y{1}", v, suffix);
    }

    static std::string FormatTime(std::chrono::seconds duration)
    {
        auto time = date::make_time(duration);
        auto hours = time.hours().count();
        auto minutes = time.minutes().count();
        auto seconds = time.seconds().count();

        if (hours != 0)
        {
            return String::Place("{0:d}:{1:02d}:{2:02d}", hours, minutes, seconds);
        }

        return String::Place("{0:02d}:{1:02d}", minutes, seconds);
    }

    static size_t GetColumns()
    {
#if OS_IS_WINDOWS
        CONSOLE_SCREEN_BUFFER_INFO csbi;
        GetConsoleScreenBufferInfo(GetStdHandle(STD_OUTPUT_HANDLE), &csbi);
        return Mathf::GetMax<size_t>(80u, Mathf::GetMin< size_t >(1024u, csbi.srWindow.Right - csbi.srWindow.Left + 1));
#else
        struct winsize w;
        ioctl(0, TIOCGWINSZ, &w);
        return Mathf::GetMax<size_t>(80u, Mathf::GetMin< size_t >(1024u, w.ws_col));
#endif
    }
};


template< typename tT, typename tR = tT >
constexpr ProgressBar< std::vector< tR >> Progress(std::initializer_list< tT > val)
{
    return ProgressBar< std::vector< tR >>(std::vector< tT >(val.begin(), val.end()));
}

ProgressBar< std::vector< int32_t >> Progress(int32_t val);

template< typename tT>
ProgressBar< tT > Progress(tT val)
{
    return ProgressBar< tT >(val);
}

#endif
