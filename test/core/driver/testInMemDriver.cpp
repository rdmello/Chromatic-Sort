
#include "PixelSort.hpp"
#include "InMemDriver.hpp"

#include "gtest/gtest.h"

namespace PS = PixelSort;

class TestInMemDriver: public ::testing::Test
{
public:
    const PS::Coordinate size;
    const PS::BoxCoordinate extent;
    std::vector<std::vector<PS::RGBColor>> pixels;
    
    TestInMemDriver(): 
        size(15, 25), 
        extent(0, 0, 10, 20), 
        pixels(size.y, std::vector<PS::RGBColor>(size.x, 
            PS::RGBColor(0.5, 0.6, 0.7))) {};
};

TEST_F(TestInMemDriver, Constructor) 
{
    // construct InMemDriver object
    PS::InMemDriver image(pixels, extent);

    /* Check that the rgb components are correctly set */
    for (auto& row: pixels) {
        for (auto& element: row) {
            EXPECT_EQ(0.5, element.red());
            EXPECT_EQ(0.6, element.green());
            EXPECT_EQ(0.7, element.blue());
        }
    }
    
}

TEST_F(TestInMemDriver, Dimensions)
{
    // construct InMemDriver object
    PS::InMemDriver image(pixels, extent);
    
    /* Verify that image size is correct */
    EXPECT_EQ(size.y, image.pixels.size());
    EXPECT_EQ(size.x, image.pixels[0].size());

    /* Verify that columns and rows are being reported correctly */
    EXPECT_EQ(extent.height, image.rows());
    EXPECT_EQ(extent.width, image.columns());

    /* Verify that coordinates are correctly bounded when they exceed
     * the image dimensions */
    EXPECT_EQ(extent.height, image.box.height);
    EXPECT_EQ(extent.width, image.box.width);
}

TEST_F(TestInMemDriver, ReadRGBFromXY)
{
    // construct InMemDriver object
    PS::InMemDriver image(pixels, extent);

    /* Check that the rgb components are correctly being read */
    for (unsigned i = 0; i < pixels.size(); ++i) {
        for (unsigned j = 0; j < pixels[0].size(); ++j) {
            PS::Pixel color(PS::Coordinate(j, i), PS::RGBColor(0, 0, 0));
            image.readRGBFromXY(color);
            EXPECT_EQ(pixels[i][j].red(), color.red());
            EXPECT_EQ(pixels[i][j].green(), color.green());
            EXPECT_EQ(pixels[i][j].blue(), color.blue());
        }
    }
}

TEST_F(TestInMemDriver, WriteRGBFromXY)
{
    // construct InMemDriver object
    PS::InMemDriver image(pixels, extent);

    /* Check that the rgb components are correctly being read */
    for (unsigned i = 0; i < pixels.size(); ++i) {
        for (unsigned j = 0; j < pixels[0].size(); ++j) {
            PS::Pixel color(PS::Coordinate(j, i), PS::RGBColor(0, 0, 0));
            image.writeRGBFromXY(color);
            EXPECT_EQ(pixels[i][j].red(), color.red());
            EXPECT_EQ(pixels[i][j].green(), color.green());
            EXPECT_EQ(pixels[i][j].blue(), color.blue());
        }
    }
}


TEST_F(TestInMemDriver, BlockSort) 
{
    // construct InMemDriver object
    PS::InMemDriver image(pixels, extent);

    double i = 1;
    /* Populate first row of object */
    for (PS::RGBColor& c: image.pixels[0]) {
        c.red(i); 
        i -= (1.0/extent.width);
    }

    /* verify that last red color on first row has been set correctly */
    /* Annoyingly, it seems like these two double-precision floating-pt
     * calculations are producing results that differ by greater than 4 ULPs
     * so we cannot directly use ASSERT_DOUBLE_EQ. Using EXPECT_NEAR
     * with an arbitrary abs_error for now.
     */
    EXPECT_NEAR(1.0/extent.width, image.pixels[0][extent.width-1].red(), 0.00001);

}

