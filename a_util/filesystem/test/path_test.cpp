/**
 * @file
 * Filesystem test implementation
 *
 * @copyright
 * @verbatim
   Copyright @ 2017 Audi Electronics Venture GmbH. All rights reserved.

       This Source Code Form is subject to the terms of the Mozilla
       Public License, v. 2.0. If a copy of the MPL was not distributed
       with this file, You can obtain one at https://mozilla.org/MPL/2.0/.

   If it is not possible or desirable to put the notice in a particular file, then
   You may include the notice in a location (such as a LICENSE file in a
   relevant directory) where a recipient would be likely to look for such a notice.

   You may add additional accurate notices of copyright ownership.
   @endverbatim
*/

#include <algorithm>
#include "gtest/gtest.h"
#include "a_util/filesystem/path.h"
using namespace a_util;
using namespace a_util::filesystem;

// Path Tests ==================================
TEST(path_test, TestPathCTORS)
{
    // CTOR, CCTOR, Assignment, Init CTOR
    Path path;
    path = Path("test");
    EXPECT_EQ((std::string) path, "test");
    Path path2("test2");
    EXPECT_EQ((std::string) path2, "test2");
    path = path2;
    EXPECT_EQ(path, path2);
}

TEST(path_test, TestPathIsEmpty)
{
    // isEmpty
    Path path;
    EXPECT_TRUE(path.isEmpty());
    EXPECT_EQ((std::string) path, "");
    EXPECT_EQ(path.toString(Path::PS_BackwardSlash), "");
    EXPECT_EQ(path.toString(Path::PS_ForwardSlash), "");
    EXPECT_EQ(path.toString(Path::PS_Native), "");

    EXPECT_FALSE(Path("test").isEmpty());
}

TEST(path_test, TestPathClear)
{
    // Clear
    EXPECT_FALSE(Path("test").isEmpty());
    EXPECT_TRUE(Path("test").Clear().isEmpty());
}

TEST(path_test, TestPathStringConversion)
{
    // toString, std::string operator
    Path path, path2;

    EXPECT_TRUE(path.setPath("/a/b/c"));
    EXPECT_TRUE(path2.setPath("C:\\a\\b\\c"));

    EXPECT_EQ(path.toString(Path::PS_BackwardSlash), "\\a\\b\\c");
    EXPECT_EQ(path2.toString(Path::PS_ForwardSlash), "C:/a/b/c");

#if WIN32
    EXPECT_EQ(path.toString(Path::PS_Native), "\\a\\b\\c");
    EXPECT_EQ((std::string) path2, "C:\\a\\b\\c");
#else
    EXPECT_EQ(path.toString(Path::PS_Native), "/a/b/c");
    EXPECT_EQ((std::string) path2, "C:/a/b/c");
#endif

    EXPECT_EQ(path.toString(Path::PS_ForwardSlash), "/a/b/c");
    EXPECT_EQ(path2.toString(Path::PS_ForwardSlash), "C:/a/b/c");
}

TEST(path_test, TestPathComparison)
{
    // Comparison operator
    Path path("test"), path2("test");
    EXPECT_TRUE(path == path2);
    path.Clear();
    EXPECT_TRUE(path != path2);
}

TEST(path_test, TestPathSetPath)
{
    // setPath, path normalization
    Path path;
    EXPECT_TRUE(path.setPath("\\\\ci-aev\\test/"));
    EXPECT_EQ(path.toString(Path::PS_ForwardSlash), "//ci-aev/test");
    EXPECT_TRUE(path.setPath("/"));
    EXPECT_EQ(path.toString(Path::PS_ForwardSlash), "/");
    EXPECT_TRUE(path.setPath("C:"));
    EXPECT_EQ(path.toString(Path::PS_ForwardSlash), "C:");
}

TEST(path_test, TestPathGetRoot)
{
    // getRoot
    Path path("C:\\subdir\\..\\anotherone\\test.txt");
    Path path2("/usr/./bin/subdir");
    Path path3("//ci-aev/envinstall/blubb.txt");
    EXPECT_EQ(Path().getRoot().toString(Path::PS_ForwardSlash), "");
    EXPECT_EQ(path.getRoot().toString(Path::PS_ForwardSlash), "C:");
    EXPECT_EQ(path2.getRoot().toString(Path::PS_ForwardSlash), "/");
    EXPECT_EQ(path3.getRoot().toString(Path::PS_ForwardSlash), "//ci-aev");
    EXPECT_EQ(Path("test").getRoot().toString(Path::PS_ForwardSlash), "");
    EXPECT_EQ(Path("test/test.txt").getRoot().toString(Path::PS_ForwardSlash), "");
}

