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

#include "common/file.h"

#include "engineTest.h"

namespace
{

    TEST(Directory, Exists)
    {
        //! [Exists]
        const std::string directory = ::Test::GenerateDirectoryName("common");

        {
            ASSERT_TRUE(Directory::Create(directory));
        }

        EXPECT_TRUE(Directory::Exists(directory));
        //! [Exists]
        ::Test::CleanUp(directory);
    }

    TEST(Directory, Exists, NoTrailingSlash)
    {
        std::string directory = ::Test::GenerateDirectoryName("common");

        {
            ASSERT_TRUE(Directory::Create(directory));
        }

        directory.pop_back();

        EXPECT_TRUE(Directory::Exists(directory));

        ::Test::CleanUp(directory);
    }

    TEST(Directory, Exists, IsFile)
    {
        std::string root = ::Test::GenerateDirectoryName("common");
        const std::string file = root + Path::GetUniqueFileName();
        const std::string content = ::Test::GenerateRandomString();

        {
            ASSERT_TRUE(Directory::Create(root));
            ASSERT_TRUE(Directory::Exists(root));

            ::Test::GenerateRandomFile(file, content);
        }

        EXPECT_FALSE(Directory::Exists(file));

        ::Test::CleanUp(root);
    }

    TEST(Directory, Exists, NotExisting)
    {
        const std::string directory = ::Test::GenerateDirectoryName("common");
        EXPECT_FALSE(Directory::Exists(directory));
    }

    TEST(Directory, IsEmpty, Directories)
    {
        const std::string root = ::Test::GenerateDirectoryName("common");
        const std::string dirA = root + ::Test::GenerateDirectoryName();
        const std::string dirB = root + ::Test::GenerateDirectoryName();

        {
            ASSERT_TRUE(Directory::Create(root));

            ASSERT_TRUE(Directory::Exists(root));

            ASSERT_TRUE(Directory::Create(dirA));
            ASSERT_TRUE(Directory::Create(dirB));

            ASSERT_TRUE(Directory::Exists(dirA));
            ASSERT_TRUE(Directory::Exists(dirB));
        }

        EXPECT_FALSE(Directory::IsEmpty(root, true));
        EXPECT_TRUE(Directory::IsEmpty(root, false));

        ::Test::CleanUp(root);
    }

    TEST(Directory, IsEmpty, Files)
    {
        //! [IsEmpty]
        const std::string root = ::Test::GenerateDirectoryName("common");
        const std::string dirA = root + ::Test::GenerateDirectoryName();
        const std::string dirB = root + ::Test::GenerateDirectoryName();
        const std::string fileA = root + Path::GetUniqueFileName();
        const std::string fileB = root + Path::GetUniqueFileName();
        const std::string fileC = dirA + Path::GetUniqueFileName();

        {
            ASSERT_TRUE(Directory::Create(root));

            ASSERT_TRUE(Directory::Exists(root));

            ASSERT_TRUE(Directory::Create(dirA));
            ASSERT_TRUE(Directory::Create(dirB));

            ASSERT_TRUE(Directory::Exists(dirA));
            ASSERT_TRUE(Directory::Exists(dirB));

            ::Test::GenerateRandomFile(fileA);
            ::Test::GenerateRandomFile(fileB);
            ::Test::GenerateRandomFile(fileC);
        }

        EXPECT_FALSE(Directory::IsEmpty(root, true));
        EXPECT_FALSE(Directory::IsEmpty(root, false));

        //! [IsEmpty]

        ::Test::CleanUp(root);
    }

    TEST(Directory, IsEmpty, Empty)
    {
        const std::string root = ::Test::GenerateDirectoryName("common");

        {
            ASSERT_TRUE(Directory::Create(root));

            ASSERT_TRUE(Directory::Exists(root));
        }

        EXPECT_TRUE(Directory::IsEmpty(root, true));

        ::Test::CleanUp(root);
    }

    TEST(Directory, IsEmpty, NonExistent)
    {
        const std::string root = ::Test::GenerateDirectoryName("common");

        {
            ASSERT_FALSE(Directory::Exists(root));
        }

        EXPECT_TRUE(Directory::IsEmpty(root, true));
        EXPECT_TRUE(Directory::IsEmpty(root, false));

        ::Test::CleanUp(root);
    }

