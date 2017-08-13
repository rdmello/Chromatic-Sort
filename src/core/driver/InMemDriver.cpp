
#include "InMemDriver.hpp"

#include <iostream>

/*
 * build the InMemImage
 */
PixelSort::InMemDriver::InMemDriver(const Coordinate& maxExtent, const BoxCoordinate& box):
pixels{maxExtent.y, std::vector<RGBColor>{maxExtent.y, RGBColor(0.5, 0.6, 0.7)}},
boxExtent{maxExtent}, 
box{box, maxExtent.y, maxExtent.x} {}

void 
PixelSort::InMemDriver::readPrep (const BoxCoordinate& box)
{
    /* do nothing */
}


void 
PixelSort::InMemDriver::readRGBFromXY (Pixel& p) const
{
    RGBColor rgbColor = pixels[p.y][p.x];

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
    pixels[p.y][p.x] = RGBColor(p.red(), p.green(), p.blue());
}

void 
PixelSort::InMemDriver::writeFinish (void)
{
    /* do nothing */
}

unsigned int
PixelSort::InMemDriver::columns (void) const
{
    return pixels[0].size();
}

unsigned int
PixelSort::InMemDriver::rows (void) const
{
    return pixels.size();
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
