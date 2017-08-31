
#include "QImageDriver.hpp"

/*
 * set the reference to the QImage
 */
PixelSort::QImageDriver::QImageDriver(QImage& image):
image{image}, 
box{0, 0, static_cast<unsigned int>(image.width()), static_cast<unsigned int>(image.height())}
{
    /* do nothing */
}

void 
PixelSort::QImageDriver::readPrep (const BoxCoordinate& box)
{
    this->box = box;
}


void 
PixelSort::QImageDriver::readRGBFromXY (Pixel& p) const
{
    p.color = image.pixel(box.x + p.x, box.y + p.y);
}

void 
PixelSort::QImageDriver::readFinish (void) const
{
    /* do nothing */
}

void 
PixelSort::QImageDriver::writePrep (const BoxCoordinate& box)
{
    /* do nothing */
}


void 
PixelSort::QImageDriver::writeRGBFromXY (const Pixel& p)
{
    image.setPixelColor(box.x + p.x, box.y + p.y, p.color);
}

void 
PixelSort::QImageDriver::writeFinish (void)
{
    /* do nothing */
}

unsigned int
PixelSort::QImageDriver::columns (void) const
{
    return image.width();
}

unsigned int
PixelSort::QImageDriver::rows (void) const
{
    return image.height();
}

