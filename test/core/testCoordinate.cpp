
#include "gtest/gtest.h"

#include "PixelSort.hpp"

namespace PS = PixelSort;

TEST(TestCoordinate, PSCoordinate)
{
    /* constructor test */
    PS::Coordinate a{15, 25};
    EXPECT_EQ(static_cast<unsigned>(15), a.x);
    EXPECT_EQ(static_cast<unsigned>(25), a.y);

    /* setter test */
    a.x = 35;
    a.y = 52;
    EXPECT_EQ(static_cast<unsigned>(35), a.x);
    EXPECT_EQ(static_cast<unsigned>(52), a.y);
}

TEST(TestCoordinate, PSRGBAColor)
{
    /* Test aggregate initializer */
    PS::RGBAColor a{0xDEADCAFE};
    EXPECT_EQ(0xDEADCAFE, a.color);

    /* Test getters */
    EXPECT_EQ(uint32_t(0xAD), a.red());
    EXPECT_EQ(uint32_t(0xCA), a.green());
    EXPECT_EQ(uint32_t(0xFE), a.blue());

    /* Test setters */
    a.red(0x12);
    a.green(0x34);
    a.blue(0x56);
    EXPECT_EQ(uint32_t(0x12), a.red());
    EXPECT_EQ(uint32_t(0x34), a.green());
    EXPECT_EQ(uint32_t(0x56), a.blue());

    /* Test aggregate size */
    EXPECT_EQ(unsigned(4), sizeof(a));
}

TEST(TestCoordinate, PSPixel)
{
    /* Test aggregate initializer */
    PS::Pixel a{15, 25, 0xDEADCAFE};
    EXPECT_EQ(0xDEADCAFE, a.color);
    EXPECT_EQ(static_cast<unsigned>(15), a.x);
    EXPECT_EQ(static_cast<unsigned>(25), a.y);

    /* Test getters */
    EXPECT_EQ(uint32_t(0xAD), a.red());
    EXPECT_EQ(uint32_t(0xCA), a.green());
    EXPECT_EQ(uint32_t(0xFE), a.blue());

    /* Test setters */
    a.red(0x12);
    a.green(0x34);
    a.blue(0x56);
    a.x = 10;
    a.y = 20;
    EXPECT_EQ(uint32_t(0x12), a.red());
    EXPECT_EQ(uint32_t(0x34), a.green());
    EXPECT_EQ(uint32_t(0x56), a.blue());
    EXPECT_EQ(static_cast<unsigned>(10), a.x);
    EXPECT_EQ(static_cast<unsigned>(20), a.y);

    /* Test aggregate size */
    EXPECT_EQ(unsigned(12), sizeof(a));
}