TEST(path_test, TestPathGetParent)
{
    // getParent
    Path path("C:\\subdir\\..\\anotherone\\test.txt");
    Path path2("/usr/./bin/subdir");
    Path path3("//ci-aev/envinstall/blubb.txt");
    EXPECT_EQ(Path().getParent().toString(Path::PS_ForwardSlash), "");
    EXPECT_EQ(path.getParent().toString(Path::PS_ForwardSlash), "C:/subdir/../anotherone");
    EXPECT_EQ(path2.getParent().toString(Path::PS_ForwardSlash), "/usr/./bin");
    EXPECT_EQ(path3.getParent().toString(Path::PS_ForwardSlash), "//ci-aev/envinstall");
    EXPECT_EQ(Path("/").getParent().toString(Path::PS_ForwardSlash), "/");
    EXPECT_EQ(Path("c:").getParent().toString(Path::PS_ForwardSlash), "c:");
    EXPECT_EQ(Path("//ci-aev").getParent().toString(Path::PS_ForwardSlash), "//ci-aev");
    EXPECT_EQ(Path("test").getParent().toString(Path::PS_ForwardSlash), "");
    EXPECT_EQ(Path("test/test.txt").getParent().toString(Path::PS_ForwardSlash), "test");
}

TEST(path_test, TestPathGetName)
{
    // getLastElement
    Path path("C:\\subdir\\..\\anotherone\\test.txt");
    Path path2("/usr/./bin/subdir");
    Path path3("//ci-aev/envinstall/blubb.txt");
    EXPECT_EQ(Path().getLastElement().toString(Path::PS_ForwardSlash), "");
    EXPECT_EQ(path.getLastElement().toString(Path::PS_ForwardSlash), "test.txt");
    EXPECT_EQ(path2.getLastElement().toString(Path::PS_ForwardSlash), "subdir");
    EXPECT_EQ(path3.getLastElement().toString(Path::PS_ForwardSlash), "blubb.txt");
    EXPECT_EQ(Path("/").getLastElement().toString(Path::PS_ForwardSlash), "");
    EXPECT_EQ(Path("c:").getLastElement().toString(Path::PS_ForwardSlash), "");
    EXPECT_EQ(Path("//ci-aev").getLastElement().toString(Path::PS_ForwardSlash), "");
    EXPECT_EQ(Path("test").getLastElement().toString(Path::PS_ForwardSlash), "test");
    EXPECT_EQ(Path("test/test.txt").getLastElement().toString(Path::PS_ForwardSlash), "test.txt");
}

TEST(path_test, TestPathGetExtension)
{
    // getExtension
    Path path("C:\\subdir\\..\\anotherone\\test.txt");
    Path path2("/usr/./bin/subdir");
    Path path3("//ci-aev/envinstall/blubb.txt");
    EXPECT_EQ(Path().getExtension(), "");
    EXPECT_EQ(Path(".").getExtension(), "");
    EXPECT_EQ(Path("../test").getExtension(), "");
    EXPECT_EQ(path.getExtension(), "txt");
    EXPECT_EQ(path2.getExtension(), "");
    EXPECT_EQ(path3.getExtension(), "txt");
    EXPECT_EQ(Path("/").getExtension(), "");
    EXPECT_EQ(Path("c:").getExtension(), "");
    EXPECT_EQ(Path("//ci-aev").getExtension(), "");
    EXPECT_EQ(Path("test").getExtension(), "");
    EXPECT_EQ(Path("test/test.txt").getExtension(), "txt");
}