    TEST(Directory, List, NonRecursive)
    {
        const std::string root = ::Test::GenerateDirectoryName("common");
        const std::string dirA = root + ::Test::GenerateDirectoryName();
        const std::string dirB = root + ::Test::GenerateDirectoryName();

        {
            ASSERT_TRUE(Directory::Create(root));

            ASSERT_TRUE(Directory::Exists(root));

            ASSERT_TRUE(Directory::Create(dirA));
            ASSERT_TRUE(Directory::Create(dirB));

            ASSERT_TRUE(Directory::Exists(dirA));
            ASSERT_TRUE(Directory::Exists(dirB));
        }


        const std::vector< std::string > contents = Directory::List(root, false);
        std::vector< std::string > test;
        test.push_back(dirA);
        test.push_back(dirB);

        EXPECT_EQ(2u, contents.size());
        EXPECT_TRUE(std::is_permutation(contents.begin(), contents.end(), test.begin()));

        ::Test::CleanUp(root);
    }

    TEST(Directory, List, Recursive)
    {
        //! [List]
        const std::string root = ::Test::GenerateDirectoryName("common");
        const std::string dirA = root + ::Test::GenerateDirectoryName();
        const std::string dirB = root + ::Test::GenerateDirectoryName();
        const std::string dirC = dirA + ::Test::GenerateDirectoryName();

        {
            ASSERT_TRUE(Directory::Create(root));

            ASSERT_TRUE(Directory::Exists(root));

            ASSERT_TRUE(Directory::Create(dirA));
            ASSERT_TRUE(Directory::Create(dirB));
            ASSERT_TRUE(Directory::Create(dirC));

            ASSERT_TRUE(Directory::Exists(dirA));
            ASSERT_TRUE(Directory::Exists(dirB));
            ASSERT_TRUE(Directory::Exists(dirC));
        }

        const std::vector< std::string > contents = Directory::List(root, true);

        std::vector< std::string > test;
        test.push_back(dirA);
        test.push_back(dirB);
        test.push_back(dirC);

        EXPECT_EQ(3u, contents.size());
        EXPECT_TRUE(std::is_permutation(contents.begin(), contents.end(), test.begin()));

        //! [List]

        ::Test::CleanUp(root);
    }

    TEST(Directory, List, Default)
    {
        const std::string root = ::Test::GenerateDirectoryName("common");
        const std::string dirA = root + ::Test::GenerateDirectoryName();
        const std::string dirB = root + ::Test::GenerateDirectoryName();

        {
            ASSERT_TRUE(Directory::Create(root));

            EXPECT_TRUE(Directory::Exists(root));

            ASSERT_TRUE(Directory::Create(dirA));
            ASSERT_TRUE(Directory::Create(dirB));

            ASSERT_TRUE(Directory::Exists(dirA));
            ASSERT_TRUE(Directory::Exists(dirB));
        }


        const std::vector< std::string > contents = Directory::List(root, false);

        std::vector< std::string > test;
        test.push_back(dirA);
        test.push_back(dirB);

        EXPECT_EQ(2u, contents.size());
        EXPECT_TRUE(std::is_permutation(contents.begin(), contents.end(), test.begin()));

        ::Test::CleanUp(root);
    }

    TEST(Directory, List, Empty)
    {
        const std::string root = ::Test::GenerateDirectoryName("common");

        {
            ASSERT_TRUE(Directory::Create(root));

            ASSERT_TRUE(Directory::Exists(root));
        }

        const std::vector< std::string > contents = Directory::List(root);
        EXPECT_EQ(0u, contents.size());

        ::Test::CleanUp(root);
    }

    TEST(Directory, List, File)
    {
        const std::string root = ::Test::GenerateDirectoryName("common");
        const std::string file = root + Path::GetUniqueFileName();
        const std::string content = ::Test::GenerateRandomString();

        {
            ASSERT_TRUE(Directory::Create(root));
            ASSERT_TRUE(Directory::Exists(root));

            ::Test::GenerateRandomFile(file, content);
        }

        const std::vector< std::string > contents = Directory::List(root);
        EXPECT_EQ(0u, contents.size());

        ::Test::CleanUp(root);
    }

    TEST(Directory, Delete, NonExistent)
    {
        //! [Delete]
        const std::string directory = ::Test::GenerateDirectoryName("common");

        {
            ASSERT_FALSE(Directory::Exists(directory));
        }

        Directory::Delete(directory);
    }

    TEST(Directory, Delete, SingleDirectory)
    {
        //! [Delete]
        const std::string directory = ::Test::GenerateDirectoryName("common");

        {
            ASSERT_TRUE(Directory::Create(directory));
            ASSERT_TRUE(Directory::Exists(directory));
        }

        Directory::Delete(directory);

        EXPECT_FALSE(Directory::Exists(directory));
        //! [Delete]

        ::Test::CleanUp(directory);
    }

