
#include <vector>

/*
 * Coordinate.hpp
 * The Coordinate class and its subclasses provide
 * some simple structures to make it easier to work with Pixels
 * Colors, and boundaries.
 */

#ifndef _PIXELSORT_COORDINATE_HPP_
#define _PIXELSORT_COORDINATE_HPP_

namespace PixelSort {

    /* Store x, y coordinates in convenient way */
    struct Coordinate {
        unsigned int x, y;
        Coordinate(unsigned int x, unsigned int y);
    };

    /* Store RGB colors between 0 to 1 */
    struct RGBColor {
        double r, g, b;

        RGBColor(double r, double g, double b);
        
        double red(void) const;
        void red(double);
        
        double green(void) const;
        void green(double);
        
        double blue(void) const;
        void blue(double);
    };

    /* Store x, y coordinates with RGBColor in a convenient way */
    struct ColorCoordinate : public Coordinate, public RGBColor {
        ColorCoordinate();
        ColorCoordinate(const Coordinate& coord, const RGBColor& color);
    };

    struct BoxCoordinate : public Coordinate {
        unsigned int width, height;
        BoxCoordinate(int x, int y, int width, int height);
    };

    /* Keeps x, y, w, h clamped to limits */
    struct BoundedCoordinate : public BoxCoordinate {
        unsigned int max_x, max_y;
        BoundedCoordinate(int x, int y, int width, int height, int max_x, int max_y);
        BoundedCoordinate(BoxCoordinate box, int max_x, int max_y);
        void clampToMax(unsigned int max_x, unsigned int max_y);
    };

    /* We'll be using ColorCoordinate a lot */
    using Pixel = PixelSort::ColorCoordinate;

    /* Main storage type */
    // using PixelVector = std::vector<Pixel>;

}

#endif /* _PIXELSORT_COORDINATE_HPP_ */
