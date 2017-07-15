
#include "../Image.hpp"

#include <Magick++.h>

#ifndef _PIXELSORT_DRIVER_MAGICK_HPP_
#define _PIXELSORT_DRIVER_MAGICK_HPP_

namespace PixelSort {

    class MagickDriver : public Image
    {
        /* Reference to image object */
        Magick::Image& image;

        /* Pointer to quantum array within image */
        Magick::Quantum* q;

        /* Pointer to ROI being sorted */
        BoundedCoordinate box;

    public:
        MagickDriver(Magick::Image& image, const BoxCoordinate& box);
        virtual unsigned int columns (void) const;
        virtual unsigned int rows    (void) const;

        virtual void readPrep (const BoxCoordinate& box);
        virtual void readRGBFromXY (Pixel& p) const;
        virtual void readFinish (void) const;
        
        virtual void writePrep (const BoxCoordinate& box);
        virtual void writeRGBFromXY (const Pixel& p);
        virtual void writeFinish (void);

    };

}

#endif /* _PIXELSORT_DRIVER_MAGICK_HPP_ */