    TEST(Directory, Delete, MultipleDirectories)
    {
        const std::string root = ::Test::GenerateDirectoryName("common");
        const std::string dirA = root + ::Test::GenerateDirectoryName();

        {
            ASSERT_TRUE(Directory::CreateAll(dirA));

            ASSERT_TRUE(Directory::Exists(root));
            ASSERT_TRUE(Directory::Exists(dirA));
        }

        Directory::Delete(root);

        EXPECT_TRUE(Directory::Exists(root));
        EXPECT_TRUE(Directory::Exists(dirA));

        ::Test::CleanUp(root);
    }

    TEST(Directory, DeleteAll, NonExistent)
    {
        //! [Delete]
        const std::string directory = ::Test::GenerateDirectoryName("common");

        {
            ASSERT_FALSE(Directory::Exists(directory));
        }

        Directory::DeleteAll(directory);
    }

    TEST(Directory, DeleteAll, SingleDirectory)
    {
        const std::string root = ::Test::GenerateDirectoryName("common");
        const std::string dirA = root + ::Test::GenerateDirectoryName();

        {
            ASSERT_TRUE(Directory::CreateAll(dirA));

            ASSERT_TRUE(Directory::Exists(root));
            ASSERT_TRUE(Directory::Exists(dirA));
        }

        Directory::DeleteAll(dirA);

        EXPECT_TRUE(Directory::Exists(root));
        EXPECT_FALSE(Directory::Exists(dirA));

        ::Test::CleanUp(root);
    }

    TEST(Directory, DeleteAll, MultipleDirectories)
    {
        //! [DeleteAll]
        const std::string root = ::Test::GenerateDirectoryName("common");
        const std::string dirA = root + ::Test::GenerateDirectoryName();

        {
            ASSERT_TRUE(Directory::CreateAll(dirA));

            EXPECT_TRUE(Directory::Exists(root));
            EXPECT_TRUE(Directory::Exists(dirA));
        }

        Directory::DeleteAll(root);

        EXPECT_FALSE(Directory::Exists(root));
        EXPECT_FALSE(Directory::Exists(dirA));
        //! [DeleteAll]

        ::Test::CleanUp(root);
    }

    TEST(Directory, Create, SingleDirectory)
    {
        //! [Create]
        const std::string directory = ::Test::GenerateDirectoryName("common");

        {
            ASSERT_FALSE(Directory::Exists(directory));
        }

        EXPECT_TRUE(Directory::Create(directory));

        EXPECT_TRUE(Directory::Exists(directory));

        //! [Create]

        ::Test::CleanUp(directory);
    }

    TEST(Directory, Create, NoBackslash)
    {
        const std::string directory = "common/file";

        {
            ASSERT_FALSE(Directory::Exists(directory));
        }

        EXPECT_TRUE(Directory::Create(directory));

        EXPECT_TRUE(Directory::Exists(directory));

        ::Test::CleanUp(directory);
    }

    TEST(Directory, Create, CreateDirectoryTruncateFile)
    {
        const std::string directory = "common/file-dir/file";

        {
            ASSERT_FALSE(Directory::Exists(directory));
        }

        ASSERT_FALSE(Directory::Create(directory));

        ASSERT_FALSE(Directory::Exists("common/file-dir/"));

        ::Test::CleanUp("common/file-dir/");
    }

    TEST(Directory, Create, NoParent)
    {
        const std::string directory = "common-dir-create-noparent/";

        {
            ASSERT_FALSE(Directory::Exists(directory));
        }

        EXPECT_TRUE(Directory::Create(directory));

        EXPECT_TRUE(Directory::Exists(directory));

        ::Test::CleanUp(directory);
    }

    TEST(Directory, Create, MultipleDirectories)
    {
        const std::string root = ::Test::GenerateDirectoryName("common");
        const std::string dirA = root + ::Test::GenerateDirectoryName();

        {
            ASSERT_FALSE(Directory::Exists(root));
            ASSERT_FALSE(Directory::Exists(dirA));
        }

        EXPECT_FALSE(Directory::Create(dirA));

        EXPECT_FALSE(Directory::Exists(root));
        EXPECT_FALSE(Directory::Exists(dirA));

        ::Test::CleanUp(root);
    }

    TEST(Directory, CreateAll, SingleDirectory)
    {
        const std::string directory = ::Test::GenerateDirectoryName("common");

        {
            ASSERT_FALSE(Directory::Exists(directory));
        }

        EXPECT_TRUE(Directory::CreateAll(directory));

        EXPECT_TRUE(Directory::Exists(directory));

        ::Test::CleanUp(directory);
    }

