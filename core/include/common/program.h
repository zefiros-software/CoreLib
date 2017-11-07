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

#include "api/logMode.h"

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

    template< typename tT >
    class Options
    {
        friend class Program;
    public:

        explicit Options( boost::program_options::typed_value< tT > *tv )
            : mTv( tv )
        {
        }

        Options( const Options &o )
            : mTv( o.mTv )
        {
        }

        Options &DefaultValue( const tT &val )
        {
            mTv->default_value( val );
            return *this;
        }

        Options &DefaultValue( const tT &val, const std::string &textual )
        {
            mTv->default_value( val, textual );
            return *this;
        }

        Options &ImplicitValue( const tT &val )
        {
            mTv->implicit_value( val );
            return *this;
        }

        Options &ImplicitValue( const tT &val, const std::string &textual )
        {
            mTv->implicit_value( val, textual );
            return *this;
        }

        Options &ValueName( const std::string &name )
        {
            mTv->value_name( name );
            return *this;
        }

        Options &Notifier( std::function<void( const tT & )> f )
        {
            mTv->notifier( f );
            return *this;
        }

        Options &Composing()
        {
            mTv->composing();
            return *this;
        }

        Options &MultiToken()
        {
            mTv->multitoken();
            return *this;
        }

        Options &ZeroTokens()
        {
            mTv->zero_tokens();
            return *this;
        }

        Options &Required()
        {
            mTv->required();
            return *this;
        }

    private:

        boost::program_options::typed_value< tT > *mTv;
    };

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

    void Init( Console::LogMode logMode = Console::LogMode::All );

    bool IsRunning() const noexcept;

    /// @}


    /// @name CLI
    /// @{


    template< typename tT >
    void AddOption( const std::string &val, const std::string &description,
                    std::function<Options< tT > ( Options< tT > )> call,
                    const std::string &group = "" )
    {
        mDesc[group].add_options()
        ( val.c_str(), call( Options< tT >( boost::program_options::value<tT>() ) ).mTv, description.c_str() );
    }

    void AddOption( const std::string &val, const std::string &description, const std::string &group = "" );

    bool HasOption( const std::string &opt ) const;

    /// @}

protected:

    /// @name Start & Run
    /// @{

    /**
     * Shuts down the application and frees any resources it is using.
     */

    static void Shutdown();

    /// @}

    bool ParseCLI();

private:

    std::map< std::string, boost::program_options::options_description > mDesc;
    boost::program_options::variables_map mVM;
    bool mIsHelpCommand;
    bool mCLIParsed;
    bool mIsInitialised;
    S32 mArgc;
    const char **mArgv;

};

/// @}

/// @}

#endif
