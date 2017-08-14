
#include "PixelSort.hpp"
#include "InMemDriver.hpp"

#include "gtest/gtest.h"

namespace PS = PixelSort;

/* Size of test image being created */
PS::Coordinate extent{10, 20};

TEST(testInMemDriver, Constructor) 
{
    PS::InMemDriver image(extent, PS::BoxCoordinate(0, 0, 15, 25));
    image.print();

    /* Check that red component is correctly set */
    EXPECT_EQ(0.5, image.pixels[0][0].red());

    /* Verify that image size is correct */
    EXPECT_EQ(extent.y, image.pixels.size());
    EXPECT_EQ(extent.x, image.pixels[0].size());

    /* Verify that columns and rows are being reported correctly */
    EXPECT_EQ(extent.y, image.rows());
    EXPECT_EQ(extent.x, image.columns());

    /* Verify that coordinates are correctly bounded when they exceed
     * the image dimensions */
    EXPECT_EQ(extent.y, image.box.height);
    EXPECT_EQ(extent.x, image.box.width);
}

TEST(testInMemDriver, BlockSort) 
{
    /* construct image driver object */
    PS::InMemDriver image(extent, PS::BoxCoordinate(0, 0, 10, 20));
   
    double i = 1;
    /* Populate first row of object */
    for (PS::RGBColor& c: image.pixels[0]) {
        c.red(i); 
        i -= (1.0/extent.x);
    }

    /* verify that last red color on first row has been set correctly */
    /* Annoyingly, it seems like these two double-precision floating-pt
     * calculations are producing results that differ by greater than 4 ULPs
     * so we cannot directly use ASSERT_DOUBLE_EQ. Using EXPECT_NEAR
     * with an arbitrary abs_error for now.
     */
    EXPECT_NEAR(1.0/extent.x, image.pixels[0][extent.x-1].red(), 0.00001);
}

