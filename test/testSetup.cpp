
/*
 * running a very simple test to verify that 
 * cmake and googletest have been set-up correctly.
 */

#include "PixelSort.hpp"

#include "gtest/gtest.h"

namespace PS = PixelSort;

TEST(testSetup, Start) {
    PS::Coordinate co{10, 20};
    EXPECT_EQ(unsigned(10), co.x);
    EXPECT_EQ(unsigned(20), co.y);
}

