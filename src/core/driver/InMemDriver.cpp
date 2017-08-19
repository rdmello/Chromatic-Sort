
#include "InMemDriver.hpp"

#include <iostream>

/*
 * build the InMemImage
 */
PixelSort::InMemDriver::InMemDriver(std::vector<std::vector<RGBColor>>& pixels, const BoxCoordinate& box):
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
    RGBColor rgbColor = pixels[p.y + box.y][p.x + box.x];

    p.red(rgbColor.red());
    p.green(rgbColor.green());
    p.blue(rgbColor.blue());
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
    pixels[p.y + box.y][p.x + box.x] = RGBColor(p.red(), p.green(), p.blue());
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
    for(const std::vector<RGBColor>& row: pixels) {
        for(const RGBColor& color: row) {
            std::cout << "(" << color.red() << "," 
                      << color.green() << "," << color.blue() << ") ";
        }
        std::cout << std::endl;
    }
}
