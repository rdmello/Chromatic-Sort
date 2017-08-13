
#include "PixelSort.hpp"
#include "InMemDriver.hpp"

#include "gtest/gtest.h"

namespace PS = PixelSort;

TEST(testInMemDriver, Constructor) {
    PS::Coordinate co{10, 20};
    EXPECT_EQ(unsigned(10), co.x);
    EXPECT_EQ(unsigned(20), co.y);
    
    PS::InMemDriver image(co, PS::BoxCoordinate(0, 0, 10, 20));
    image.print();

    EXPECT_EQ(0.5, image.pixels[0][0].red());

}
