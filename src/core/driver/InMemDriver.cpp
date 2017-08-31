
#include "InMemDriver.hpp"

#include <iostream>

/*
 * build the InMemImage
 */
PixelSort::InMemDriver::InMemDriver(
    std::vector<std::vector<RGBAColor>>& pixels, const BoxCoordinate& box):
pixels{pixels},
box{box, unsigned(pixels[0].size()), unsigned(pixels.size())} {}

void 
PixelSort::InMemDriver::readPrep (const BoxCoordinate& box)
{
    /* do nothing */
}

void 
PixelSort::InMemDriver::readRGBFromXY (Pixel& p) const
{
    RGBAColor pixel = pixels[p.y + box.y][p.x + box.x];
    p = Pixel{p.x, p.y, pixel.color};
}

void 
PixelSort::InMemDriver::readFinish (void) const
{
    /* do nothing */
}

void 
PixelSort::InMemDriver::writePrep (const BoxCoordinate& box)
{
    /* do nothing */
}


void 
PixelSort::InMemDriver::writeRGBFromXY (const Pixel& p)
{
    pixels[p.y + box.y][p.x + box.x] = RGBAColor{p.color};
}

void 
PixelSort::InMemDriver::writeFinish (void)
{
    /* do nothing */
}

unsigned int
PixelSort::InMemDriver::columns (void) const
{
    return box.width;
}

unsigned int
PixelSort::InMemDriver::rows (void) const
{
    return box.height;
}

void
PixelSort::InMemDriver::print(void) const
{
    std::cout << std::hex;
    for(const std::vector<RGBAColor>& row: pixels) {
        for(const RGBAColor& color: row) {
            std::cout << color.color << " ";
        }
        std::cout << std::endl;
    }
    std::cout << std::dec;
}