    TEST(Directory, CreateAll, MultipleDirectories)
    {
        //! [CreateAll]
        const std::string root = ::Test::GenerateDirectoryName("common");
        const std::string dirA = root + ::Test::GenerateDirectoryName();

        {
            ASSERT_FALSE(Directory::Exists(root));
            ASSERT_FALSE(Directory::Exists(dirA));
        }

        EXPECT_TRUE(Directory::CreateAll(dirA));

        EXPECT_TRUE(Directory::Exists(root));
        EXPECT_TRUE(Directory::Exists(dirA));

        //! [CreateAll]

        ::Test::CleanUp(root);
    }

    TEST(Directory, Copy)
    {
        //! [Copy]
        const std::string root  = ::Test::GenerateDirectoryName("common");
        const std::string root2 = ::Test::GenerateDirectoryName("common");

        const std::string nameA = Path::GetUniqueFileName();
        const std::string fileA = root + nameA;
        const std::string file2A = root2 + nameA;

        const std::string contentA = ::Test::GenerateRandomString();

        {
            ASSERT_TRUE(Directory::Create(root));

            ASSERT_TRUE(Directory::Exists(root));

            {
                std::ofstream file;
                File::WriteOpen(file, fileA, Path::Type::None);
                file << contentA;
                file.close();

                ASSERT_TRUE(File::Exists(fileA));
                ASSERT_EQ(contentA, File::ReadAllText(fileA));
            }
        }

        {
            ASSERT_FALSE(Directory::Exists(root2));
        }

        EXPECT_TRUE(Directory::Copy(root, root2));

        {
            EXPECT_TRUE(File::Exists(fileA));
            EXPECT_EQ(contentA, File::ReadAllText(fileA));
        }

        {
            EXPECT_FALSE(File::Exists(file2A));
        }
        //! [Copy]

        ::Test::CleanUp(root);
        ::Test::CleanUp(root2);
    }

    TEST(Directory, Copy, BothExist)
    {
        const std::string root  = ::Test::GenerateDirectoryName("common");
        const std::string root2 = ::Test::GenerateDirectoryName("common");

        {
            ASSERT_TRUE(Directory::Create(root));

            ASSERT_TRUE(Directory::Exists(root));

            ASSERT_TRUE(Directory::Create(root2));

            ASSERT_TRUE(Directory::Exists(root2));
        }

        EXPECT_FALSE(Directory::Copy(root, root2));


        ::Test::CleanUp(root);
        ::Test::CleanUp(root2);
    }

    TEST(Directory, Copy, IsSame)
    {
        const std::string root = ::Test::GenerateDirectoryName("common");

        {
            ASSERT_TRUE(Directory::Create(root));

            ASSERT_TRUE(Directory::Exists(root));
        }

        EXPECT_FALSE(Directory::Copy(root, root));

        ::Test::CleanUp(root);
    }

    TEST(Directory, Copy, Parent)
    {
        const std::string root = ::Test::GenerateDirectoryName("common");
        const std::string root2 = ::Test::GenerateDirectoryName(root);

        {
            ASSERT_TRUE(Directory::Create(root));

            ASSERT_TRUE(Directory::Exists(root));

            ASSERT_FALSE(Directory::Exists(root2));
        }

        EXPECT_FALSE(Directory::Copy(root, root2));

        ::Test::CleanUp(root);
    }

    TEST(Directory, Copy, NotExists)
    {
        const std::string root = ::Test::GenerateDirectoryName("common2");
        const std::string root2 = ::Test::GenerateDirectoryName("common");

        {
            ASSERT_TRUE(Directory::Create(root2));

            ASSERT_FALSE(Directory::Exists(root));
            ASSERT_TRUE(Directory::Exists(root2));
        }

        EXPECT_FALSE(Directory::Copy(root, root2));

        ::Test::CleanUp(root);
    }

