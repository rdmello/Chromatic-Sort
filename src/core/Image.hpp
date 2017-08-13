
/**
 * Image.hpp
 *   a wrapper for Image classes from Qt and ImageMagick
 *   
 *   the PixelSort::Image class defines an abstract, 
 *   driver-independent interface for images.
 *
 *   this allows the rest of the code in the PixelSort
 *   library to be driver independent
 *
 *   At least two drivers will be implemented:
 *    - MagickDriver.hpp
 *    - QImageDriver.hpp
 *
 *   An additional 'dummy' driver might also be implemented in Image.cpp
 *
 *   Configure flags can be used to select the drivers which
 *   are being compiled
 */

#include "Coordinate.hpp"

#ifndef _PIXELSORT_IMAGE_HPP_
#define _PIXELSORT_IMAGE_HPP_

namespace PixelSort 
{
    class Image 
    {
    public:

        virtual unsigned int columns (void) const = 0;
        virtual unsigned int rows    (void) const = 0;

        virtual void readPrep (const BoxCoordinate& box) = 0;
        virtual void readRGBFromXY (Pixel& p) const = 0;
        virtual void readFinish (void) const = 0;
        
        virtual void writePrep (const BoxCoordinate& box) = 0;
        virtual void writeRGBFromXY (const Pixel& p) = 0;
        virtual void writeFinish (void) = 0;
    };
}

#endif /* _PIXELSORT_IMAGE_HPP_ */
