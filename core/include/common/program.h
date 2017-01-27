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
#ifndef __ENGINE_GAMEINSTANCE_H__
#define __ENGINE_GAMEINSTANCE_H__

#include "common/types.h"

#include <boost/program_options.hpp>
#include <map>
#include <iostream>

/// @addtogroup docCommon
/// @{

/// @addtogroup docCommon_Game
/// @{

/**
 * A game instance.
 */
class Program
{
public:

    /// @name Construction
    /// @{

    /**
     * Constructor.
     *
     * @param           argc The command line argument count.
     * @param [in,out]  argv The arguments array.
     */

    Program( S32 argc, const char **argv ) noexcept;

    ~Program();

    /// @}

    /// @name Start & Run
    /// @{


    void Update();

    /**
     * Initialize the game instance by setting the working directory and handle command line arguments.
     */

    void Init();

    bool IsRunning() const noexcept;

    /// @}


    /// @name CLI
    /// @{

    void AddOption()
    {
        mDesc.add_options()
        ( "help,h", "Shows this help screen" );
    }

    /// @}

protected:

    /// @name Start & Run
    /// @{

    /**
     * Shuts down the application and frees any resources it is using.
     */

    static void Shutdown();

    /// @}

    bool ParseCLI()
    {
        try
        {
            mDesc.add_options()
            ( "help,h", "Shows this help screen" );

            boost::program_options::variables_map vm;
            boost::program_options::store( parse_command_line( mArgc, mArgv, mDesc ), vm );

            if ( vm.count( "help" ) )
            {
                std::cout << mDesc << '\n';
                mIsHelpCommand = true;
                return false;
            }
        }
        catch ( const boost::program_options::error &ex )
        {
            std::cerr << ex.what() << '\n';
            return false;
        }

        return true;
    }

private:

    boost::program_options::options_description mDesc;
    bool mIsHelpCommand;
    bool mIsInitialised;
    S32 mArgc;
    const char **mArgv;

};

/// @}

/// @}

#endif
