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

#include "manager/systemManager.h"
#include "manager/logManager.h"

namespace Console
{

    void SetMode( LogMode mode )
    {
        SystemManager::Get()->GetManagers()->log->SetMode( mode );
    }

    Console::LogMode GetMode()
    {
        return SystemManager::Get()->GetManagers()->log->GetMode();
    }

    void Printf( const std::string &fmt )
    {
        SystemManager::Get()->GetManagers()->log->Printf( fmt );
    }

    void Printf( const std::string &fmt,
                 const boost::any &v1 )
    {
        SystemManager::Get()->GetManagers()->log->Printf( fmt, v1 );
    }

    void Printf( const std::string &fmt,
                 const boost::any &v1,
                 const boost::any &v2 )
    {
        SystemManager::Get()->GetManagers()->log->Printf( fmt, v1, v2 );
    }

    void Printf( const std::string &fmt,
                 const boost::any &v1,
                 const boost::any &v2,
                 const boost::any &v3 )
    {
        SystemManager::Get()->GetManagers()->log->Printf( fmt, v1, v2, v3 );
    }

    void Printf( const std::string &fmt,
                 const boost::any &v1,
                 const boost::any &v2,
                 const boost::any &v3,
                 const boost::any &v4 )
    {
        SystemManager::Get()->GetManagers()->log->Printf( fmt, v1, v2, v3, v4 );
    }

    void Printf( const std::string &fmt,
                 const boost::any &v1,
                 const boost::any &v2,
                 const boost::any &v3,
                 const boost::any &v4,
                 const boost::any &v5 )
    {
        SystemManager::Get()->GetManagers()->log->Printf( fmt, v1, v2, v3, v4, v5 );
    }

    void Printf( const std::string &fmt,
                 const boost::any &v1,
                 const boost::any &v2,
                 const boost::any &v3,
                 const boost::any &v4,
                 const boost::any &v5,
                 const boost::any &v6 )
    {
        SystemManager::Get()->GetManagers()->log->Printf( fmt, v1, v2, v3, v4, v5, v6 );
    }

    void Printf( const std::string &fmt,
                 const boost::any &v1,
                 const boost::any &v2,
                 const boost::any &v3,
                 const boost::any &v4,
                 const boost::any &v5,
                 const boost::any &v6,
                 const boost::any &v7 )
    {
        SystemManager::Get()->GetManagers()->log->Printf( fmt, v1, v2, v3, v4, v5, v6, v7 );
    }

    void Printf( const std::string &fmt,
                 const boost::any &v1,
                 const boost::any &v2,
                 const boost::any &v3,
                 const boost::any &v4,
                 const boost::any &v5,
                 const boost::any &v6,
                 const boost::any &v7,
                 const boost::any &v8 )
    {
        SystemManager::Get()->GetManagers()->log->Printf( fmt, v1, v2, v3, v4, v5, v6, v7, v8 );
    }

    void Errorf( const std::string &fmt )
    {
        SystemManager::Get()->GetManagers()->log->Errorf( fmt );
    }

    void Errorf( const std::string &fmt,
                 const boost::any &v1 )
    {
        SystemManager::Get()->GetManagers()->log->Errorf( fmt, v1 );
    }

    void Errorf( const std::string &fmt,
                 const boost::any &v1,
                 const boost::any &v2 )
    {
        SystemManager::Get()->GetManagers()->log->Errorf( fmt, v1, v2 );
    }

    void Errorf( const std::string &fmt,
                 const boost::any &v1,
                 const boost::any &v2,
                 const boost::any &v3 )
    {
        SystemManager::Get()->GetManagers()->log->Errorf( fmt, v1, v2, v3 );
    }

    void Errorf( const std::string &fmt,
                 const boost::any &v1,
                 const boost::any &v2,
                 const boost::any &v3,
                 const boost::any &v4 )
    {
        SystemManager::Get()->GetManagers()->log->Errorf( fmt, v1, v2, v3, v4 );
    }

    void Errorf( const std::string &fmt,
                 const boost::any &v1,
                 const boost::any &v2,
                 const boost::any &v3,
                 const boost::any &v4,
                 const boost::any &v5 )
    {
        SystemManager::Get()->GetManagers()->log->Errorf( fmt, v1, v2, v3, v4, v5 );
    }

    void Errorf( const std::string &fmt,
                 const boost::any &v1,
                 const boost::any &v2,
                 const boost::any &v3,
                 const boost::any &v4,
                 const boost::any &v5,
                 const boost::any &v6 )
    {
        SystemManager::Get()->GetManagers()->log->Errorf( fmt, v1, v2, v3, v4, v5, v6 );
    }

    void Errorf( const std::string &fmt,
                 const boost::any &v1,
                 const boost::any &v2,
                 const boost::any &v3,
                 const boost::any &v4,
                 const boost::any &v5,
                 const boost::any &v6,
                 const boost::any &v7 )
    {
        SystemManager::Get()->GetManagers()->log->Errorf( fmt, v1, v2, v3, v4, v5, v6, v7 );
    }

    void Errorf( const std::string &fmt,
                 const boost::any &v1,
                 const boost::any &v2,
                 const boost::any &v3,
                 const boost::any &v4,
                 const boost::any &v5,
                 const boost::any &v6,
                 const boost::any &v7,
                 const boost::any &v8 )
    {
        SystemManager::Get()->GetManagers()->log->Errorf( fmt, v1, v2, v3, v4, v5, v6, v7, v8 );
    }

    void Warningf( const std::string &fmt )
    {
        SystemManager::Get()->GetManagers()->log->Warningf( fmt );
    }

