
#include "MagickDriver.hpp"

/*
 * set the reference to the Magick::Image 
 */
PixelSort::MagickDriver::MagickDriver(Magick::Image& image, const BoxCoordinate& box):
image{image}, box{box, int(image.rows()), int(image.columns())} 
{
    image.type(Magick::TrueColorType);
}

void 
PixelSort::MagickDriver::readPrep (const BoxCoordinate& box)
{
    this->box = PixelSort::BoundedCoordinate(box, int(image.rows()), int(image.columns()));
    q = image.getPixels(this->box.x, this->box.y, this->box.width, this->box.height);
}


void 
PixelSort::MagickDriver::readRGBFromXY (Pixel& p) const
{
    int idx = 3*(p.x + p.y*box.width);
    Magick::Color color(q[idx], q[idx+1], q[idx+2]);
    Magick::ColorRGB rgbColor(color);

    p.red(rgbColor.red());
    p.green(rgbColor.green());
    p.blue(rgbColor.blue());
}

void 
PixelSort::MagickDriver::readFinish (void) const
{
    /* do nothing */
}

void 
PixelSort::MagickDriver::writePrep (const BoxCoordinate& box)
{
    image.modifyImage();
    this->box = PixelSort::BoundedCoordinate(box, int(image.rows()), int(image.columns()));
    q = image.getPixels(this->box.x, this->box.y, this->box.width, this->box.height);
}


void 
PixelSort::MagickDriver::writeRGBFromXY (const Pixel& p)
{
    Magick::ColorRGB color(p.red(), p.green(), p.blue()); 
    Magick::Quantum* q2 = &q[(3*p.x) + (3*p.y*box.width)];

    *q2 = color.quantumRed();
    *(q2+1) = color.quantumGreen();
    *(q2+2) = color.quantumBlue();
}

void 
PixelSort::MagickDriver::writeFinish (void)
{
    image.syncPixels();
}

unsigned int
PixelSort::MagickDriver::columns (void) const
{
    return image.columns();
}

unsigned int
PixelSort::MagickDriver::rows (void) const
{
    return image.rows();
}

