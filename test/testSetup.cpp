
#include "PixelSort.hpp"
#include "gtest/gtest.h"

TEST(addTwoNumbers, CoordinateTest) {
    PixelSort::Coordinate co{100, 200};
    EXPECT_EQ(unsigned(100), co.x);
    EXPECT_EQ(unsigned(200), co.y);
}

