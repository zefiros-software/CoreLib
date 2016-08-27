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

#include "common/file.h"

#include "engineTest.h"

#include <random>

namespace Test
{

    void EnsureTestDirectories( const std::vector<std::string > &strs )
    {
        for ( auto dir : strs )
        {
            if ( Directory::Exists( dir ) )
            {
                Directory::DeleteAll( dir );
            }

            Directory::CreateAll( dir );
            ASSERT_TRUE( Directory::Exists( dir ) );
        }
    }

    std::string GenerateDirectoryName( const std::string &root /*= "" */ )
    {
        return root != "" ? root + "/" + Path::GetUniqueDirectory() : Path::GetUniqueDirectory();
    }

    std::string GenerateRandomString()
    {
        static std::default_random_engine generator = std::default_random_engine( TEST_RANDOM_SEED );
        static std::uniform_int_distribution<U32> distribution( 0, 57 );
        // Imagine the boredom it took to write this :)
        // Sometimes I wonder whether this is the real life...
        static const char *lyrics[58] =
        {
            "Is this the real life?",
            "Is this just fantasy?",
            "Caught in a landslide,",
            "No escape from reality.",

            "Open your eyes,",
            "Look up to the skies and see,",
            "I'm just a poor boy, I need no sympathy,",
            "Because I'm easy come, easy go,",
            "Little high, little low,",
            "Anyway the wind blows doesn't really matter to me, to me.",

            "Mama, just killed a man,",
            "Put a gun against his head,",
            "Pulled my trigger, now he's dead.",
            "Mama, life had just begun,",
            "But now I've gone and thrown it all away.",

            "Mama, ooh,",
            "Didn't mean to make you cry,",
            "If I'm not back again this time tomorrow,",
            "Carry on, carry on as if nothing really matters.",

            "Too late, my time has come,",
            "Sent shivers down my spine,",
            "Body's aching all the time.",
            "Goodbye, everybody, I've got to go,",
            "Gotta leave you all behind and face the truth.",

            "Mama, ooh (anyway the wind blows),",
            "I don't wanna die,",
            "I sometimes wish I'd never been born at all.",

            "I see a little silhouetto of a man,",
            "Scaramouche, Scaramouche, will you do the Fandango?",
            "Thunderbolt and lightning,",
            "Very, very frightening me.",
            "(Galileo) Galileo.",
            "(Galileo) Galileo,",
            "Galileo Figaro",
            "Magnifico.",

            "I'm just a poor boy, nobody loves me.",
            "He's just a poor boy from a poor family,",
            "Spare him his life from this monstrosity.",

            "Easy come, easy go, will you let me go?",
            "Bismillah! No, we will not let you go. (Let him go!)",
            "Bismillah! We will not let you go. (Let him go!)",
            "Bismillah! We will not let you go. (Let me go!)",
            "Will not let you go. (Let me go!)",
            "Never, never let you go",
            "Never let me go, oh.",
            "No, no, no, no, no, no, no.",
            "Oh, mama mia, mama mia (Mama mia, let me go.)",
            "Beelzebub has a devil put aside for me, for me, for me.",

            "So you think you can stone me and spit in my eye?",
            "So you think you can love me and leave me to die?",
            "Oh, baby, can't do this to me, baby,",
            "Just gotta get out, just gotta get right outta here.",

            "(Oh, yeah, oh yeah)",

            "Nothing really matters,",
            "Anyone can see,",
            "Nothing really matters,",
            "Nothing really matters to me.",

            "Anyway the wind blows."
        };

        return lyrics[distribution( generator )];
    }

    S32 GenerateRandomS32()
    {
        static std::default_random_engine generator = std::default_random_engine( TEST_RANDOM_SEED );
        static std::uniform_int_distribution<S32> distribution( std::numeric_limits<S32>::min(),
                                                                std::numeric_limits<S32>::max() );
        return distribution( generator );
    }

    U32 GenerateRandomU32()
    {
        static std::default_random_engine generator = std::default_random_engine( TEST_RANDOM_SEED );
        static std::uniform_int_distribution<U32> distribution( std::numeric_limits<U32>::min(),
                                                                std::numeric_limits<U32>::max() );
        return distribution( generator );
    }

    F32 GenerateRandomF32()
    {
        static std::default_random_engine generator = std::default_random_engine( TEST_RANDOM_SEED );
        static std::uniform_real_distribution<F32> distribution( -1000000, 1000000 );
        return distribution( generator );
    }

    bool GenerateRandomBool()
    {
        // Once upon a time this function made use of std::generate_canonical,
        // but apparently Microsoft thinks [0,1[ is in the range of [10e19, oo[.
        // https://connect.microsoft.com/VisualStudio/feedback/details/811611/std-generate-canonical-does-not-generate-random-number-in-range-0-1#
        // This will hold the same result, however we need more codez, and apparently
        // more documentation.
        static std::default_random_engine generator = std::default_random_engine( TEST_RANDOM_SEED );
        static std::uniform_real_distribution<F32> distribution( 0, 1 );
        return distribution( generator ) > 0.5f;
    }

    void GenerateRandomFile( const std::string &path, std::string content/*= "" */ )
    {
        if ( content == "" )
        {
            content = GenerateRandomString() + "\n" + GenerateRandomString()  + "\n" + GenerateRandomString();
        }

        ASSERT_FALSE( File::Exists( path ) );

        std::ofstream file;
        File::WriteOpen( file, path, Path::Type::None );
        file << content;
        file.close();

        ASSERT_TRUE( File::Exists( path ) );
        ASSERT_EQ( content, File::ReadAllText( path ) );
    }

    void CleanUp( const std::string &path )
    {
        if ( File::Exists( path ) )
        {
            File::Delete( path );
            ASSERT_FALSE( File::Exists( path ) );
        }
        else
        {
            Path::DeleteAll( path );
            ASSERT_FALSE( Directory::Exists( path ) );
        }
    }

}