    void Warningf( const std::string &fmt,
                   const boost::any &v1 )
    {
        SystemManager::Get()->GetManagers()->log->Warningf( fmt, v1 );
    }

    void Warningf( const std::string &fmt,
                   const boost::any &v1,
                   const boost::any &v2 )
    {
        SystemManager::Get()->GetManagers()->log->Warningf( fmt, v1, v2 );
    }

    void Warningf( const std::string &fmt,
                   const boost::any &v1,
                   const boost::any &v2,
                   const boost::any &v3 )
    {
        SystemManager::Get()->GetManagers()->log->Warningf( fmt, v1, v2, v3 );
    }

    void Warningf( const std::string &fmt,
                   const boost::any &v1,
                   const boost::any &v2,
                   const boost::any &v3,
                   const boost::any &v4 )
    {
        SystemManager::Get()->GetManagers()->log->Warningf( fmt, v1, v2, v3, v4 );
    }

    void Warningf( const std::string &fmt,
                   const boost::any &v1,
                   const boost::any &v2,
                   const boost::any &v3,
                   const boost::any &v4,
                   const boost::any &v5 )
    {
        SystemManager::Get()->GetManagers()->log->Warningf( fmt, v1, v2, v3, v4, v5 );
    }

    void Warningf( const std::string &fmt,
                   const boost::any &v1,
                   const boost::any &v2,
                   const boost::any &v3,
                   const boost::any &v4,
                   const boost::any &v5,
                   const boost::any &v6 )
    {
        SystemManager::Get()->GetManagers()->log->Warningf( fmt, v1, v2, v3, v4, v5, v6 );
    }

    void Warningf( const std::string &fmt,
                   const boost::any &v1,
                   const boost::any &v2,
                   const boost::any &v3,
                   const boost::any &v4,
                   const boost::any &v5,
                   const boost::any &v6,
                   const boost::any &v7 )
    {
        SystemManager::Get()->GetManagers()->log->Warningf( fmt, v1, v2, v3, v4, v5, v6, v7 );
    }

    void Warningf( const std::string &fmt,
                   const boost::any &v1,
                   const boost::any &v2,
                   const boost::any &v3,
                   const boost::any &v4,
                   const boost::any &v5,
                   const boost::any &v6,
                   const boost::any &v7,
                   const boost::any &v8 )
    {
        SystemManager::Get()->GetManagers()->log->Warningf( fmt, v1, v2, v3, v4, v5, v6, v7, v8 );
    }

    void Initf( const std::string &fmt )
    {
        SystemManager::Get()->GetManagers()->log->Initf( fmt );
    }

    void Initf( const std::string &fmt,
                const boost::any &v1 )
    {
        SystemManager::Get()->GetManagers()->log->Initf( fmt, v1 );
    }

    void Initf( const std::string &fmt,
                const boost::any &v1,
                const boost::any &v2 )
    {
        SystemManager::Get()->GetManagers()->log->Initf( fmt, v1, v2 );
    }

    void Initf( const std::string &fmt,
                const boost::any &v1,
                const boost::any &v2,
                const boost::any &v3 )
    {
        SystemManager::Get()->GetManagers()->log->Initf( fmt, v1, v2, v3 );
    }

    void Initf( const std::string &fmt,
                const boost::any &v1,
                const boost::any &v2,
                const boost::any &v3,
                const boost::any &v4 )
    {
        SystemManager::Get()->GetManagers()->log->Initf( fmt, v1, v2, v3, v4 );
    }

    void Initf( const std::string &fmt,
                const boost::any &v1,
                const boost::any &v2,
                const boost::any &v3,
                const boost::any &v4,
                const boost::any &v5 )
    {
        SystemManager::Get()->GetManagers()->log->Initf( fmt, v1, v2, v3, v4, v5 );
    }

    void Initf( const std::string &fmt,
                const boost::any &v1,
                const boost::any &v2,
                const boost::any &v3,
                const boost::any &v4,
                const boost::any &v5,
                const boost::any &v6 )
    {
        SystemManager::Get()->GetManagers()->log->Initf( fmt, v1, v2, v3, v4, v5, v6 );
    }

    void Initf( const std::string &fmt,
                const boost::any &v1,
                const boost::any &v2,
                const boost::any &v3,
                const boost::any &v4,
                const boost::any &v5,
                const boost::any &v6,
                const boost::any &v7 )
    {
        SystemManager::Get()->GetManagers()->log->Initf( fmt, v1, v2, v3, v4, v5, v6, v7 );
    }

    void Initf( const std::string &fmt,
                const boost::any &v1,
                const boost::any &v2,
                const boost::any &v3,
                const boost::any &v4,
                const boost::any &v5,
                const boost::any &v6,
                const boost::any &v7,
                const boost::any &v8 )
    {
        SystemManager::Get()->GetManagers()->log->Initf( fmt, v1, v2, v3, v4, v5, v6, v7, v8 );
    }


    void PrintTitle( const std::string &title, const U8 headerLength /* = 70 */ )
    {
        const size_t titleLength = title.length();
        // Find the start of the header output
        const size_t midoffset = ( headerLength - titleLength ) / 2 ;
        std::stringstream ss;

        // Fill up the header
        for ( U8 i = 0; i < headerLength; ++i )
        {
            if ( i < midoffset || i > midoffset + titleLength )
            {
                ss << "-";
            }
            else
            {
                // We have come to the part where the title can be
                // inserted into the header, after that we skip to the
                // last part of the header.
                ss << title;
                i += static_cast< U8 >( titleLength );
            }
        }

        /// Output the header as an init statement
        Initf( ss.str() );
    }

}