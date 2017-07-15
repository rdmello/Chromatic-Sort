
#include "QImageDriver.hpp"

/*
 * set the reference to the QImage
 */
PixelSort::QImageDriver::QImageDriver(QImage& image, const BoxCoordinate& box):
image{image}, box{box, image.width(), image.height()} 
{
    /* do nothing */
}

void 
PixelSort::QImageDriver::readPrep (const BoxCoordinate& box)
{
    /* do nothing */
}


void 
PixelSort::QImageDriver::readRGBFromXY (Pixel& p) const
{
    QColor rgbColor = image.pixelColor(p.x, p.y);

    p.red(rgbColor.redF());
    p.green(rgbColor.greenF());
    p.blue(rgbColor.blueF());
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
    QColor color = QColor::fromRgbF(p.red(), p.green(), p.blue());
    image.setPixelColor(p.x, p.y, color);
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

