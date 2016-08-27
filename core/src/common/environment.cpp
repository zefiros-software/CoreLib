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

#include "common/environment.h"

#include "preproc/env.h"

#include <stdlib.h>

#if OS_IS_WINDOWS
#   include <windows.h>
#endif

namespace Environment
{

    std::string GetVariable( const std::string &var )
    {
#if OS_IS_WINDOWS
        char *buf = nullptr;
        size_t sz = 0;
        std::string result;

        if ( _dupenv_s( &buf, &sz, var.c_str() ) == 0 )
        {
            if ( buf )
            {
                result = buf;
                free( buf );
            }
        }

        return result;
#else
        return getenv( var.c_str() );
#endif
    }

    bool SetVariable( const std::string &var, const std::string &value )
    {
#if OS_IS_WINDOWS
        return _putenv( ( var + "=" + value ).c_str() ) == 0;
#else
        return setenv( ar.c_str(), value.c_str(), 1 ) == 0;
#endif
    }

    bool HasVariable( const std::string &var )
    {
#if OS_IS_WINDOWS
        char *buf = nullptr;
        size_t sz = 0;
        _dupenv_s( &buf, &sz, var.c_str() ) ;
        bool result = buf != nullptr;
        free( buf );

        return result;
#else
        return getenv( var.c_str() ) != nullptr;
#endif
    }

    bool RemoveVariable( const std::string &var )
    {
        return Environment::SetVariable( var, "" );
    }

}