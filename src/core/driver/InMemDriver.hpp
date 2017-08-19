
/*
 * InMemDriver is an interface for in-memory image objects
 * this class allows us to use vector<vector<RGBColor>> objects with
 * BlockPixelSort, AsendorfSort, etc.
 */

#include "../Image.hpp"

#ifndef _PIXELSORT_DRIVER_INMEM_HPP_
#define _PIXELSORT_DRIVER_INMEM_HPP_

#include <vector>

namespace PixelSort {

    class InMemDriver : public Image
    {
      
    public:
        /* image object */
        std::vector<std::vector<RGBColor>>& pixels;

        /* ROI being sorted */
        BoundedCoordinate box;
 
        InMemDriver(std::vector<std::vector<RGBColor>>& pixels, const BoxCoordinate& box);
        virtual unsigned int columns (void) const;
        virtual unsigned int rows    (void) const;

        virtual void readPrep (const BoxCoordinate& box);
        virtual void readRGBFromXY (Pixel& p) const;
        virtual void readFinish (void) const;
        
        virtual void writePrep (const BoxCoordinate& box);
        virtual void writeRGBFromXY (const Pixel& p);
        virtual void writeFinish (void);

        void print(void) const;

    };
}

#endif /* _PIXELSORT_DRIVER_MAGICK_HPP_ */


