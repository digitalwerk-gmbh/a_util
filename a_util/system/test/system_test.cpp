/**
 * @file
 * System test implementation
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

#include "gtest/gtest.h"
#include "a_util/filesystem.h"
#include "a_util/result/result_info.h"
#include "a_util/system.h"
using namespace a_util;

TEST(system_test, TestUsername)
{
    // can't test any more than non-emptyness
    ASSERT_FALSE(system::getCurrentUserName().empty());
}

TEST(system_test, TestHostname)
{
    // can't test any more than non-emptyness
    ASSERT_FALSE(system::getHostname().empty());
}

TEST(system_test, TestSystemError)
{
    // might have been set by previous test
    system::resetLastSystemError();

    int code = system::getLastSystemError();
    ASSERT_EQ(code, 0);
    EXPECT_FALSE(system::formatSystemError(code).empty());

    EXPECT_FALSE(filesystem::setWorkingDirectory("HopefullyNonExistingPath") == filesystem::OK);
    code = system::getLastSystemError();
    EXPECT_NE(code, 0);
    EXPECT_FALSE(system::formatSystemError(code).empty());

    // test reset
    system::resetLastSystemError();
    code = system::getLastSystemError();
    EXPECT_EQ(code, 0);
}

TEST(system_test, TestSleep)
{
    system::HighResSchedulingSupport support;
    // leave this here and reconsider once we have a platform where it fails
    EXPECT_TRUE(support.isSupported());

    timestamp_t now = system::getCurrentMilliseconds();
    system::sleepMilliseconds(1500);
    ASSERT_NEAR(1500, static_cast<double>(system::getCurrentMilliseconds() - now), 100);

    now = system::getCurrentMilliseconds();
    system::sleepMilliseconds(500);
    ASSERT_NEAR(500, static_cast<double>(system::getCurrentMilliseconds() - now), 100);

    // warm up, needs initialization
    now = system::getCurrentMicroseconds();
    if (now != -1)
    {
        now = system::getCurrentMicroseconds();
        system::sleepMilliseconds(500);
        ASSERT_NEAR(500000, static_cast<double>(system::getCurrentMicroseconds() - now), 100000);
    }

    now = system::getCurrentMicroseconds();
    system::sleepMicroseconds(1000);
    ASSERT_NEAR(1000, static_cast<double>(system::getCurrentMicroseconds() - now), 2000);
}
