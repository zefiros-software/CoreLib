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

#include "engineTest.h"

namespace
{

    ENGINE_TEST( Environment, GetVariable, NonExisting )
    {
        const std::string var = "ENGINE_TEST_VAR";

        EXPECT_FALSE( Environment::HasVariable( var ) );
        EXPECT_EQ( "", Environment::GetVariable( var ) );
    }

    ENGINE_TEST( Environment, Combo )
    {
        // ![Combo]
        const std::string var = "ENGINE_TEST_VAR";
        const std::string value = ::Test::GenerateRandomString();

        ASSERT_FALSE( Environment::HasVariable( var ) );
        ASSERT_EQ( "", Environment::GetVariable( var ) );

        Environment::SetVariable( var, value );

        EXPECT_EQ( value, Environment::GetVariable( var ) );
        ASSERT_TRUE( Environment::HasVariable( var ) );

        Environment::RemoveVariable( var );

        EXPECT_EQ( "", Environment::GetVariable( var ) );
        ASSERT_FALSE( Environment::HasVariable( var ) );
        // ![Combo]
    }

}