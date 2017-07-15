
#include "../Image.hpp"

#include <QImage>

#ifndef _PIXELSORT_DRIVER_QIMAGE_HPP_
#define _PIXELSORT_DRIVER_QIMAGE_HPP_

namespace PixelSort {

    class QImageDriver : public Image
    {
        /* Reference to image object */
        QImage& image;

        /* Pointer to ROI being sorted */
        BoundedCoordinate box;

    public:
        QImageDriver(QImage& image, const BoxCoordinate& box);
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

#endif /* _PIXELSORT_DRIVER_QIMAGE_HPP_ */


