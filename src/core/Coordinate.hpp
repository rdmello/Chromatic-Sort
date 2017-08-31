
/*
 * Coordinate.hpp
 * The Coordinate class and its subclasses provide
 * some simple structures to make it easier to work with Pixels
 * Colors, and boundaries.
 */

#ifndef _PIXELSORT_COORDINATE_HPP_
#define _PIXELSORT_COORDINATE_HPP_

#include <cstdint>

namespace PixelSort {

    /* Store x, y coordinates in convenient way */
    struct Coordinate 
    {
        unsigned int x, y;
        // Coordinate(unsigned int x, unsigned int y);
    };

    /* Store ARGB colors between 0 to 255 */
    struct RGBAColor
    {
        unsigned int color;
        
        uint8_t red(void) const;
        void red(uint32_t);
        
        uint8_t green(void) const;
        void green(uint32_t);
        
        uint8_t blue(void) const;
        void blue(uint32_t);
    };
    
    /* Store x, y coordinates with RGBAColor in a convenient way */
    struct ColorCoordinate 
    {
        unsigned int x, y, color;
        
        uint8_t red(void) const;
        void red(uint32_t);
        
        uint8_t green(void) const;
        void green(uint32_t);
        
        uint8_t blue(void) const;
        void blue(uint32_t);
    };

    struct BoxCoordinate : public Coordinate {
        unsigned int width, height;
        BoxCoordinate(unsigned int x, unsigned int y, unsigned int width, unsigned int height);
    };

    /* Keeps x, y, w, h clamped to limits */
    struct BoundedCoordinate : public BoxCoordinate {
        unsigned int max_x, max_y;
        BoundedCoordinate(unsigned int x, unsigned int y, unsigned int width, unsigned int height, unsigned int max_x, unsigned int max_y);
        BoundedCoordinate(BoxCoordinate box, unsigned int max_x, unsigned int max_y);
        void clampToMax(unsigned int max_x, unsigned int max_y);
    };

    /* We'll be using ColorCoordinate a lot */
    using Pixel = PixelSort::ColorCoordinate;
}

#endif /* _PIXELSORT_COORDINATE_HPP_ */