TEST(path_test, TestPathAppend)
{
    // append
    EXPECT_EQ(Path().append("bla").toString(Path::PS_ForwardSlash), "bla");
    // a folder may contain '.' as well :/
    EXPECT_EQ(
        Path("C:/subdir/../anotherone/test.txt").append("bla").toString(Path::PS_ForwardSlash),
        "C:/subdir/../anotherone/test.txt/bla");
    EXPECT_EQ(Path("/usr/./bin/subdir").append("bla").toString(Path::PS_ForwardSlash),
              "/usr/./bin/subdir/bla");
    EXPECT_EQ(Path("//ci-aev/envinstall/blubb.txt").append("bla").toString(Path::PS_ForwardSlash),
              "//ci-aev/envinstall/blubb.txt/bla");
    EXPECT_EQ(Path("/").append("bla").toString(Path::PS_ForwardSlash), "/bla");
    EXPECT_EQ(Path("c:").append("bla").toString(Path::PS_ForwardSlash), "c:/bla");
    EXPECT_EQ(Path("//ci-aev").append("bla").toString(Path::PS_ForwardSlash), "//ci-aev/bla");
    EXPECT_EQ(Path("test").append("bla").toString(Path::PS_ForwardSlash), "test/bla");
    EXPECT_EQ(Path("test/test.txt").append("bla").toString(Path::PS_ForwardSlash),
              "test/test.txt/bla");
    EXPECT_THROW(Path("blubb").append("C:/"), PathException);
}

TEST(path_test, TestPathReplaceExtension)
{
    // replaceExtension
    EXPECT_EQ(Path().replaceExtension("bla").toString(Path::PS_ForwardSlash), "");
    EXPECT_EQ(Path("C:/subdir/../anotherone/test.txt")
                  .replaceExtension("bla")
                  .toString(Path::PS_ForwardSlash),
              "C:/subdir/../anotherone/test.bla");
    EXPECT_EQ(Path("/usr/./bin/subdir").replaceExtension("bla").toString(Path::PS_ForwardSlash),
              "/usr/./bin/subdir.bla");
    EXPECT_EQ(Path("//ci-aev/envinstall/blubb.txt")
                  .replaceExtension("bla")
                  .toString(Path::PS_ForwardSlash),
              "//ci-aev/envinstall/blubb.bla");
    EXPECT_EQ(Path("/").replaceExtension("bla").toString(Path::PS_ForwardSlash), "/");
    EXPECT_EQ(Path("c:").replaceExtension("bla").toString(Path::PS_ForwardSlash), "c:");
    EXPECT_EQ(Path("//ci-aev").replaceExtension("bla").toString(Path::PS_ForwardSlash), "//ci-aev");
    EXPECT_EQ(Path("test").replaceExtension("bla").toString(Path::PS_ForwardSlash), "test.bla");
    EXPECT_EQ(Path("test/test.txt").replaceExtension("bla").toString(Path::PS_ForwardSlash),
              "test/test.bla");
}

TEST(path_test, TestPathRemoveLastElement)
{
    // removeLastElement
    EXPECT_EQ(Path().removeLastElement().toString(Path::PS_ForwardSlash), "");
    EXPECT_EQ(Path("C:/subdir/../anotherone/test.txt")
                  .removeLastElement()
                  .toString(Path::PS_ForwardSlash),
              "C:/subdir/../anotherone");
    EXPECT_EQ(Path("/usr/./bin/subdir").removeLastElement().toString(Path::PS_ForwardSlash),
              "/usr/./bin");
    EXPECT_EQ(
        Path("//ci-aev/envinstall/blubb.txt").removeLastElement().toString(Path::PS_ForwardSlash),
        "//ci-aev/envinstall");
    EXPECT_EQ(Path("/").removeLastElement().toString(Path::PS_ForwardSlash), "/");
    EXPECT_EQ(Path("c:").removeLastElement().toString(Path::PS_ForwardSlash), "c:");
    EXPECT_EQ(Path("//ci-aev").removeLastElement().toString(Path::PS_ForwardSlash), "//ci-aev");
    EXPECT_EQ(Path("test").removeLastElement().toString(Path::PS_ForwardSlash), "");
    EXPECT_EQ(Path("test/test.txt").removeLastElement().toString(Path::PS_ForwardSlash), "test");
}