    TEST(Directory, Move, Content)
    {
        //! [Move]
        const std::string root  = ::Test::GenerateDirectoryName("common");
        const std::string root2 = ::Test::GenerateDirectoryName("common");

        const std::string dirNameA = ::Test::GenerateDirectoryName();
        const std::string dirNameB = ::Test::GenerateDirectoryName();

        const std::string dirA = root + dirNameA;
        const std::string dirB = root + dirNameB;

        const std::string dir2A = root2 + dirNameA;
        const std::string dir2B = root2 + dirNameB;

        const std::string nameA = Path::GetUniqueFileName();
        const std::string nameB = Path::GetUniqueFileName();
        const std::string nameC = Path::GetUniqueFileName();

        const std::string fileA = root + nameA;
        const std::string fileB = root + nameB;
        const std::string fileC = dirA + nameC;

        const std::string file2A = root2 + nameA;
        const std::string file2B = root2 + nameB;
        const std::string file2C = dir2A + nameC;

        const std::string contentA = ::Test::GenerateRandomString();
        const std::string contentB = ::Test::GenerateRandomString();
        const std::string contentC = ::Test::GenerateRandomString();

        {
            ASSERT_TRUE(Directory::Create(root));

            ASSERT_TRUE(Directory::Exists(root));

            ASSERT_TRUE(Directory::Create(dirA));
            ASSERT_TRUE(Directory::Create(dirB));

            ASSERT_TRUE(Directory::Exists(dirA));
            ASSERT_TRUE(Directory::Exists(dirB));

            {
                std::ofstream file;
                File::WriteOpen(file, fileA, Path::Type::None);
                file << contentA;
                file.close();

                ASSERT_TRUE(File::Exists(fileA));
                ASSERT_EQ(contentA, File::ReadAllText(fileA));
            }
            {
                std::ofstream file;
                File::WriteOpen(file, fileB, Path::Type::None);
                file << contentB;
                file.close();

                ASSERT_TRUE(File::Exists(fileB));
                ASSERT_EQ(contentB, File::ReadAllText(fileB));
            }
            {
                std::ofstream file;
                File::WriteOpen(file, fileC, Path::Type::None);
                file << contentC;
                file.close();

                ASSERT_TRUE(File::Exists(fileC));
                ASSERT_EQ(contentC, File::ReadAllText(fileC));
            }

        }

        {
            ASSERT_FALSE(Directory::Exists(root2));
        }

        EXPECT_TRUE(Directory::Move(root, root2));

        {
            EXPECT_FALSE(Directory::Exists(dirA));
            EXPECT_FALSE(Directory::Exists(dirB));

            EXPECT_FALSE(File::Exists(fileA));
            EXPECT_FALSE(File::Exists(fileB));
            EXPECT_FALSE(File::Exists(fileC));
        }

        {
            EXPECT_TRUE(Directory::Exists(dir2A));
            EXPECT_TRUE(Directory::Exists(dir2B));

            EXPECT_TRUE(File::Exists(file2A));
            EXPECT_EQ(contentA, File::ReadAllText(file2A));


            EXPECT_TRUE(File::Exists(file2B));
            EXPECT_EQ(contentB, File::ReadAllText(file2B));


            EXPECT_TRUE(File::Exists(file2C));
            EXPECT_EQ(contentC, File::ReadAllText(file2C));
        }

        //! [Move]

        ::Test::CleanUp(root);
        ::Test::CleanUp(root2);
    }

    TEST(Directory, Move, BothExist)
    {
        const std::string root  = ::Test::GenerateDirectoryName("common");
        const std::string root2 = ::Test::GenerateDirectoryName("common");

        {
            ASSERT_TRUE(Directory::Create(root));

            ASSERT_TRUE(Directory::Exists(root));

            ASSERT_TRUE(Directory::Create(root2));

            ASSERT_TRUE(Directory::Exists(root2));
        }

        EXPECT_FALSE(Directory::Move(root, root2));


        ::Test::CleanUp(root);
        ::Test::CleanUp(root2);
    }

    TEST(Directory, Move, IsSame)
    {
        const std::string root  = ::Test::GenerateDirectoryName("common");

        {
            ASSERT_TRUE(Directory::Create(root));

            ASSERT_TRUE(Directory::Exists(root));
        }

        EXPECT_FALSE(Directory::Move(root, root));


        ::Test::CleanUp(root);
    }

    TEST(Directory, Move, Parent)
    {
        const std::string root = ::Test::GenerateDirectoryName("common");
        const std::string root2 = ::Test::GenerateDirectoryName(root);

        {
            ASSERT_TRUE(Directory::Create(root));

            ASSERT_TRUE(Directory::Exists(root));

            ASSERT_FALSE(Directory::Exists(root2));
        }

        EXPECT_FALSE(Directory::Move(root, root2));

        ::Test::CleanUp(root);
    }

    TEST(Directory, Move, NotExists)
    {
        const std::string root = ::Test::GenerateDirectoryName("common2");
        const std::string root2 = ::Test::GenerateDirectoryName("common");

        {
            ASSERT_TRUE(Directory::Create(root2));

            ASSERT_FALSE(Directory::Exists(root));
            ASSERT_TRUE(Directory::Exists(root2));
        }

        EXPECT_FALSE(Directory::Move(root, root2));

        ::Test::CleanUp(root);
    }
}