TEST(path_test, TestPathIsRelative)
{
    // isRelative
    EXPECT_THROW(Path().isRelative(), PathException);
    EXPECT_FALSE(Path("C:/subdir/../anotherone/test.txt").isRelative());
    EXPECT_FALSE(Path("/usr/./bin/subdir").isRelative());
    EXPECT_FALSE(Path("//ci-aev/envinstall/blubb.txt").isRelative());
    EXPECT_FALSE(Path("/").isRelative());
    EXPECT_FALSE(Path("c:").isRelative());
    EXPECT_FALSE(Path("//ci-aev").isRelative());
    EXPECT_TRUE(Path("test").isRelative());
    EXPECT_TRUE(Path("test/test.txt").isRelative());
    EXPECT_TRUE(Path("../test").isRelative());
    EXPECT_TRUE(Path("..").isRelative());
    EXPECT_TRUE(Path(".").isRelative());
}

TEST(path_test, TestPathIsAbsolute)
{
    // isAbsolute
    EXPECT_THROW(Path().isAbsolute(), PathException);
    EXPECT_TRUE(Path("C:/subdir/../anotherone/test.txt").isAbsolute());
    EXPECT_TRUE(Path("/usr/./bin/subdir").isAbsolute());
    EXPECT_TRUE(Path("//ci-aev/envinstall/blubb.txt").isAbsolute());
    EXPECT_TRUE(Path("/").isAbsolute());
    EXPECT_TRUE(Path("c:").isAbsolute());
    EXPECT_TRUE(Path("//ci-aev").isAbsolute());
    EXPECT_FALSE(Path("test").isAbsolute());
    EXPECT_FALSE(Path("test/test.txt").isAbsolute());
    EXPECT_FALSE(Path("../test").isAbsolute());
    EXPECT_FALSE(Path("..").isAbsolute());
    EXPECT_FALSE(Path(".").isAbsolute());
}

TEST(path_test, TestPathMakeCanonical)
{
    // makeCanonical
    EXPECT_EQ(Path("a/b/c/../../..").makeCanonical().toString(Path::PS_ForwardSlash), "");
    EXPECT_EQ(Path("a/b/c/../..").makeCanonical().toString(Path::PS_ForwardSlash), "a");
    EXPECT_EQ(Path("a/b/c/../../d").makeCanonical().toString(Path::PS_ForwardSlash), "a/d");
    EXPECT_EQ(Path("a/b/c/.././../d").makeCanonical().toString(Path::PS_ForwardSlash), "a/b/d");
    EXPECT_EQ(Path("/../..").makeCanonical().toString(Path::PS_ForwardSlash), "");
    EXPECT_EQ(Path("/..").makeCanonical().toString(Path::PS_ForwardSlash), "");
    EXPECT_EQ(Path("/.git_config").makeCanonical().toString(Path::PS_ForwardSlash), "/.git_config");
    EXPECT_EQ(Path("././.git_config").makeCanonical().toString(Path::PS_ForwardSlash),
              "./.git_config");

    EXPECT_EQ(
        Path("C:/subdir/../anotherone/test.txt").makeCanonical().toString(Path::PS_ForwardSlash),
        "C:/anotherone/test.txt");
    EXPECT_EQ(Path("/usr/./bin/../subdir").makeCanonical().toString(Path::PS_ForwardSlash),
              "/usr/subdir");
    EXPECT_EQ(Path("//ci-aev/envinstall/blubb.txt").makeCanonical().toString(Path::PS_ForwardSlash),
              "//ci-aev/envinstall/blubb.txt");
    EXPECT_EQ(Path("/").makeCanonical().toString(Path::PS_ForwardSlash), "/");
    EXPECT_EQ(Path("c:").makeCanonical().toString(Path::PS_ForwardSlash), "c:");
    EXPECT_EQ(Path("c:/..").makeCanonical().toString(Path::PS_ForwardSlash), "");
    EXPECT_EQ(Path("//ci-aev").makeCanonical().toString(Path::PS_ForwardSlash), "//ci-aev");
    EXPECT_EQ(Path("//ci-aev/..").makeCanonical().toString(Path::PS_ForwardSlash), "");
    EXPECT_EQ(Path("test").makeCanonical().toString(Path::PS_ForwardSlash), "test");
    EXPECT_EQ(Path("test/test.txt").makeCanonical().toString(Path::PS_ForwardSlash),
              "test/test.txt");
    EXPECT_EQ(Path("../test").makeCanonical().toString(Path::PS_ForwardSlash), "../test");
    EXPECT_EQ(Path("..").makeCanonical().toString(Path::PS_ForwardSlash), "..");
    EXPECT_EQ(Path(".").makeCanonical().toString(Path::PS_ForwardSlash), ".");
}
